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

#include "RatificationCmdBuild.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security {

std::shared_ptr<ApduRequest>
    RatificationCmdBuild::getApduRequest(const PoClass& poClass)
{
    const std::vector<uint8_t> ratificationApdu = {
        static_cast<uint8_t>(poClass.getValue()), 0xB2, 0x00, 0x00, 0x00};

    return std::make_shared<ApduRequest>(ratificationApdu, false);
}

}
}
}
}
}
}
