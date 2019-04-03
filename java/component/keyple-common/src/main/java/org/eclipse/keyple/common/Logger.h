#pragma once

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

#ifdef __GNUG__ // gnu C++ compiler
    #include <cxxabi.h>
#endif

/* Common*/
#include "Export.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace common {

                class EXPORT Logger {
                  private:
                    /**
                     *
                     */
                    bool traceEnabled;

                    /**

                     *
                     */
                    bool debugEnabled;

                    /**
                     *
                     */
                    bool warnEnabled;

                    /**
                     *
                     */
                    bool infoEnabled;

                    /**
                     *
                     */
                    bool errorEnabled;

                    /**
                     *
                     */
                    const std::string className;

                    /**
                     *
                     */
                    void log(const char *s)
                    {
                        while (s && *s)
                        {
                            if (*s == '%' &&
                                *++s != '%') // make sure that there wasn't meant to be more arguments
                                             // %% represents plain % in a format string
                                throw std::runtime_error("invalid format: missing arguments");
                            std::cout << *s++;
                        }
                    }

                    /**
                     * Actual printing function
                     */
                    template <typename T, typename... Args>
                    void log(const char *s, T value, Args... args)
                    {
                        while (s && *s)
                        {
                            if (*s == '%' && *++s != '%')
                            {                             // a format specifier (ignore which one it is)
                                std::cout << value;       // use first non-format argument
                                return log(++s, args...); // ``peel off'' first argument
                            }
                            std::cout << *s++;
                        }
                        throw std::runtime_error("extra arguments provided to log");
                    }

                  public:
                    /**
	                 * Constructor
	                 */
                    Logger(const std::string &className);

                    /**
                     * Destructor
                     */
                    ~Logger();

                    bool isTraceEnabled();

                    bool isDebugEnabled();

                    bool isWarnEnabled();

                    bool isInfoEnabled();

                    bool isErrorEnabled();

                    std::string getClassName();

                    void setTraceEnabled(bool enabled);

                    void setDebugEnabled(bool enabled);

                    void setWarnEnabled(bool enabled);

                    void setInfoEnabled(bool enabled);

                    void setErrorEnabled(bool enabled);

#ifdef __GNUG__ // gnu C++ compiler
                    std::string demangle( const char* mangled_name ) {
                        std::size_t len = 0 ;
                        int status = 0 ;
                        std::unique_ptr< char, decltype(&std::free) > ptr(
                        __cxxabiv1::__cxa_demangle( mangled_name, nullptr, &len, &status ), &std::free ) ;
                        return ptr.get() ;
                    }
#else
                    std::string demangle(const char* name) {
                        return name;
                    }
#endif // _GNUG_

                    void trace(const char *s)
                    {
                        if (traceEnabled)
                        {
                            std::cout << "[TRACE]   [" << className << "]   ";
                            log(s);
                        }
                    }

                    template <typename T, typename... Args>
                    void trace(const char *s, T value, Args... args)
                    {
                        if (traceEnabled)
                        {
                            std::cout << "[TRACE]   [" << className << "]   ";
                            log(s, value, std::forward<Args>(args)...);
                        }
                    }

                    void debug(const char *s)
                    {
                        if (debugEnabled)
                        {
                            std::cout << "[DEBUG]   [" << className << "]   ";
                            log(s);
                        }
                    }

                    template <typename T, typename... Args>
                    void debug(const char *s, T value, Args... args)
                    {
                        if (debugEnabled)
                        {
                            std::cout << "[DEBUG]   [" << className << "]   ";
                            log(s, value, std::forward<Args>(args)...);
                        }
                    }

                    void warn(const char *s)
                    {
                        if (warnEnabled)
                        {
                            std::cout << "[ WARN]   [" << className << "    ";
                            log(s);
                        }
                    }

                    template <typename T, typename... Args>
                    void warn(const char *s, T value, Args... args)
                    {
                        if (warnEnabled)
                        {
                            std::cout << "[ WARN]   [" << className << "]   ";
                        }
                    }

                    void info(const char *s)
                    {
                        if (infoEnabled)
                        {
                            std::cout << "[ INFO]   [" << className << "]   ";
                            log(s);
                        }
                    }

                    template <typename T, typename... Args>
                    void info(const char *s, T value, Args... args)
                    {
                        if (infoEnabled)
                        {
                            std::cout << "[ INFO]   [" << className << "]   ";
                            log(s, value, std::forward<Args>(args)...);
                        }
                    }

                    void error(const char *s)
                    {
                        if (errorEnabled)
                        {
                            std::cout << "[ERROR]   [" << className << "]   ";
                            log(s);
                        }
                    }

                    template <typename T, typename... Args>
                    void error(const char *s, T value, Args... args)
                    {
                        if (errorEnabled)
                        {
                            std::cout << "[ERROR]   [" << className << "]   ";
                            log(s, value, std::forward<Args>(args)...);
                        }
                    }
                };
            } // namespace common
        }
    }
}

