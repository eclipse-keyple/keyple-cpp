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

#include "GiveRandomCmdBuild.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;

GiveRandomCmdBuild::GiveRandomCmdBuild(SamRevision revision, std::vector<char> &random)
: AbstractSamCommandBuilder(CalypsoSamCommands::GIVE_RANDOM, nullptr)
{
    //if (revision != nullptr) {
        this->defaultRevision = revision;
    //}
    char cla = this->defaultRevision.getClassByte();
    char p1 = static_cast<char>(0x00);
    char p2 = static_cast<char>(0x00);

    if (random.size() > 0 && random.size() != 8) {
        throw std::invalid_argument("Random value should be an 8 bytes long");
    }

    request = setApduRequest(cla, command, p1, p2, random, -1);
}

}
}
}
}
}
}
