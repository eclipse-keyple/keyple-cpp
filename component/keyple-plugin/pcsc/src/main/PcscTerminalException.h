/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
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

#include "Exception.h"

namespace keyple {
namespace plugin {
namespace pcsc {

using namespace keyple::common;

class PcscTerminalException : public Exception {
public:
    /**
     *
     */
    PcscTerminalException(const std::string &msg)
    : Exception(msg)
    {

    }

    /**
     *
     */
    PcscTerminalException(const std::string &msg, std::exception& cause)
    : Exception(msg, cause)
    {

    }
};

}
}
}
