#pragma once

#include <typeinfo>
#include "Logger.h"

class LoggerFactory
{
private:
	static Logger privateLogger;

public:
	static std::shared_ptr<Logger> getLogger(std::type_info type)
	{

		return std::shared_ptr<Logger>(&privateLogger);
	}
};
