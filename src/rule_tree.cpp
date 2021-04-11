#include "rule_tree.h"

TRuleNode::TRuleNode(size_t sampleSize)
    : Rule()
    , Captured(sampleSize, 0)
    , NotCaptured(sampleSize, 0)
    , Children()
    , Parent()
    , LowerBound(0.0)
    , Error(0.0)
{
}

TRuleTree::TRuleTree(size_t sampleSize)
    : Root(std::make_shared<TRuleNode>(sampleSize))
{
}
