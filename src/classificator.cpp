#include "classificator.h"
#include "rule_tree.h"
#include <deque>
#include <set>

TClassificator::TClassificator(const TClassificatorConfig& cfg)
    : TrainData(cfg.TrainDataFilename, cfg.TrainLabelsFilename)
    , TestData(cfg.TestDataFilename, cfg.TestLabelsFilename)
{
}

TRuleList TClassificator::FindBestRuleList() const {
    TRuleList result;

    const size_t sampleSize = TrainData.Features.empty() ? 0 : TrainData.FeaturesStorage.front().Values->size();
    TRuleTree cache(sampleSize);
    std::deque<std::shared_ptr<TRuleNode>> queue{cache.Root};

    while (not queue.empty()) {
        auto parent = queue.front();
        queue.pop_front();

        std::set<int> usedRules = parent->GetUsedRules();
        for (auto& [feature, featureValues] : TrainData.Features) {
            if (usedRules.contains(feature)) {
                continue;
            }
            for (auto& [label, labelValue] : TrainData.Labels) {
                for (bool featureValue : {false, true}) {
                    auto child = parent->Children.emplace_back(std::make_shared<TRuleNode>(sampleSize));
                    child->Rule.Antecedent = {
                        .Feature = feature,
                        .Value = featureValue,
                    };
                    child->Rule.Label = label;
                    if (featureValue) {
                        child->Captured = parent->Captured | (*featureValues);
                    } else {
                        child->Captured = parent->Captured | ~(*featureValues);
                    }
                }
            }
        }
    }

    return result;
}

std::ostream& operator<<(std::ostream& os, const TClassificator& classificator) {
    os << "Train sample:" << std::endl << classificator.TrainData << "Test sample:" << std::endl << classificator.TestData;
    return os;
}
