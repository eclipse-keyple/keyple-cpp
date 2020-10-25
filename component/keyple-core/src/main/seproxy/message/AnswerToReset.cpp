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

#include "Arrays.h"
#include "AnswerToReset.h"

/* Common */
#include "KeypleStd.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

AnswerToReset::AnswerToReset(const std::vector<uint8_t>& atrBytes)
: atrBytes(atrBytes)
{
}

const std::vector<uint8_t>& AnswerToReset::getBytes() const
{
    return atrBytes;
}

bool AnswerToReset::operator==(const AnswerToReset& o) const
{
    return Arrays::equals(o.getBytes(), this->atrBytes);
}

bool AnswerToReset::operator!=(const AnswerToReset& o) const
{
    return !(*this == o);
}

std::ostream& operator<<(std::ostream& os, const AnswerToReset& atr)
{
    os << "ATR = " << atr.atrBytes;

	return os;
}

std::ostream& operator<<(std::ostream& os,
                         const std::shared_ptr<AnswerToReset>& atr)
{
    if (atr)
        os << *(atr.get());
    else
		os << "ATR = null";

	return os;
}

}
}
}
}
