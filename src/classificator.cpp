#include "classificator.h"

TClassificator::TClassificator(const TClassificatorConfig& cfg)
    : TrainData(cfg.TrainDataFilename, cfg.TrainLabelsFilename)
    , TestData(cfg.TestDataFilename, cfg.TestLabelsFilename)
{
}

std::ostream& operator<<(std::ostream& os, const TClassificator& classificator) {
    os << "Train sample:" << std::endl << classificator.TrainData << "Test sample:" << std::endl << classificator.TestData;
    return os;
}
