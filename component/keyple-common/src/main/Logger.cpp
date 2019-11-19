/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#include "Logger.h"
#include "LoggerFactory.h"

namespace keyple {
namespace common {

class LoggerFactory;

Logger::Logger(const std::string &className, std::mutex* mtx)
: className(demangle(className.c_str()))
{
    traceEnabled = 1;
    debugEnabled = 1;
    warnEnabled  = 1;
    infoEnabled  = 1;
    errorEnabled = 1;

    this->mtx = mtx;
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

void Logger::trace(const std::string s, ...)
{
    if (traceEnabled) {
        va_list arg;
        va_start (arg, s);
        log("TRACE", s, arg);
        va_end(arg);
    }
}

void Logger::debug(const std::string s, ...)
{
    if (debugEnabled) {
        va_list arg;
        va_start (arg, s);
        log("DEBUG", s, arg);
        va_end(arg);
    }
}

void Logger::warn(const std::string s, ...)
{
    if (warnEnabled) {
        va_list arg;
        va_start (arg, s);
        log("WARN", s, arg);
        va_end(arg);
    }
}

void Logger::info(const std::string s, ...)
{
    if (infoEnabled) {
        va_list arg;
        va_start (arg, s);
        log("INFO", s, arg);
        va_end(arg);
    }
}

void Logger::error(const std::string s, ...)
{
    if (errorEnabled) {
        va_list arg;
        va_start (arg, s);
        log("ERROR", s, arg);
        va_end(arg);
    }
}
}
}
