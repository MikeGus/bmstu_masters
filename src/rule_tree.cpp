#include "rule_tree.h"
#include <algorithm>

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

void TRuleNode::PruneParents(double minError) {
    std::shared_ptr<TRuleNode> parent = Parent.lock();
    while (parent and parent->LowerBound < minError) {
        parent = parent->Parent.lock();
    }
    if (parent) {
        std::shared_ptr<TRuleNode> parentOfParent = parent->Parent.lock();
        if (parentOfParent) {
            parentOfParent->Children.erase(std::remove(parentOfParent->Children.begin(), parentOfParent->Children.end(), parent));
        }
    }
}

TRuleTree::TRuleTree(size_t sampleSize)
    : Root(std::make_shared<TRuleNode>(sampleSize))
{
}
