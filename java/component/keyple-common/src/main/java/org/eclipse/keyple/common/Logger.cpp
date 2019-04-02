#include "Logger.h"
#include "LoggerFactory.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace common {

                class LoggerFactory;

                Logger::Logger(const std::string &className) : className(demangle(className.c_str()))
                {
                    traceEnabled = 1;
                    debugEnabled = 1;
                    warnEnabled  = 1;
                    infoEnabled  = 1;
                    errorEnabled = 1;
                }

                Logger::~Logger()
                {

                }

                bool Logger::isTraceEnabled()
                {
                    return traceEnabled;
                }

                bool Logger::isDebugEnabled()
                {
                    return debugEnabled;
                }

                bool Logger::isWarnEnabled()
                {
                    return warnEnabled;
                }

                bool Logger::isInfoEnabled()
                {
                    return infoEnabled;
                }

                bool Logger::isErrorEnabled()
                {
                    return errorEnabled;
                }

                std::string Logger::getClassName()
                {
                    return className;
                }

                void Logger::setTraceEnabled(bool enabled)
                {
                    traceEnabled = enabled;
                }

                void Logger::setDebugEnabled(bool enabled)
                {
                    debugEnabled = enabled;
                }

                void Logger::setWarnEnabled(bool enabled)
                {
                    warnEnabled = enabled;
                }

                void Logger::setInfoEnabled(bool enabled)
                {
                    infoEnabled = enabled;
                }

                void Logger::setErrorEnabled(bool enabled)
                {
                    errorEnabled = enabled;
                }

            } // namespace common
        }     // namespace keyple
    }         // namespace eclipse
} // namespace org
