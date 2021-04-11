#include "arguments_parser.h"
#include <iostream>

void PrintUsage(std::string_view programName) {
    std::cerr << "Usage: " << programName << " config.cfg" << std::endl;
}

TClassificatorConfig ParseConfig(std::string_view configFile) {
    TClassificatorConfig result;

    libconfig::Config config;
    try {
        config.readFile(configFile.data());
    } catch (const libconfig::FileIOException& fioex) {
        std::cerr << "I/O error while reading config file " << configFile << std::endl;
        throw;
    } catch(const libconfig::ParseException& pex) {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine() << " - " << pex.getError() << std::endl;
        throw;
    }

    try {
        result.TrainDataFilename = static_cast<std::string>(config.lookup("train_data_filename"));
    } catch (const libconfig::SettingNotFoundException& nfex) {
        std::cerr << "No 'train_data_filename' section in config" << std::endl;
        throw;
    }
    try {
        result.TrainLabelsFilename = static_cast<std::string>(config.lookup("train_labels_filename"));
    } catch (const libconfig::SettingNotFoundException& nfex) {
        std::cerr << "No 'train_labels_filename' section in config" << std::endl;
        throw;
    }
    try {
        result.TestDataFilename = static_cast<std::string>(config.lookup("test_data_filename"));
    } catch (const libconfig::SettingNotFoundException& nfex) {
        std::cerr << "No 'test_data_filename' section in config" << std::endl;
        throw;
    }
    try {
        result.TestLabelsFilename = static_cast<std::string>(config.lookup("test_labels_filename"));
    } catch (const libconfig::SettingNotFoundException& nfex) {
        std::cerr << "No 'test_labels_filename' section in config" << std::endl;
        throw;
    }

    try {
        config.lookupValue("regularization_constant", result.RegularizationConstant);
    } catch (const libconfig::SettingNotFoundException& nfex) {
        std::cerr << "No 'regularization_constant' section in config" << std::endl;
        throw;
    } catch (const libconfig::SettingTypeException& tex) {
        std::cerr << "Type of 'regularization_constant' is not double" << std::endl;
        throw;
    }

    try {
        config.lookupValue("max_iterations", result.MaxIterations);
    } catch (const libconfig::SettingNotFoundException& nfex) {
        std::cerr << "No 'max_iterations' section in config" << std::endl;
        throw;
    } catch (const libconfig::SettingTypeException& tex) {
        std::cerr << "Type of 'max_iterations' is not unsigned int" << std::endl;
        throw;
    }

    return result;
}
