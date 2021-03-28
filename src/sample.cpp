#include "sample.h"
#include <algorithm>
#include <fstream>

TSample::TSample(std::string_view featuresFilename, std::string_view labelsFilename) {
    auto parseFile = [](std::vector<TFeature>& features, std::string_view filename) {
        std::ifstream file(filename.data(), std::ifstream::in);
        while (not file.eof()) {
            features.emplace_back(file);
        }
    };

    parseFile(Features, featuresFilename);
    parseFile(Labels, labelsFilename);
}

std::ostream& operator<<(std::ostream& os, const TSample& sample) {
    os << "===Features===" << std::endl;
    for (auto& feature : sample.Features) {
        os << feature;
    }
    os << "====Labels====" << std::endl;
    for (auto& label : sample.Labels) {
        os << label;
    }
    return os;
}
