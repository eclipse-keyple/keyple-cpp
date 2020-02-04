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

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

AnswerToReset::AnswerToReset(const std::vector<uint8_t>& atrBytes)
{
    this->atrBytes.assign(atrBytes.begin(), atrBytes.end());
}

const std::vector<uint8_t>& AnswerToReset::getBytes()
{
    return atrBytes;
}

bool AnswerToReset::equals(std::shared_ptr<void> o)
{
    if (o == shared_from_this()) {
        return true;
    }

    if (!(std::static_pointer_cast<AnswerToReset>(o) != nullptr)) {
        return false;
    }

    std::shared_ptr<AnswerToReset> atr =
        std::static_pointer_cast<AnswerToReset>(o);

    return Arrays::equals(atr->getBytes(), this->atrBytes);
}

int AnswerToReset::hashCode()
{
    int hash = 17;
    hash = 19 * hash + (atrBytes.empty() ? 0 : Arrays::hashCode(atrBytes));
    return hash;
}

void AnswerToReset::finalize()
{
}

}
}
}
}
