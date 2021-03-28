#pragma once

#include "feature.h"
#include <iostream>
#include <string_view>

struct TSample {
    explicit TSample(std::string_view featuresFilename, std::string_view labelsFilename);
    friend std::ostream& operator<<(std::ostream& os, const TSample& sample);

    std::vector<TFeature> Features;
    std::vector<TFeature> Labels;
};
