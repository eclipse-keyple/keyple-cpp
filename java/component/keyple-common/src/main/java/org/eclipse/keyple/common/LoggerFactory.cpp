#include "LoggerFactory.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace common {

                std::vector<Logger *> LoggerFactory::factory;

                std::shared_ptr<Logger> LoggerFactory::getLogger(const std::type_info &type)
                {
                    /* Look for an existing logger */
                    for (auto logger : factory)
                    {
                        if (!logger->getClassName().compare(type.name()))
                        {
                            return std::shared_ptr<Logger>(logger);
                        }
                    }

                    /* Create a new one */
                    Logger *logger = new Logger(type.name());
                    factory.push_back(logger);
                    return std::shared_ptr<Logger>(logger);
                }

                void LoggerFactory::removeLogger(Logger *logger)
                {
                    /* Look for an existing logger */
                    for (auto it = factory.begin(); it != factory.end(); it++)
                    {
                        if (!(*it)->getClassName().compare(logger->getClassName()))
                        {
                            factory.erase(it);
                            return;
                        }
                    }
                }

            } // namespace common
        }     // namespace keyple
    }         // namespace eclipse
} // namespace org
