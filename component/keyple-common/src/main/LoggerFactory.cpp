

#include "LoggerFactory.h"

namespace keyple {
namespace common {

std::mutex LoggerFactory::mtx;

std::shared_ptr<Logger> LoggerFactory::getLogger(const std::type_info& type)
{
    Logger* logger = new Logger(type.name(), &mtx);
    return std::shared_ptr<Logger>(logger);
}

}
}
