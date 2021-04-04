#pragma once

#include "rule.h"

#include <boost/dynamic_bitset.hpp>

#include <memory>
#include <set>
#include <vector>

struct TRuleNode {
    TRuleNode(size_t sampleSize);
    std::set<int> GetUsedRules() const;


    TRule Rule;
    boost::dynamic_bitset<> Captured;
    std::vector<std::shared_ptr<TRuleNode>> Children;
    std::weak_ptr<TRuleNode> Parent;

    double LowerBound = 0.0;
};

struct TRuleTree {
    explicit TRuleTree(size_t sampleSize);

    std::shared_ptr<TRuleNode> Root;
};
