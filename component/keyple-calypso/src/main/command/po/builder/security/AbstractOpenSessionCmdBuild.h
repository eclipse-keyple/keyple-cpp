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
#include <type_traits>

/* Calypso */
#include "AbstractPoCommandBuilder.h"
#include "AbstractPoResponseParser.h"
#include "CalypsoPoCommand.h"
#include "KeypleCalypsoExport.h"
#include "PoRevision.h"

/* Common */
#include "stringhelper.h"

/* Forward declaration */
namespace keyple { namespace calypso { namespace command { namespace po {
    namespace builder { namespace security { class OpenSession10CmdBuild; } } }
    } } }

namespace keyple { namespace calypso { namespace command { namespace po {
    namespace builder { namespace security { class OpenSession24CmdBuild; } } }
    } } }

namespace keyple { namespace calypso { namespace command { namespace po {
    namespace builder { namespace security { class OpenSession31CmdBuild; } } }
    } } }

namespace keyple { namespace calypso { namespace command { namespace po {
    namespace builder { namespace security { class OpenSession32CmdBuild; } } }
    } } }

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security {

using namespace keyple::calypso::command::po;
using namespace keyple::common;
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
    AbstractOpenSessionCmdBuild(PoRevision revision)
    : AbstractPoCommandBuilder<T>(
          std::make_shared<CalypsoPoCommand>(
              CalypsoPoCommand::getOpenSessionForRev(revision)),
          nullptr) {}

    static  std::shared_ptr<AbstractOpenSessionCmdBuild<T>> create(
        const PoRevision revision,
        const uint8_t debitKeyIndex,
        const std::vector<uint8_t>& sessionTerminalChallenge,
        const uint8_t sfi,
        const uint8_t recordNumber)
    {
        switch (revision) {
        case PoRevision::REV1_0:
            return std::make_shared<OpenSession10CmdBuild>(
                       debitKeyIndex,
                       sessionTerminalChallenge,
                       sfi,
                       recordNumber);
        case PoRevision::REV2_4:
            return std::make_shared<OpenSession24CmdBuild>(
                       debitKeyIndex,
                       sessionTerminalChallenge,
                       sfi,
                       recordNumber);
        case PoRevision::REV3_1:
        case PoRevision::REV3_1_CLAP:
            return std::make_shared<OpenSession31CmdBuild>(
                       debitKeyIndex,
                       sessionTerminalChallenge,
                       sfi,
                       recordNumber);
        case PoRevision::REV3_2:
            return std::make_shared<OpenSession32CmdBuild>(
                       debitKeyIndex,
                       sessionTerminalChallenge,
                       sfi,
                       recordNumber);
        default:
            throw std::invalid_argument("Revision " +
                                        StringHelper::toString(revision) +
                                        " isn't supported");
        }
    }

    /**
     * @return the SFI of the file read while opening the secure session
     */
    virtual uint8_t getSfi() const = 0;

    /**
     * @return the record number to read
     */
    virtual uint8_t getRecordNumber() const  = 0;
};

}
}
}
}
}
}
