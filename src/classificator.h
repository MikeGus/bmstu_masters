#pragma once

#include "classificator_config.h"
#include "rule_list.h"
#include "sample.h"
#include <iostream>

class TClassificator {
public:
    explicit TClassificator(const TClassificatorConfig& cfg);

    TRuleList FindBestRuleList() const;
    double CalculateInitialMinError() const;

    friend std::ostream& operator<<(std::ostream& os, const TClassificator& classificator);
private:
    const TSample TrainData;
    const TSample TestData;
    const double RegularizationConstant;
};
