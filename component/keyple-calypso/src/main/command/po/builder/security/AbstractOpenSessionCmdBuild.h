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

#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
#include <type_traits>

/* Calypso */
#include "AbstractPoCommandBuilder.h"
#include "CalypsoPoCommands.h"
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
     * @throws IllegalArgumentException - if the key index is 0 and rev is 2.4
     * @throws IllegalArgumentException - if the request is inconsistent
     */
    AbstractOpenSessionCmdBuild(PoRevision revision)
    : AbstractPoCommandBuilder<T>(
          CalypsoPoCommands ::getOpenSessionForRev(revision), nullptr)
    {
    }

    static std::shared_ptr<AbstractOpenSessionCmdBuild<T>>
    create(PoRevision revision, uint8_t debitKeyIndex,
           const std::vector<uint8_t>& sessionTerminalChallenge, uint8_t sfi,
           uint8_t recordNb, const std::string& extraInfo);

protected:
    std::shared_ptr<AbstractOpenSessionCmdBuild> shared_from_this()
    {
        return std::static_pointer_cast<AbstractOpenSessionCmdBuild>(
            AbstractPoCommandBuilder<T>::shared_from_this());
    }
};

}
}
}
}
}
}
