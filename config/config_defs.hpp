#pragma once
#include <config/config.hpp>
#include <ConfigTree/ConfigProcessor.hpp>
#include <ConfigTree/ConfigProcessorMacro.hpp>
CONFIG_BEGIN(grouping_t)
    FIELD(id)
    FIELD(params)
CONFIG_END(grouping_t)
CONFIG_BEGIN(flyItfConfig_t)
    FIELD(mapping)
    FIELD(grouping)
CONFIG_END(flyItfConfig_t)
CONFIG_BEGIN(config_t)
    FIELD(flyItfConfig)
CONFIG_END(config_t)

#undef CONFIG_BEGIN
#undef FIELD
#undef CONFIG_END

