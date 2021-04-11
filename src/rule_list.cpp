#include "rule_list.h"
#include <sstream>

std::string TRuleList::AsString(const std::unordered_map<int, std::string_view>& featuresMapping, const std::unordered_map<int, std::string_view>& labelsMapping) const {
    std::stringstream result;
    for (const auto& rule : Rules) {
        result << "if (" << featuresMapping.at(rule.Antecedent.Feature) << " = " << rule.Antecedent.Value << ") then " << labelsMapping.at(rule.Label) << std::endl << "else ";
    }
    result << labelsMapping.at(DefaultLabel);
    return result.str();
}
