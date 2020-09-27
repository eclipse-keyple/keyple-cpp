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

#include "ChangeKeyCmdBuild.h"

/* Common */
#include "IllegalArgumentException.h"

/* Core */
#include "ApduResponse.h"

/* Calypso */
#include "CalypsoPoCommand.h"
#include "ChangeKeyRespPars.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::common;
using namespace keyple::core::seproxy::message;

ChangeKeyCmdBuild::ChangeKeyCmdBuild(
  const PoClass poClass,
  const uint8_t keyIndex,
  const std::vector<uint8_t>& cryptogram)
: AbstractPoCommandBuilder<ChangeKeyRespPars>(
      std::make_shared<CalypsoPoCommand>(CalypsoPoCommand::CHANGE_KEY),
      nullptr)
{

    if (cryptogram.empty() ||
        (cryptogram.size() != 0x18 && cryptogram.size() != 0x20)) {
        throw IllegalArgumentException("Bad cryptogram value.");
    }

    mRequest = setApduRequest(poClass.getValue(),
                              command,
                              0x00,
                              keyIndex,
                              cryptogram);

    addSubName("Change Key");
}

std::shared_ptr<ChangeKeyRespPars> ChangeKeyCmdBuild::createResponseParser(
    std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<ChangeKeyRespPars>(apduResponse, this);
}

bool ChangeKeyCmdBuild::isSessionBufferUsed() const
{
    return false;
}

}
}
}
}
}
}
