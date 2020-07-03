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

/* Common */
#include "RuntimeException.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

using namespace keyple::common;

/**
 * Base Exception for all Keyple Checked Exception
 */
class KeypleException : public RuntimeException {
public:
    /**
     *
     */
    KeypleException(const std::string& msg) : RuntimeException(msg) {}

    /**
     *
     */
    KeypleException(const std::string& msg, const std::exception& cause)
    : RuntimeException(msg, cause) {}
};

}
}
}
}
