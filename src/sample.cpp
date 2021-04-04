#include "sample.h"
#include <algorithm>
#include <fstream>

TSample::TSample(std::string_view featuresFilename, std::string_view labelsFilename) {
    auto parseFile = [](std::vector<TFeature>& features, std::string_view filename) {
        std::ifstream file(filename.data(), std::ifstream::in);
        while (not file.eof()) {
            features.emplace_back(file);
        }
        features.pop_back();
    };

    parseFile(FeaturesStorage, featuresFilename);
    parseFile(LabelsStorage, labelsFilename);

    auto setupMappings = [](std::vector<TFeature>& storage, std::unordered_map<std::string_view, int>& mapping,
                            std::unordered_map<int, std::string_view>& reverseMapping,
                            std::unordered_map<int, std::shared_ptr<boost::dynamic_bitset<>>>& features)
    {
        int currentClass = 0;
        for (const auto& feature : storage) {
            mapping[feature.Label] = currentClass;
            reverseMapping[currentClass] = feature.Label;
            features[currentClass] = feature.Values;
            ++currentClass;
        }
    };

    setupMappings(FeaturesStorage, FeaturesMapping, FeaturesReverseMapping, Features);
    setupMappings(LabelsStorage, LabelsMapping, LabelsReverseMapping, Labels);
}

std::ostream& operator<<(std::ostream& os, const TSample& sample) {
    os << "===FeaturesStorage===" << std::endl;
    for (auto& feature : sample.FeaturesStorage) {
        os << feature;
    }
    os << "====LabelsStorage====" << std::endl;
    for (auto& label : sample.LabelsStorage) {
        os << label;
    }
    os << "===FeaturesMapping===" << std::endl;
    for (auto& [k, v] : sample.FeaturesMapping) {
        os << k << " = " << v << std::endl;
    }
    os << "===LabelsMapping===" << std::endl;
    for (auto& [k, v] : sample.LabelsMapping) {
        os << k << " = " << v << std::endl;
    }
    os << "===Features===" << std::endl;
    for (auto [k, v] : sample.Features) {
        os << k << " = " << *v << std::endl;
    }
    os << "===Labels===" << std::endl;
    for (auto [k, v] : sample.Labels) {
        os << k << " = " << *v << std::endl;
    }
    return os;
}
