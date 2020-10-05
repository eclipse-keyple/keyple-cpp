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

/* Core */
#include "ApduRequest.h"

/* Calypso */
#include "PoClass.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security {

using namespace keyple::core::seproxy::message;

/**
 * The RatificationCmdBuild class provides the ApduRequest dedicated to the
 * ratification command, i.e. the command sent after closing the secure session
 * to handle the ratification mechanism. <br> This particular builder is not
 * associated with any parser since the response to this command is always an
 * error and is never checked.
 */
class RatificationCmdBuild final {
public:
    /**
     * @param poClass the PO class
     * @return the ApduRequest ratification command according to the PO class
     *         provided
     */
    static std::shared_ptr<ApduRequest> getApduRequest(
        const PoClass& poClass);

private:
    /**
     *
     */
    RatificationCmdBuild() {}
};

}
}
}
}
}
}
