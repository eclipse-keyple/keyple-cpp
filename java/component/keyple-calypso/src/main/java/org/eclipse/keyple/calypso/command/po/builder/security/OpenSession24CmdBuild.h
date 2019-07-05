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

#include "AbstractOpenSessionCmdBuild.h"
#include "OpenSession24RespPars.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security {

using OpenSession24RespPars = org::eclipse::keyple::calypso::command::po::parser::security::OpenSession24RespPars;
using ApduResponse          = org::eclipse::keyple::core::seproxy::message::ApduResponse;

class OpenSession24CmdBuild final : public AbstractOpenSessionCmdBuild<OpenSession24RespPars> {
    /**
     * Instantiates a new AbstractOpenSessionCmdBuild.
     *
     * @param keyIndex the key index
     * @param samChallenge the sam challenge returned by the SAM Get Challenge APDU command
     * @param sfiToSelect the sfi to select
     * @param recordNumberToRead the record number to read
     * @param extraInfo extra information included in the logs (can be null or empty)
     * @throws IllegalArgumentException - if key index is 0 (rev 2.4)
     * @throws IllegalArgumentException - if the request is inconsistent
     */
public:
    OpenSession24CmdBuild(char keyIndex, std::vector<char> &samChallenge, char sfiToSelect, char recordNumberToRead, const std::string &extraInfo);
	std::shared_ptr<OpenSession24RespPars> createResponseParser(std::shared_ptr<ApduResponse> apduResponse) override;

protected:
    std::shared_ptr<OpenSession24CmdBuild> shared_from_this() {
        return std::static_pointer_cast<OpenSession24CmdBuild>(AbstractOpenSessionCmdBuild<OpenSession24RespPars>::shared_from_this());
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
