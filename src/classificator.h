#pragma once

#include "classificator_config.h"
#include "sample.h"
#include <iostream>

class TClassificator {
public:
    explicit TClassificator(const TClassificatorConfig& cfg);
    friend std::ostream& operator<<(std::ostream& os, const TClassificator& classificator);
private:
    TSample TrainData;
    TSample TestData;
};
