#pragma once


#include <memory>
#include <typeinfo>

#include "Logger.h"

class LoggerFactory
{
public:
    static std::shared_ptr<Logger> getLogger(const std::type_info &type)
	{
        static std::shared_ptr<Logger> logger;
        if (!logger.get())
            logger.reset(new Logger(type.name()));

        return logger;
	}
};
