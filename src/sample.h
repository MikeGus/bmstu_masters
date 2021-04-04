#pragma once

#include "feature.h"

#include <boost/dynamic_bitset.hpp>
#include <iostream>
#include <string_view>
#include <unordered_map>

struct TSample {
    explicit TSample(std::string_view featuresFilename, std::string_view labelsFilename);
    friend std::ostream& operator<<(std::ostream& os, const TSample& sample);

    std::unordered_map<std::string_view, int> FeaturesMapping;
    std::unordered_map<std::string_view, int> LabelsMapping;

    std::unordered_map<int, std::string_view> FeaturesReverseMapping;
    std::unordered_map<int, std::string_view> LabelsReverseMapping;

    std::unordered_map<int, std::shared_ptr<boost::dynamic_bitset<>>> Features;
    std::unordered_map<int, std::shared_ptr<boost::dynamic_bitset<>>> Labels;

    std::vector<TFeature> FeaturesStorage;
    std::vector<TFeature> LabelsStorage;
};
