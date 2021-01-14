/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#pragma once

#include <cctype>
#include <exception>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

#include <iostream>
namespace keyple {
namespace common {

class StringHelper {
public:
    static std::string trimStart(std::string source, const std::string& trimChars = " \t\n\r\v\f")
    {
        return source.erase(0, source.find_first_not_of(trimChars));
    }

    static std::string trimEnd(std::string source, const std::string& trimChars = " \t\n\r\v\f")
    {
        return source.erase(source.find_last_not_of(trimChars) + 1);
    }

    static std::string trim(const std::string& source, const std::string& trimChars = " \t\n\r\v\f")
    {
        return trimStart(trimEnd(source, trimChars), trimChars);
    }

    static std::string replace(std::string source, const std::string& find,
                               const std::string& replace)
    {
        size_t pos = 0;
        while ((pos = source.find(find, pos)) != std::string::npos) {
            source.replace(pos, find.length(), replace);
            pos += replace.length();
        }
        return source;
    }

    static bool startsWith(const std::string &source, const std::string &value)
    {
        if (source.length() < value.length())
            return false;
        else
            return source.compare(0, value.length(), value) == 0;
    }

    static bool endsWith(const std::string &source, const std::string &value)
    {
        if (source.length() < value.length())
            return false;
        else
            return source.compare(source.length() - value.length(),
                                  value.length(),
                                  value) == 0;
    }

    static std::vector<std::string> split(const std::string &source,
                                          char delimiter)
    {
        std::vector<std::string> output;
        std::istringstream ss(source);
        std::string nextItem;

        while (std::getline(ss, nextItem, delimiter))
        {
            output.push_back(nextItem);
        }

        return output;
    }

    template<typename T>
    static std::string toString(const T &subject)
    {
        std::ostringstream ss;
        ss << subject;

        return ss.str();
    }

    static std::string uint8ToHexString(const uint8_t v)
    {
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(2)
           << static_cast<int>(v);

        return ss.str();
    }

    static std::string uint8ToHexString(const uint8_t *v, const int s)
    {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');

        for (int i = 0; i < s; i++)
            ss << std::hex << std::setw(2) << static_cast<int>(v[i]);

        return ss.str();
    }

    static std::string uint16ToHexString(const uint16_t v)
    {
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(4)
           << static_cast<int>(v);

        return ss.str();
    }

    static std::string uint24ToHexString(const uint32_t v)
    {
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(6)
           << static_cast<int>(v & 0xffffff);

        return ss.str();
    }

    static std::string uint32ToHexString(const uint32_t v)
    {
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(8)
           << static_cast<int>(v);

        return ss.str();
    }
};

}
}
