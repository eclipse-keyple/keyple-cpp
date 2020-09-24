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

#include "PoClass.h"

#include <iomanip>

/* Common */
#include "IllegalArgumentException.h"

namespace keyple {
namespace calypso {
namespace command {

using namespace keyple::common;

const PoClass PoClass::LEGACY("LEGACY", static_cast<char>(0x94));
const PoClass PoClass::ISO("ISO", static_cast<char>(0x00));

uint8_t PoClass::getValue() const
{
    return mCla;
}

PoClass::PoClass(const std::string& name, const uint8_t cla)
: mName(name), mCla(cla) {}

PoClass& PoClass::operator=(const PoClass& o)
{
    mCla = o.mCla;
    mName = o.mName;

    return *this;
}

bool PoClass::operator==(const PoClass& o) const
{
    return mCla == o.mCla &&
           mName == o.mName;
}

bool PoClass::operator!=(const PoClass& o) const
{
    return !(*this == o);
}

const PoClass& PoClass::valueOf(const std::string& name)
{
    if (name == LEGACY.mName)
        return LEGACY;
    else if (name == ISO.mName)
        return ISO;
    else
        throw IllegalArgumentException("PoClass " + name + "doesn't exist");
}

std::ostream& operator<<(std::ostream& os, const PoClass& pc)
{
    os << "POCLASS: {"
       << "NAME = "  << pc.mName << ", "
       << "CLASS = " << std::hex << std::setfill('0') << std::setw(2) << pc.mCla
       << "}";

    return os;
}

}
}
}
