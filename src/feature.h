#pragma once

#include <boost/dynamic_bitset.hpp>
#include <memory>

struct TFeature {
    explicit TFeature(std::istream& inputStream);
    friend std::ostream& operator<<(std::ostream& os, const TFeature& feature);

    std::string Label;
    std::shared_ptr<boost::dynamic_bitset<>> Values;
};
