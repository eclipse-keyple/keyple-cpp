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

#include "ChangeKeyCmdBuild.h"
#include "ChangeKeyRespPars.h"
#include "ApduResponse.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::core::seproxy::message;

ChangeKeyCmdBuild::ChangeKeyCmdBuild(PoClass poClass, char keyIndex, std::vector<char> &cryptogram)
: AbstractPoCommandBuilder<ChangeKeyRespPars>(CalypsoPoCommands::CHANGE_KEY, nullptr) {

    if (cryptogram.empty() || (cryptogram.size() != 0x18 && cryptogram.size() != 0x20)) {
        throw std::invalid_argument("Bad cryptogram value.");
    }

    char cla = poClass.getValue();
    char p1 = static_cast<char>(0x00);
    char p2 = keyIndex;

    this->request = setApduRequest(cla, command, p1, p2, cryptogram, -1);
    this->addSubName("Change Key");
}

std::shared_ptr<ChangeKeyRespPars> ChangeKeyCmdBuild::createResponseParser(std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<ChangeKeyRespPars>(apduResponse);
}

}
}
}
}
}
}
