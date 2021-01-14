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
#include "AbstractPoCommandBuilder.h"
#include "KeypleCalypsoExport.h"
#include "PoClass.h"
#include "PoTransaction.h"

/* Forward declaration */
namespace keyple { namespace calypso { namespace command { namespace po {
    namespace parser { namespace storedvalue { class SvGetRespPars; } } } } } }

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace storedvalue {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::parser::storedvalue;
using namespace keyple::calypso::transaction;

using SvSettings = PoTransaction::SvSettings;
using Operation = PoTransaction::SvSettings::Operation;

/**
 * The Class SvGetCmdBuild. This class provides the dedicated constructor to build the SV Get
 * command.
 */
class KEYPLECALYPSO_API SvGetCmdBuild final : public AbstractPoCommandBuilder<SvGetRespPars> {
public:
    /**
     * Instantiates a new SvGetCmdBuild.
     *
     * @param poClass the PO class
     * @param poRevision the PO revision
     * @param svOperation the desired SV operation
     * @throw IllegalArgumentException - if the command is inconsistent
     */
    SvGetCmdBuild(const PoClass& poClass,
                  const PoRevision& poRevision,
                  const Operation svOperation);

    /**
     * Gets the request SV operation (used to check the SV command sequence)
     *
     * @return the current SvSettings.Operation enum value
     */
    Operation getSvOperation() const;

    /**
     *
     */
    std::shared_ptr<SvGetRespPars> createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse) override;

    /**
     *
     */
    bool isSessionBufferUsed() const override;

private:
    /**
     * The command
     */
    const CalypsoPoCommand& mCommand = CalypsoPoCommand::SV_GET;

    /**
     *
     */
    const Operation mSvOperation;

    /**
     *
     */
    std::vector<uint8_t> mHeader;
};

}
}
}
}
}
}
