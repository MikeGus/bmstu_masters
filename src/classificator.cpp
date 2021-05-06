#include "classificator.h"
#include "rule_tree.h"
#include <cmath>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>

template<>
struct std::less<std::shared_ptr<TRuleNode>> {
    bool operator()(const std::shared_ptr<TRuleNode> a, const std::shared_ptr<TRuleNode> b) const {
        return a->LowerBound * (static_cast<float>(a->Captured.size()) / a->Captured.count()) < b->LowerBound * (static_cast<float>(b->Captured.size()) / b->Captured.count());
    }
};

using TRuleTestingPolicy = std::deque<std::shared_ptr<TRuleNode>>;

TClassificator::TClassificator(const TClassificatorConfig& cfg)
    : TrainData(cfg.TrainDataFilename, cfg.TrainLabelsFilename)
    , TestData(cfg.TestDataFilename, cfg.TestLabelsFilename)
    , RegularizationConstant(cfg.RegularizationConstant)
    , MaxIterations(cfg.MaxIterations)
{
}

size_t TClassificator::CalculateTrainSampleSize() const {
    return TrainData.Features.empty() ? 0 : TrainData.FeaturesStorage.front().Values->size();
}

size_t TClassificator::CalculateTestSampleSize() const {
    return TestData.Features.empty() ? 0 : TestData.FeaturesStorage.front().Values->size();
}

double TClassificator::CalculateInitialMinError() const {
    const size_t sampleSize = CalculateTrainSampleSize();

    size_t maxCount = 0;
    for (auto& [_, labelValues] : TrainData.Labels) {
        maxCount = std::max(maxCount, labelValues->count());
    }

    return 1.0 - (static_cast<double>(maxCount) / sampleSize);
}

std::unordered_map<int, double> TClassificator::CalculateAccuracy(TRuleList bestRuleList) const {
    const size_t sampleSize = CalculateTestSampleSize();
    for (auto& rule : bestRuleList.Rules) {
        const auto ruleAntecedentFeature = TrainData.FeaturesReverseMapping.at(rule.Antecedent.Feature);
        rule.Antecedent.Feature = TestData.FeaturesMapping.at(ruleAntecedentFeature);
        const auto ruleLabel = TrainData.LabelsReverseMapping.at(rule.Label);
        rule.Label = TestData.LabelsMapping.at(ruleLabel);
    }
    bestRuleList.DefaultLabel = TestData.LabelsMapping.at(TrainData.LabelsReverseMapping.at(bestRuleList.DefaultLabel));

    std::unordered_map<int, boost::dynamic_bitset<>> classificatedLabels;
    for (const auto& [label, _] : TestData.Labels) {
        classificatedLabels[label] = boost::dynamic_bitset<>(sampleSize, 0);
    }
    classificatedLabels[bestRuleList.DefaultLabel].flip();

    boost::dynamic_bitset<> captured(sampleSize, 0);
    for (const auto& rule : bestRuleList.Rules) {
        const auto featureValues = TestData.Features.at(rule.Antecedent.Feature);
        const boost::dynamic_bitset<> newCaptured = rule.Antecedent.Value ? (captured | *featureValues) : (captured | (~ *featureValues));
        const boost::dynamic_bitset<> capturedByLast = newCaptured ^ captured;
        classificatedLabels[rule.Label] |= capturedByLast;
        captured = std::move(newCaptured);
    }
    std::unordered_map<int, size_t> totalCorrect(TestData.Labels.size());
    for (const auto& [label, labelValues] : TestData.Labels) {
        const size_t truePositives = (*labelValues & classificatedLabels.at(label)).count();
        const size_t trueNegatives = ((~ *labelValues) & (~ classificatedLabels.at(label))).count();
        totalCorrect[label] += truePositives + trueNegatives;
    }

    std::unordered_map<int, double> accuracies(TestData.Labels.size());
    for (auto [label, correct] : totalCorrect) {
        accuracies[label] = static_cast<double>(correct) / sampleSize;
    }

    return accuracies;
}

TRuleList TClassificator::FindBestRuleList() const {
    TRuleList result;

    const size_t sampleSize = CalculateTrainSampleSize();

    TRuleTree cache(sampleSize);
    cache.Root->NotCaptured = ~cache.Root->Captured;
    TRuleTestingPolicy queue{cache.Root};

    const double initialMinError = CalculateInitialMinError();
    double minError = initialMinError;

    std::unordered_map<int, boost::dynamic_bitset<>> notFeatures;
    for (auto& [feature, featureValues] : TrainData.Features) {
        notFeatures[feature] = ~ *featureValues;
    }

    std::map<std::set<int>, double> permutationCache;

    size_t iterations = 0;
    while (not queue.empty() and ++iterations < MaxIterations)  {
        auto queueIt = queue.begin();
        std::shared_ptr<TRuleNode> parent = *queueIt;
        queue.erase(queueIt);

        if (parent->LowerBound + RegularizationConstant > minError) {
            continue;
        }

        for (auto& [feature, featureValues] : TrainData.Features) {
            if (parent->UsedFeatures.contains(feature)) {
                continue;
            }
            TRuleNode newNode(sampleSize);
            newNode.Rule.Antecedent.Feature = feature;
            newNode.Parent = parent;
            newNode.UsedFeatures = parent->UsedFeatures;
            newNode.UsedFeatures.emplace(feature);

            for (bool featureValue : {false, true}) {
                newNode.Rule.Antecedent.Value = featureValue;
                const boost::dynamic_bitset<> capturedByChildOnly = featureValue ? (parent->NotCaptured & *featureValues) : (parent->NotCaptured & notFeatures.at(feature));
                const size_t capturedByChildOnlyCount = capturedByChildOnly.count();
                if (capturedByChildOnlyCount == 0) {
                    continue;
                }
                const double capturedByChildOnlyFraction = capturedByChildOnlyCount / static_cast<double>(sampleSize);
                if (capturedByChildOnlyFraction < RegularizationConstant) {
                    continue;
                }

                newNode.Captured = parent->Captured | capturedByChildOnly;
                newNode.NotCaptured = ~ newNode.Captured;

                for (auto& [ruleLabel, ruleLabelValues] : TrainData.Labels) {
                    auto child = parent->Children.emplace_back(std::make_shared<TRuleNode>(newNode));
                    child->Rule.Label = ruleLabel;
                    const boost::dynamic_bitset<> capturedByChildOnlyAndLabel = capturedByChildOnly & *ruleLabelValues;
                    const size_t capturedByChildOnlyAndLabelCount = capturedByChildOnlyAndLabel.count();
                    const double labelFractionInCapturedByChildOnly = static_cast<double>(capturedByChildOnlyAndLabelCount) / capturedByChildOnlyCount;
                    const double lowerBoundByChild = capturedByChildOnlyFraction * (1.0 -  labelFractionInCapturedByChildOnly) + RegularizationConstant;
                    double& lowerBound = child->LowerBound;
                    lowerBound = parent->LowerBound + lowerBoundByChild;
                    if (lowerBound + RegularizationConstant < minError) {
                        if (not permutationCache.contains(child->UsedFeatures) or permutationCache.at(child->UsedFeatures) > lowerBound) {
                            permutationCache[child->UsedFeatures] = lowerBound;
                            queue.emplace_back(child);
                        } else {
                            continue;
                        }
                    } else {
                        parent->Children.pop_back();
                        child->PruneParents(minError);
                        continue;
                    }

                    double& error = child->Error;

                    const boost::dynamic_bitset<> notCaptured = child->NotCaptured;
                    const size_t notCapturedCount = notCaptured.count();
                    const double notCapturedFraction = notCapturedCount / static_cast<double>(sampleSize);

                    int defaultLabel = -1;
                    size_t defaultCount = 0;
                    for (auto& [label, labelValues] : TrainData.Labels) {
                        const boost::dynamic_bitset<> notCapturedAndCurrentLabel = notCaptured & *labelValues;
                        const size_t notCapturedAndCurrentLabelCount = notCapturedAndCurrentLabel.count();
                        if ((defaultLabel == -1) or (notCapturedAndCurrentLabelCount > defaultCount)) {
                            defaultLabel = label;
                            defaultCount = notCapturedAndCurrentLabelCount;
                        }
                    }

                    const double defaultLabelFractionInNonCaptured = static_cast<double>(defaultCount) / notCapturedCount;
                    error = lowerBound + notCapturedFraction * (1.0 - defaultLabelFractionInNonCaptured);
                    if (error < minError) {
                        minError = error;
                        result.DefaultLabel = defaultLabel;

                        result.Rules.clear();
                        for (std::shared_ptr<TRuleNode> currentNode = child; currentNode->Parent.lock(); currentNode = currentNode->Parent.lock()) {
                            result.Rules.emplace_front(currentNode->Rule);
                        }
                        /*
                        std::cout << "============================" << std::endl;
                        std::cout << "New best rule: " << std::endl << result.AsString(TrainData.FeaturesReverseMapping, TrainData.LabelsReverseMapping) << std::endl;
                        std::cout << "New min lb: " << lowerBound << std::endl;
                        std::cout << "New child lb: " << lowerBoundByChild << std::endl;
                        std::cout << "New parent lb: " << parent->LowerBound << std::endl;
                        std::cout << "Captured by child only: " << capturedByChildOnlyFraction << std::endl;
                        std::cout << "Label " << TrainData.LabelsReverseMapping.at(ruleLabel) << " fraction in captured by child only: " << labelFractionInCapturedByChildOnly << std::endl;
                        std::cout << "Default label fraction: " << defaultLabelFractionInNonCaptured << std::endl;
                        std::cout << "Not captured: " << notCapturedFraction << std::endl;
                        std::cout << "New min error: " << minError << std::endl;
                        std::cout << "============================" << std::endl;
                        */
                    }
                }
            }
        }
    }

    const auto accuracy = CalculateAccuracy(result);
    std::cout << "============================" << std::endl;
    std::cout << "BEST RULE: " << std::endl << result.AsString(TrainData.FeaturesReverseMapping, TrainData.LabelsReverseMapping) << std::endl;
    std::cout << "OBJECTIVE: " <<  minError << std::endl;
    for (auto& [label, _] : TestData.Labels) {
        std::cout << "ACCURACY FOR CLASS " << TestData.LabelsReverseMapping.at(label) << ": " <<  accuracy.at(label) << std::endl;
    }
    std::cout << "TOTAL ITERATIONS: " << iterations << std::endl;
    std::cout << "============================" << std::endl;

    return result;
}

std::ostream& operator<<(std::ostream& os, const TClassificator& classificator) {
    os << "Train sample:" << std::endl << classificator.TrainData << "Test sample:" << std::endl << classificator.TestData;
    return os;
}
