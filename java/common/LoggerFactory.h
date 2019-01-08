#pragma once

#include <typeinfo>
#include "Logger.h"

class LoggerFactory
{
private:
	Logger privateLogger;

public:
	static std::shared_ptr<Logger> getLogger(std::type_info type);
};
