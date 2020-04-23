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

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <cstdio>
#include <ostream>
#include <sstream>

#ifdef __GNUG__ // gnu C++ compiler
#include <cxxabi.h>
#endif

/* Common*/
#include "Export.h"

namespace keyple {
namespace common {

class IMPORT Logger {
public:
    /**
     *
     */
    enum class Level {
        logNone = 0,
        logError,
        logWarn,
        logInfo,
        logTrace,
        logDebug
    };

    /**
     * Constructor
     */
    Logger(const std::string& className, std::mutex* mtx);

    /**
     * Destructor
     */
    ~Logger();

    /**
     *
     */
    std::string getClassName();

    /**
     *
     */
    static void setLoggerLevel(Level level);

    /**
     *
     */
    template <typename... Args>
    void trace(const std::string& format, Args... args)
    {
        if (level >= Level::logTrace)
            log("TRACE", format, std::forward<Args>(args)...);
    }

    /**
	 *
	 */
    template <typename... Args>
    void debug(const std::string& format, Args... args)
    {
        if (level >= Level::logDebug)
            log("DEBUG", format, std::forward<Args>(args)...);
    }

    /**
	 *
	 */
    template <typename... Args>
    void warn(const std::string& format, Args... args)
    {
        if (level >= Level::logWarn)
            log("WARN", format, std::forward<Args>(args)...);
    }

    /**
	 *
	 */
    template <typename... Args>
    void info(const std::string& format, Args... args)
    {
        if (level >= Level::logInfo)
            log("INFO", format, std::forward<Args>(args)...);
    }

    /**
	 *
	 */
    template <typename... Args>
    void error(const std::string& format, Args... args)
    {
        if (level >= Level::logError)
            log("ERROR", format, std::forward<Args>(args)...);
    }

private:
    /**
     *
     */
    static Level level;

    /**
     *
     */
    const size_t maxClassNameLength = 100;

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
    std::string demangle(const char* mangled_name)
    {
        std::size_t len = 0;
        int status      = 0;
        std::unique_ptr<char, decltype(&std::free)> ptr(
            __cxxabiv1::__cxa_demangle(mangled_name, nullptr, &len, &status),
            &std::free);
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

    void printf(std::ostringstream& os, const char* s)
    {
        while (s && *s) {
            if (*s == '%' && *(s + 1) != '%')
                throw std::runtime_error("invalid format: missing arguments");
            os << *s++;
        }
    }

    template <typename T, typename... Args>
    void printf(std::ostringstream& os, const char* s, T& value, Args... args)
    {
        while (s && *s) {
            if (*s == '%' && *(s + 1) != '%') {
                os << value;
                return printf(os, ++s, args...);
            }
            os << *s++;
        }
        throw std::runtime_error("extra arguments provided to printf");
    }

    template <typename T, typename... Args>
    void printf(std::ostringstream& os, const char* s, T* value, Args... args)
    {
        while (s && *s) {
            if (*s == '%' && *(s + 1) != '%') {
                os << *value;
                return printf(os, ++s, args...);
            }
            os << *s++;
        }
        throw std::runtime_error("extra arguments provided to printf");
    }

    /**
	 * Because of variadic templates usage, the function must be declared and
	 * defined in the header file.
	 */
    template <typename... Args>
    void log(const std::string& label, const std::string& format, Args... args)
    {
        mtx->lock();

        std::printf("[%5s]   [%-100s]   ", label.c_str(), className.c_str());
        std::ostringstream os;
        printf(os, format.c_str(), args...);
        const std::string& str = os.str();
        std::printf("%s", str.c_str());

        mtx->unlock();
    }
};

}
}

namespace std {
/**
 * Helper functions
 */
IMPORT std::ostream& operator<<(std::ostream& os,
                                const std::vector<uint8_t>& v);
}
