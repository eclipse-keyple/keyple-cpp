#pragma once

#include <typeinfo>
#include "Logger.h"

class LoggerFactory
{
public:
	static std::shared_ptr<Logger> getLogger(const std::type_info &type)
	{
		Logger *logger = new Logger();
		return std::shared_ptr<Logger>(logger);
	}
};
