#pragma once

#include "rule.h"

#include <boost/dynamic_bitset.hpp>

#include <memory>
#include <unordered_set>
#include <vector>

struct TRuleNode {
    explicit TRuleNode(size_t sampleSize);
    TRuleNode(const TRuleNode& other) = default;
    TRuleNode(TRuleNode&& other) = delete;

    TRule Rule;

    boost::dynamic_bitset<> Captured;
    boost::dynamic_bitset<> NotCaptured;

    std::unordered_set<int> UsedFeatures;

    std::vector<std::shared_ptr<TRuleNode>> Children;
    std::weak_ptr<TRuleNode> Parent;

    double LowerBound = 0.0;
    double Error = 0.0;
};

struct TRuleTree {
    explicit TRuleTree(size_t sampleSize);

    std::shared_ptr<TRuleNode> Root;
};
