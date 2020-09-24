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

/* Calypso */
#include "AbstractPoCommandBuilder.h"
#include "CalypsoPoCommand.h"
#include "KeypleCalypsoExport.h"
#include "PoClass.h"

/* Forward declaration */
namespace keyple { namespace calypso { namespace command { namespace po {
    namespace parser { class GetDataTraceRespPars; } } } } }

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace keyple::calypso::command::po::parser;

/**
 * This class provides the dedicated constructor to build the Get data APDU
 * commands.
 *
 * This command can not be sent in session because it would generate a 6Cxx
 * status in contact mode and thus make calculation of the digest impossible.
 *
 */
class KEYPLECALYPSO_API GetDataTraceCmdBuild final :
public AbstractPoCommandBuilder<GetDataTraceRespPars> {
public:
    /**
     * Instantiates a new GetDataTraceCmdBuild.
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     */
     GetDataTraceCmdBuild(const PoClass poClass);

    /**
     *
     */
    std::unique_ptr<GetDataTraceRespPars> createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse) override;

    /**
     * This command doesn't modify the contents of the PO and therefore doesn't
     * uses the session buffer.
     *
     * @return false
     */
    bool isSessionBufferUsed() const override;

private:
    /**
     *
     */
    const CalypsoPoCommand& command = CalypsoPoCommand::GET_DATA_TRACE;
};

}
}
}
}
}
