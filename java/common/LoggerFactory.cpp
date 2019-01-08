#include "LoggerFactory.h"

Logger privateLogger;

std::shared_ptr<Logger> getLogger(std::type_info type) {
	return std::shared_ptr(&privateLogger);
}
