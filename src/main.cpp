#include "arguments_parser.h"
#include "classificator_config.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        PrintUsage(argv[0]);
        return -1;
    }

    try {
        TClassificatorConfig config = ParseConfig(argv[1]);
        std::cout << "Config successfully loaded!" << std::endl << "\tTrain data:\t" << config.TrainDataFilename << std::endl
            << "\tTrain labels:\t" << config.TrainLabelsFilename << std::endl << "\tTest data:\t" << config.TestDataFilename << std::endl
            << "\tTest labels:\t" << config.TestLabelsFilename << std::endl;
    } catch (...) {}

    return 0;
}
