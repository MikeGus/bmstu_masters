#include "feature.h"
#include <iostream>

static std::string GetValuesAsString(std::istream& inputStream) {
    std::string valuesAsString;
    char ch = '\0';
    while (inputStream.get(ch)) {
        if (ch == '\n') {
            break;
        }
        if (ch == '0' or ch == '1') {
            valuesAsString += ch;
        }
    }
    return valuesAsString;
}

TFeature::TFeature(std::istream& inputStream) {
    inputStream >> Label;
    std::string valuesAsString = GetValuesAsString(inputStream);
    Values = boost::dynamic_bitset<>(valuesAsString);
}

std::ostream& operator<<(std::ostream& os, const TFeature& feature) {
    os << feature.Label << " " << feature.Values << std::endl;
    return os;
}
