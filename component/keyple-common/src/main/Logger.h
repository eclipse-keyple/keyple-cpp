/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

#pragma once

#include <iomanip>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <cstdio>

#ifdef __GNUG__ // gnu C++ compiler
#include <cxxabi.h>
#endif

/* Common*/
#include "Export.h"

/* Forward declaration */
namespace keyple { namespace common { class LoggerFactory; } }

namespace keyple {
namespace common {

class EXPORT Logger {
public:
    /**
     * Constructor
     */
    Logger(const std::string &className, std::mutex* mtx);

    /**
     * Destructor
     */
    ~Logger();

    /**
     *
     */
    bool isTraceEnabled();
    
    /**
     *
     */
    bool isDebugEnabled();
    
    /**
     *
     */
    bool isWarnEnabled();

    /**
     *
     */
    bool isInfoEnabled();

    /**
     *
     */
    bool isErrorEnabled();

    /**
     *
     */
    std::string getClassName();

    /**
     *
     */
    void setTraceEnabled(bool enabled);

    /**
     *
     */
    void setDebugEnabled(bool enabled);

    /**
     *
     */
    void setWarnEnabled(bool enabled);

    /**
     *
     */
    void setInfoEnabled(bool enabled);

    /**
     *
     */
    void setErrorEnabled(bool enabled);

    /**
     *
     */
    void trace(const std::string s, ...);

    /**
     *
     */
    void debug(const std::string s, ...);

    /**
     *
     */
    void warn(const std::string s, ...);

    /**
     *
     */
    void info(const std::string s, ...);

    /**
     *
     */
    void error(const std::string s, ...);

private:
    /**
     *
     */
    const size_t maxClassNameLength = 100;

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
    std::mutex* mtx;

    /**
     *
     */
    #ifdef __GNUG__ // gnu C++ compiler
    std::string demangle( const char* mangled_name )
    {
        std::size_t len = 0 ;
        int status = 0 ;
        std::unique_ptr<char, decltype(&std::free)> ptr(
            __cxxabiv1::__cxa_demangle(mangled_name, nullptr, &len, &status),
                                       &std::free );
        std::string s(ptr.get());
        if (s.size() > maxClassNameLength)
            s.resize(maxClassNameLength);
        return s;
    }
#else
    std::string demangle(const char* name)
    {
        std::string s(name);
        if (s.size() > maxClassNameLength)
            s.resize(maxClassNameLength);
        return s;
    }
#endif // _GNUG_

    template <typename T>
    T const * argument(const std::basic_string<T>& value) noexcept
    {
        return value.c_str();
    }

    char const * argument(const bool& value) noexcept
    {
        return value ? "true" : "false";
    }

    /**
     *
     */
    template <typename... Args>
    void log(const std::string label, const std::string format,  va_list args)
    {
        mtx->lock();
        
        std::printf("[%5s]   [%-100s]   ", label.c_str(), className.c_str());
        std::vprintf(format.c_str(), args);
        
        mtx->unlock();
    }
};

}
}

