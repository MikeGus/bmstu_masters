#pragma once

#include "rule.h"
#include <string>
#include <string_view>
#include <unordered_map>
#include <deque>

struct TRuleList {
    std::string AsString(const std::unordered_map<int, std::string_view>& featuresMapping, const std::unordered_map<int, std::string_view>& labelsMapping) const;

    std::deque<TRule> Rules;
    int DefaultLabel = -1;
};
