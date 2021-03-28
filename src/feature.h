#pragma once

#include <boost/dynamic_bitset.hpp>

struct TFeature {
    explicit TFeature(std::istream& inputStream);
    friend std::ostream& operator<<(std::ostream& os, const TFeature& feature);

    std::string Label;
    boost::dynamic_bitset<> Values;
};
