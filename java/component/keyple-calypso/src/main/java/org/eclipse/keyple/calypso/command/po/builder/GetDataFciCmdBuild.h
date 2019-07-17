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

#include <memory>

/* Calypso */
#include "AbstractPoCommandBuilder.h"
#include "CalypsoPoCommands.h"
#include "GetDataFciRespPars.h"
#include "PoClass.h"

/* Common */
#include "Export.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using PoClass                  = org::eclipse::keyple::calypso::command::PoClass;
using CalypsoPoCommands        = org::eclipse::keyple::calypso::command::po::CalypsoPoCommands;
using GetDataFciRespPars       = org::eclipse::keyple::calypso::command::po::parser::GetDataFciRespPars;
using ApduResponse             = org::eclipse::keyple::core::seproxy::message::ApduResponse;

/**
* This class provides the dedicated constructor to build the Get data APDU commands.
*
* This command can not be sent in session because it would generate a 6Cxx status in contact mode
* and thus make calculation of the digest impossible.
*
*/
class EXPORT GetDataFciCmdBuild final
: public org::eclipse::keyple::calypso::command::po::AbstractPoCommandBuilder<GetDataFciRespPars> {

private:
    static const std::shared_ptr<CalypsoPoCommands> command;

    /**
     * Instantiates a new GetDataFciCmdBuild.
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     */
public:
    GetDataFciCmdBuild(PoClass poClass);

    std::shared_ptr<GetDataFciRespPars> createResponseParser(std::shared_ptr<ApduResponse> apduResponse) override;

protected:
    std::shared_ptr<GetDataFciCmdBuild> shared_from_this() {
         return std::static_pointer_cast<GetDataFciCmdBuild>(AbstractPoCommandBuilder<GetDataFciRespPars>::shared_from_this());
    }
};

}
}
}
}
}
}
}
