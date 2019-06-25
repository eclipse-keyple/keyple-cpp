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

#include "AbstractPoCommandBuilder.h"
#include "PoModificationCommand.h"
#include "PoSendableInSession.h"
#include "CalypsoPoCommands.h"
#include "PoClass.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace calypso { namespace command { namespace po { namespace parser { class DecreaseRespPars; } } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace core { namespace seproxy { namespace message { class ApduResponse; } } } } } }


namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {


using PoClass = org::eclipse::keyple::calypso::command::PoClass;
using DecreaseRespPars = org::eclipse::keyple::calypso::command::po::parser::DecreaseRespPars;
using ApduResponse = org::eclipse::keyple::core::seproxy::message::ApduResponse;

/**
* The Class DecreaseCmdBuild. This class provides the dedicated constructor to build the Decrease
* APDU command.
*
*/
class DecreaseCmdBuild final : public AbstractPoCommandBuilder<DecreaseRespPars>, public PoSendableInSession, public PoModificationCommand {

/** The command. */
private:
const CalypsoPoCommands command = CalypsoPoCommands::DECREASE;

/**
 * Instantiates a new decrease cmd build from command parameters.
 *
 * @param poClass indicates which CLA byte should be used for the Apdu
 * @param sfi SFI of the file to select or 00h for current EF
 * @param counterNumber &gt;= 01h: Counters file, number of the counter. 00h: Simulated Counter
 *        file.
 * @param decValue Value to subtract to the counter (defined as a positive int &lt;= 16777215
 *        [FFFFFFh])
 * @param extraInfo extra information included in the logs (can be null or empty)
 * @throws IllegalArgumentException - if the decrement value is out of range
 * @throws IllegalArgumentException - if the command is inconsistent
 */

public:
DecreaseCmdBuild(PoClass poClass, char sfi, char counterNumber, int decValue, const std::string &extraInfo);
std::shared_ptr<DecreaseRespPars> createResponseParser(std::shared_ptr<ApduResponse> apduResponse) override;

protected:
std::shared_ptr<DecreaseCmdBuild> shared_from_this() {
    return std::static_pointer_cast<DecreaseCmdBuild>(AbstractPoCommandBuilder<DecreaseRespPars>::shared_from_this());
}
};

}
}
}
}
}
}
}
