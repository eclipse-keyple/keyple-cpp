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

#include <vector>
#include <stdexcept>
#include <memory>

/* Calypso */
#include "AbstractSamCommandBuilder.h"
#include "CalypsoSamCommand.h"
#include "KeyReference.h"
#include "SamRevision.h"

/* Forward declaration */
namespace keyple { namespace calypso { namespace command { namespace sam {
    namespace parser { namespace security { class CardCipherPinRespPars; } } }
    } } }

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso;
using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::parser::security;

/**
 * Builder for the SAM Card Cipher PIN APDU command.
 */
class CardCipherPinCmdBuild
: public AbstractSamCommandBuilder<CardCipherPinRespPars> {
public:
    /**
     * Instantiates a new CardCipherPinCmdBuild and generate the ciphered data for a Verify PIN or
     * Change PIN PO command.
     * <p>
     * In the case of a PIN verification, only the current PIN must be provided (newPin must be set
     * to null).
     * <p>
     * In the case of a PIN update, the current and new PINs must be provided.
     *
     * @param revision of the SAM
     * @param cipheringKey the key used to encipher the PIN data
     * @param currentPin the current PIN (a 4-byte byte array)
     * @param newPin the new PIN (a 4-byte byte array if the operation in progress is a PIN update,
     *        null if the operation in progress is a PIN verification)
     */
    CardCipherPinCmdBuild(const SamRevision& revision,
                          const std::shared_ptr<KeyReference> cipheringKey,
                          const std::vector<uint8_t>& currentPin,
                          const std::vector<uint8_t>& newPin);

    /**
     *
     */
    std::shared_ptr<CardCipherPinRespPars> createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse) override;

private:
    /**
     * The command reference
     */
    const CalypsoSamCommand& mCommand = CalypsoSamCommand::CARD_CIPHER_PIN;
};

}
}
}
}
}
}
