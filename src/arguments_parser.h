#pragma once

#include "classificator_config.h"

#include <libconfig.h++>
#include <string_view>

void PrintUsage(std::string_view programName);

TClassificatorConfig ParseConfig(std::string_view configFile);
