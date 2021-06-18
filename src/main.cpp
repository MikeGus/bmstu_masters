#include "arguments_parser.h"
#include "classificator.h"
#include "classificator_config.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        PrintUsage(argv[0]);
        return -1;
    }

    try {
        TClassificatorConfig config = ParseConfig(argv[1]);
        TClassificator classificator{config};
        classificator.FindBestRuleList();
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}
