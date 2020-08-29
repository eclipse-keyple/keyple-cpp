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

#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
#include <type_traits>

/* Calypso */
#include "AbstractOpenSessionRespPars.h"
#include "AbstractPoCommandBuilder.h"
#include "AbstractPoResponseParser.h"
#include "CalypsoPoCommand.h"
#include "KeypleCalypsoExport.h"
#include "PoRevision.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security {

using namespace keyple::calypso::command::po;
using namespace keyple::core::command;

/**
 * The Class AbstractOpenSessionCmdBuild. This class provides the dedicated
 * constructor to build the Open Secure Session APDU command.
 *
 */
template <typename T>
class KEYPLECALYPSO_API AbstractOpenSessionCmdBuild
: public AbstractPoCommandBuilder<T> {
public:
    /**
     * Instantiates a new AbstractOpenSessionCmdBuild.
     *
     * @param revision the revision of the PO
     * @throw IllegalArgumentException - if the key index is 0 and rev is 2.4
     * @throw IllegalArgumentException - if the request is inconsistent
     */
    AbstractOpenSessionCmdBuild(PoRevision revision);
    : AbstractPoCommandBuilder<T>(
          CalypsoPoCommand::getOpenSessionForRev(revision), nullptr) {}

    template <typename T>
    static  std::shared_ptr<AbstractOpenSessionCmdBuild<T>> create(
        PoRevision revision, uint8_t debitKeyIndex,
        const std::vector<uint8_t>& sessionTerminalChallenge, uint8_t sfi,
        uint8_t recordNumber)
    {
        switch (revision) {
        case PoRevision::REV1_0: {
            std::shared_ptr<OpenSession10CmdBuild> cmd =
                std::make_shared<OpenSession10CmdBuild>(
                    debitKeyIndex, sessionTerminalChallenge, sfi, recordNumber);
            return std::shared_ptr<AbstractOpenSessionCmdBuild<T>>(
                cmd, reinterpret_cast<AbstractOpenSessionCmdBuild<T>*>(
                    cmd.get()));
        }
        case PoRevision::REV2_4: {
            std::shared_ptr<OpenSession24CmdBuild> cmd =
                std::make_shared<OpenSession24CmdBuild>(
                    debitKeyIndex, sessionTerminalChallenge, sfi, recordNumber);
            return std::shared_ptr<AbstractOpenSessionCmdBuild<T>>(
                cmd, reinterpret_cast<AbstractOpenSessionCmdBuild<T>*>(
                    cmd.get()));
        }
        case PoRevision::REV3_1:
        case PoRevision::REV3_1_CLAP: {
            std::shared_ptr<OpenSession31CmdBuild> cmd =
                std::make_shared<OpenSession31CmdBuild>(
                    debitKeyIndex, sessionTerminalChallenge, sfi, recordNumber);
            return std::shared_ptr<AbstractOpenSessionCmdBuild<T>>(
                cmd, reinterpret_cast<AbstractOpenSessionCmdBuild<T>*>(
                    cmd.get()));
        }
        case PoRevision::REV3_2: {
            std::shared_ptr<OpenSession32CmdBuild> cmd =
                std::make_shared<OpenSession32CmdBuild>(
                    debitKeyIndex, sessionTerminalChallenge, sfi, recordNumber);
            return std::shared_ptr<AbstractOpenSessionCmdBuild<T>>(
                cmd, reinterpret_cast<AbstractOpenSessionCmdBuild<T>*>(
                    cmd.get()));
        }
        default:
            throw std::invalid_argument(StringHelper::formatSimple(
                "Revision %d isn't supported", static_cast<int>(revision)));
        }
    }

    /**
     * @return the SFI of the file read while opening the secure session
     */
    virtual const uint8_t getSfi() const = 0;

    /**
     * @return the record number to read
     */
    virtual const uint8_t getRecordNumber() const  = 0;
};

}
}
}
}
}
}
