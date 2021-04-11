#pragma once

#include <string>

struct TClassificatorConfig {
    std::string TrainDataFilename;
    std::string TrainLabelsFilename;
    std::string TestDataFilename;
    std::string TestLabelsFilename;
    double RegularizationConstant;
    unsigned int MaxIterations;
};
