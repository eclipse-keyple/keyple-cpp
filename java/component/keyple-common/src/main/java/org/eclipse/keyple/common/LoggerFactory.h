#pragma once

#include <memory>
#include <typeinfo>
#include <vector>

/* Common*/
#include "Export.h"
#include "Logger.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace common {

                class EXPORT LoggerFactory {
                  public:
                    /**
                     *
                     */
                    static std::shared_ptr<Logger> getLogger(const std::type_info &type);

                    /**
                     *
                     */
                    static void removeLogger(Logger *logger);

                  private:
                    /**
                     *
                     */
                    static std::vector<Logger *> factory;
                };
            } // namespace common
        }     // namespace keyple
    }         // namespace eclipse
} // namespace org
