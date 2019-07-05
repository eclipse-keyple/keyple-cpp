/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
#include <type_traits>

#include "AbstractPoCommandBuilder.h"
#include "CalypsoPoCommands.h"
#include "PoRevision.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security{

using namespace org::eclipse::keyple::calypso::command::po;

using PoRevision                 = org::eclipse::keyple::calypso::command::po::PoRevision;
using AbstractApduResponseParser = org::eclipse::keyple::core::command::AbstractApduResponseParser;

/**
 * The Class AbstractOpenSessionCmdBuild. This class provides the dedicated constructor to build the
 * Open Secure Session APDU command.
 *
 */
template<typename T>
class AbstractOpenSessionCmdBuild : public AbstractPoCommandBuilder<T> {

    /**
     * Instantiates a new AbstractOpenSessionCmdBuild.
     *
     * @param revision the revision of the PO
     * @throws IllegalArgumentException - if the key index is 0 and rev is 2.4
     * @throws IllegalArgumentException - if the request is inconsistent
     */
public:
    AbstractOpenSessionCmdBuild(PoRevision revision)
    : AbstractPoCommandBuilder<T>(std::make_shared<CalypsoPoCommands>(
                                                CalypsoPoCommands::getOpenSessionForRev(revision)),
                                  nullptr)
    {
    }

    static std::shared_ptr<AbstractOpenSessionCmdBuild> create(PoRevision revision, char debitKeyIndex, std::vector<char> &sessionTerminalChallenge, char sfi, char recordNb, const std::string &extraInfo) {
        switch (revision) {
            case PoRevision::REV1_0:
                return std::make_shared<OpenSession10CmdBuild>(debitKeyIndex, sessionTerminalChallenge, sfi, recordNb, extraInfo);
            case PoRevision::REV2_4:
                return std::make_shared<OpenSession24CmdBuild>(debitKeyIndex, sessionTerminalChallenge, sfi, recordNb, extraInfo);
            case PoRevision::REV3_1:
            case PoRevision::REV3_1_CLAP:
                return std::make_shared<OpenSession31CmdBuild>(debitKeyIndex, sessionTerminalChallenge, sfi, recordNb, extraInfo);
            case PoRevision::REV3_2:
                return std::make_shared<OpenSession32CmdBuild>(debitKeyIndex, sessionTerminalChallenge, sfi, recordNb, extraInfo);
            default:
                throw std::invalid_argument("Revision " + revision + " isn't supported");
        }
    }

protected:
    std::shared_ptr<AbstractOpenSessionCmdBuild> shared_from_this() {
        return std::static_pointer_cast<AbstractOpenSessionCmdBuild>(AbstractPoCommandBuilder<T>::shared_from_this());
    }
};

}
}
}
}
}
}
}
}
