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

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

#include "AbstractOpenSessionCmdBuild.h"
#include "AbstractOpenSessionRespPars.h"

/* Forward declaration */
namespace keyple { namespace calypso { namespace command { namespace po {
    namespace parser { namespace security { class OpenSession24RespPars; } } } }
    } }

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security {

using namespace keyple::calypso::command::po::parser::security;
using namespace keyple::core::seproxy::message;

class KEYPLECALYPSO_API OpenSession24CmdBuild final
: public AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars> {
public:
    /**
     * Instantiates a new AbstractOpenSessionCmdBuild.
     *
     * @param keyIndex the key index
     * @param samChallenge the sam challenge returned by the SAM Get Challenge
     *        APDU command
     * @param sfi the sfi to select
     * @param recordNumber the record number to read
     *        empty)
     * @throw IllegalArgumentException - if key index is 0 (rev 2.4)
     * @throw IllegalArgumentException - if the request is inconsistent
     */
    OpenSession24CmdBuild(uint8_t keyIndex,
                          const std::vector<uint8_t>& samChallenge,
                          uint8_t sfi, uint8_t recordNumber);

    /**
     * Return type should be
     *   std::shared_ptr<OpenSession24RespPars>
     * ... but moved to
     *   std::shared_ptr<AbstractOpenSessionRespPars>
     * ... because of invalid covariant return type error
     */
    std::shared_ptr<AbstractOpenSessionRespPars> createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse) override;

    /**
     *
     * This command can't be executed in session and therefore doesn't uses the
     * session buffer.
     *
     * @return false
     */
    virtual bool isSessionBufferUsed() const override;

    /**
     *
     */
    virtual uint8_t getSfi() const override;

    /**
     *
     */
    virtual uint8_t getRecordNumber() const override;

private:
    /**
     * Construction arguments used for parsing
     */
    const uint8_t mSfi;
    const uint8_t mRecordNumber;
};

}
}
}
}
}
}
