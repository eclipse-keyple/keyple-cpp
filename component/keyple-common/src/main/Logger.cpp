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

#include <cstdarg>

#include "Logger.h"
#include "LoggerFactory.h"

namespace keyple {
namespace common {

class LoggerFactory;

using Level = Logger::Level;

Level Logger::level = Level::logDebug;

Logger::Logger(const std::string& className, std::mutex* mtx)
: className(demangle(className.c_str())), mtx(mtx)
{
}

Logger::~Logger()
{
}

std::string Logger::getClassName()
{
    return className;
}

void Logger::setLoggerLevel(Level level)
{
    Logger::level = level;
}

void Logger::trace(const std::string s, ...)
{
    if (level >= Level::logTrace) {
        va_list arg;
        va_start(arg, s);
        log("TRACE", s, arg);
        va_end(arg);
    }
}

void Logger::debug(const std::string s, ...)
{
    if (level >= Level::logDebug) {
        va_list arg;
        va_start(arg, s);
        log("DEBUG", s, arg);
        va_end(arg);
    }
}

void Logger::warn(const std::string s, ...)
{
    if (level >= Level::logWarn) {
        va_list arg;
        va_start(arg, s);
        log("WARN", s, arg);
        va_end(arg);
    }
}

void Logger::info(const std::string s, ...)
{
    if (level >= Level::logInfo) {
        va_list arg;
        va_start(arg, s);
        log("INFO", s, arg);
        va_end(arg);
    }
}

void Logger::error(const std::string s, ...)
{
    if (level >= Level::logError) {
        va_list arg;
        va_start(arg, s);
        log("ERROR", s, arg);
        va_end(arg);
    }
}

}
}

