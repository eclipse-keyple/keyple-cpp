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

/* Calypso */
#include "KeypleCalypsoExport.h"
#include "PoTransaction.h"

/* Forward declaration */
namespace keyple { namespace calypso { namespace command { namespace po {
    namespace parser { namespace security { class VerifyPinRespPars; } } } }
    } }

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security {

using namespace keyple::calypso::transaction;
using namespace keyple::calypso::command::po::parser::security;

using PinTransmissionMode = PoTransaction::PinTransmissionMode;

class KEYPLECALYPSO_API VerifyPinCmdBuild : public AbstractPoCommandBuilder<VerifyPinRespPars> {
public:
    /**
     * Verify the PIN
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     * @param pinTransmissionMode defines the way the PIN code is transmitted: in clear or encrypted
     *        form.
     * @param pin the PIN data. The PIN is always 4-byte long here, even in the case of a encrypted
     *        transmission (@see setCipheredPinData).
     */
    VerifyPinCmdBuild(const PoClass& poClass,
                      const PinTransmissionMode& pinTransmissionMode,
                      const std::vector<uint8_t>& pin);

    /**
     * Alternate builder dedicated to the reading of the wrong presentation counter
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     */
    explicit VerifyPinCmdBuild(const PoClass& poClass);

    /**
     *
     */
    std::shared_ptr<VerifyPinRespPars> createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse) override;

    /**
     *
     */
    virtual bool isSessionBufferUsed() const override;

    /**
     * Indicates if the command is used to read the attempt counter only
     *
     * @return true if the command is used to read the attempt counter
     */
    bool isReadCounterOnly() const;

private:
    /**
     *
     */
    const CalypsoPoCommand& mCommand = CalypsoPoCommand::VERIFY_PIN;

    /**
     *
     */
    const uint8_t mCla;

    /**
     *
     */
    const bool mReadCounterOnly;
};

}
}
}
}
}
}
