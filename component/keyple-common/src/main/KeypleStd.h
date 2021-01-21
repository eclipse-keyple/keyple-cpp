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

#include <memory>
#include <iomanip>
#include <iostream>
#include <map>
#include <ostream>
#include <set>
#include <vector>

namespace std {

#if !defined(_WIN32)
template <typename To, typename From>
inline std::shared_ptr<To> reinterpret_pointer_cast(std::shared_ptr<From> const & ptr) noexcept
{
    return std::shared_ptr<To>(ptr, reinterpret_cast<To *>(ptr.get()));
}
#endif

inline std::ostream& operator<<(std::ostream& os, const uint8_t v)
{
    os << static_cast<int>(v)
       << "(0x" << std::hex << std::setfill('0') << std::setw(2)
       << static_cast<int>(v) << ")";

    return os;
}

inline std::ostream& operator<<(std::ostream& os, const std::vector<uint8_t>& v)
{
    for (int i = 0; i < static_cast<int>(v.size()); i++)
        os << std::hex << std::setfill('0') << std::setw(2)
           << static_cast<int>(v[i]);

    return os;
}

inline std::ostream& operator<<(std::ostream& os, const std::set<std::string>& s)
{
    os << "{";
    for (auto it = s.begin(); it != s.end(); ++it)
    {
        if (it != s.begin())
            os << ", ";
        os << *it;
    }
    os << "}";

    return os;
}

inline
std::ostream& operator<<(std::ostream& os, const std::map<const std::string, const std::string>& s)
{
    os << "MAP: {";
    for (auto it = s.begin(); it != s.end(); ++it)
    {
        if (it != s.begin())
            os << ", ";
        os << "{" << it->first << ", " << it->second << "}";
    }
    os << "}";

    return os;
}

}
