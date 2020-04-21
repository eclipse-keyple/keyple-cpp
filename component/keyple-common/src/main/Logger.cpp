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

namespace keyple {
namespace common {

Logger::Level Logger::level = Logger::Level::logDebug;

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

void Logger::setLoggerLevel(Logger::Level level)
{
    Logger::level = level;
}

}
}

namespace std {
std::ostream& operator<<(std::ostream& os, const std::vector<uint8_t>& v)
{
    for (int i = 0; i < static_cast<int>(v.size()); i++)
        os << std::hex << setfill('0') << setw(2) << static_cast<int>(v[i]);

    return os;
}
}
