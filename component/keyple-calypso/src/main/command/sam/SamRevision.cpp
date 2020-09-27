/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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

#include "SamRevision.h"

/* Common */
#include "IllegalArgumentException.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {

using namespace keyple::common;

const SamRevision SamRevision::C1("C1", "C1", 0x80);
const SamRevision SamRevision::S1E("S1E", "E1", 0x80);
const SamRevision SamRevision::S1D("S1D", "D?", 0x94);
const SamRevision SamRevision::AUTO("AUTO", "??", 0x00);
const SamRevision SamRevision::NO_REV("NO_REV", "??", 0x00);

SamRevision::SamRevision(
  const std::string& name,
  const std::string& applicationTypeMask,
  const uint8_t classByte)
: mName(name), mApplicationTypeMask(applicationTypeMask), mClassByte(classByte)
{}

SamRevision::SamRevision(const SamRevision& o)
: mName(o.mName),
 mApplicationTypeMask(o.mApplicationTypeMask),
 mClassByte(o.mClassByte) {}

const std::string& SamRevision::getName() const
{
    return mName;
}

const std::string& SamRevision::getApplicationTypeMask() const
{
    return mApplicationTypeMask;
}

uint8_t SamRevision::getClassByte() const
{
    return mClassByte;
}

bool SamRevision::operator==(const SamRevision& o) const
{
    return this->mName == o.mName &&
           this->mApplicationTypeMask == o.mApplicationTypeMask &&
           this->mClassByte == o.mClassByte;
}

bool SamRevision::operator!=(const SamRevision& o) const
{
    return !(*this == o);
}

SamRevision& SamRevision::operator=(const SamRevision& o)
{
    this->mName = o.mName;
    this->mApplicationTypeMask = o.mApplicationTypeMask;
    this->mClassByte = o.mClassByte;

    return *this;
}

const SamRevision& SamRevision::valueOf(const std::string& name)
{
    if (name == C1.mName)
        return C1;
    else if (name == S1E.mName)
        return S1E;
    else if (name == S1D.mName)
        return S1D;
    else if (name == AUTO.mName)
        return AUTO;
    else
        /* C++ vs. Java: throw for an invalid name */
        throw IllegalArgumentException(
                  "Name " + name + " is not valid a Sam Rev");
}

std::ostream& operator<<(std::ostream& os, const SamRevision& sr)
{
    if (sr == SamRevision::NO_REV)
        os << "NO_REV";
    else if (sr == SamRevision::C1)
        os << "C1";
    else if (sr == SamRevision::S1E)
        os << "S1E";
	else if (sr == SamRevision::S1D)
        os << "S1D";
	else if (sr == SamRevision::AUTO)
        os << "AUTO";
    return os;
}

}
}
}
}
