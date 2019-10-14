#include "LoggerFactory.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace common {

                std::shared_ptr<Logger> LoggerFactory::getLogger(const std::type_info &type)
                {
                    Logger *logger = new Logger(type.name());
                    return std::shared_ptr<Logger>(logger);
                }
            } // namespace common
        }     // namespace keyple
    }         // namespace eclipse
} // namespace org
