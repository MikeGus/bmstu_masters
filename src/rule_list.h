#pragma once

#include "rule.h"
#include <vector>

struct TRuleList {
    std::vector<TRule> Rules;
    std::string_view DefaultClass;
};
