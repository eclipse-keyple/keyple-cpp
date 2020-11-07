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

#include "CardGenerateKeyCmdBuild.h"

/* Common */
#include "IllegalArgumentException.h"

/* Calypso */
#include "CardGenerateKeyRespPars.h"
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
using namespace keyple::calypso::command::sam::builder::security;
using namespace keyple::common::exception;

CardGenerateKeyCmdBuild::CardGenerateKeyCmdBuild(
  const SamRevision& revision,
  const std::shared_ptr<KeyReference> cipheringKey,
  const std::shared_ptr<KeyReference> sourceKey)
: AbstractSamCommandBuilder(
      std::make_shared<CalypsoSamCommand>(CalypsoSamCommand::CARD_GENERATE_KEY),
      nullptr)
{
    if (revision != SamRevision::NO_REV)
        mDefaultRevision = revision;

    if (sourceKey == nullptr)
        throw IllegalArgumentException("The source key reference can't be null.");

    const uint8_t cla = mDefaultRevision.getClassByte();

    uint8_t p1, p2;
    std::vector<uint8_t> data;

    if (cipheringKey == nullptr) {
        /* Case where the source key is ciphered by the null key */
        p1 = 0xFF;
        p2 = 0x00;

        data    = std::vector<uint8_t>(3);
        data[0] = sourceKey->getKif();
        data[1] = sourceKey->getKvc();
        data[2] = 0x90;
    } else {
        p1 = 0xFF;
        p2 = 0xFF;

        data    = std::vector<uint8_t>(5);
        data[0] = cipheringKey->getKif();
        data[1] = cipheringKey->getKvc();
        data[2] = sourceKey->getKif();
        data[3] = sourceKey->getKvc();
        data[4] = 0x90;
    }

    mRequest = setApduRequest(cla, mCommand, p1, p2, data);
}

std::shared_ptr<CardGenerateKeyRespPars> CardGenerateKeyCmdBuild::createResponseParser(
    std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<CardGenerateKeyRespPars>(apduResponse, this);
}

}
}
}
}
}
}
