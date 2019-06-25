
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

#include <vector>
#include <stdexcept>
#include <memory>

#include "AbstractPoCommandBuilder.h"
#include "CalypsoPoCommands.h"
#include "PoClass.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace calypso { namespace command { namespace po { namespace parser { namespace security { class CloseSessionRespPars; } } } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace core { namespace seproxy { namespace message { class ApduResponse; } } } } } }


namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security{


using PoClass = org::eclipse::keyple::calypso::command::PoClass;
using AbstractPoCommandBuilder = org::eclipse::keyple::calypso::command::po::AbstractPoCommandBuilder;
using CalypsoPoCommands = org::eclipse::keyple::calypso::command::po::CalypsoPoCommands;
using CloseSessionRespPars = org::eclipse::keyple::calypso::command::po::parser::security::CloseSessionRespPars;
using ApduResponse = org::eclipse::keyple::core::seproxy::message::ApduResponse;

// TODO: Auto-generated Javadoc
/**
 * This class provides the dedicated constructor to build the Close Secure Session APDU command.
 */
class CloseSessionCmdBuild final : public AbstractPoCommandBuilder<CloseSessionRespPars> {

    /** The command. */
private:
    const CalypsoPoCommands command = CalypsoPoCommands::CLOSE_SESSION;

    /**
     * Instantiates a new CloseSessionCmdBuild depending of the revision of the PO.
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     * @param ratificationAsked the ratification asked
     * @param terminalSessionSignature the sam half session signature
     * @throws IllegalArgumentException - if the signature is null or has a wrong length
     * @throws IllegalArgumentException - if the command is inconsistent
     */
public:
    CloseSessionCmdBuild(PoClass poClass, bool ratificationAsked, std::vector<char> &terminalSessionSignature);

    /**
     * Instantiates a new CloseSessionCmdBuild based on the revision of the PO to generate an abort
     * session command (Close Secure Session with p1 = p2 = lc = 0).
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     */
    CloseSessionCmdBuild(PoClass poClass);

    std::shared_ptr<CloseSessionRespPars> createResponseParser(std::shared_ptr<ApduResponse> apduResponse) override;

protected:
    std::shared_ptr<CloseSessionCmdBuild> shared_from_this() {
        return std::static_pointer_cast<CloseSessionCmdBuild>(AbstractPoCommandBuilder<CloseSessionRespPars>::shared_from_this());
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
