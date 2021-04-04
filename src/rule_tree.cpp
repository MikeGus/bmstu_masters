#include "rule_tree.h"

std::set<int> TRuleNode::GetUsedRules() const {
    std::set<int> result{Rule.Antecedent.Feature};
    for (std::shared_ptr<TRuleNode> parent = Parent.lock(); parent; parent = parent->Parent.lock()) {
        result.emplace(parent->Rule.Antecedent.Feature);
    }
    return result;
}

TRuleNode::TRuleNode(size_t sampleSize)
    : Rule()
    , Captured(sampleSize, 0)
    , Children()
    , Parent()
    , LowerBound(0.0)
{
}

TRuleTree::TRuleTree(size_t sampleSize)
    : Root(std::make_shared<TRuleNode>(sampleSize))
{
}
