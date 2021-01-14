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

#include "CardCipherPinCmdBuild.h"

/* Common */
#include "IllegalArgumentException.h"
#include "System.h"

/* Calypso */
#include "CardCipherPinRespPars.h"
#include "CalypsoSamCommand.h"
#include "KeyReference.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso;
using namespace keyple::calypso::command::sam;
using namespace keyple::common;
using namespace keyple::common::exception;

CardCipherPinCmdBuild::CardCipherPinCmdBuild(const SamRevision& revision,
                                             const std::shared_ptr<KeyReference> cipheringKey,
                                             const std::vector<uint8_t>& currentPin,
                                             const std::vector<uint8_t>& newPin)
: AbstractSamCommandBuilder(
      std::make_shared<CalypsoSamCommand>(CalypsoSamCommand::CARD_CIPHER_PIN), nullptr)
{
    if (revision != SamRevision::NO_REV)
        mDefaultRevision = revision;

    if (currentPin.empty() || currentPin.size() != 4)
        throw IllegalArgumentException("Bad current PIN value.");

    if (!newPin.empty() && newPin.size() != 4)
        throw IllegalArgumentException("Bad new PIN value.");

    const uint8_t cla = mDefaultRevision.getClassByte();

    uint8_t p1;
    uint8_t p2;
    std::vector<uint8_t> data;

    if (newPin.empty()) {
        /* No new PIN is provided, we consider it's a PIN verification */
        p1 = 0x80;
        data = std::vector<uint8_t>(6);
    } else {
        /* A new PIN is provided, we consider it's a PIN update */
        p1 = 0x40;
        data = std::vector<uint8_t>(10);
        System::arraycopy(newPin, 0, data, 6, 4);
    }
    p2 = 0xFF; /* KIF and KVC in incoming data */

    data[0] = cipheringKey->getKif();
    data[1] = cipheringKey->getKvc();

    System::arraycopy(currentPin, 0, data, 2, 4);

    mRequest = setApduRequest(cla, mCommand, p1, p2, data);
}

std::shared_ptr<CardCipherPinRespPars> CardCipherPinCmdBuild::createResponseParser(
    std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<CardCipherPinRespPars>(apduResponse, this);
}

}
}
}
}
}
}
