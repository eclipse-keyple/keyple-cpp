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

#include "DigestCloseCmdBuild.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;

DigestCloseCmdBuild::DigestCloseCmdBuild(SamRevision revision, char expectedResponseLength)
: AbstractSamCommandBuilder(CalypsoSamCommands::DIGEST_CLOSE, nullptr)
{
    this->defaultRevision = revision;

    if (expectedResponseLength != 0x04 && expectedResponseLength != 0x08) {
        throw std::invalid_argument(StringHelper::formatSimple("Bad digest length! Expected 4 or 8, got %s", expectedResponseLength));
    }

    char cla = this->defaultRevision.getClassByte();
    char p1 = 0x00;
    char p2 = static_cast<char>(0x00);

    // CalypsoRequest calypsoRequest = new CalypsoRequest(cla, command, p1, p2, null,
    // expectedResponseLength);
    std::vector<char> emptyVector;
    request = setApduRequest(cla, command, p1, p2, emptyVector, expectedResponseLength);
}

}
}
}
}
}
}
