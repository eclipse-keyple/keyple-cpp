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

#include "KeypleException.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

/**
 * The exception {@code KeypleAllocationNoReaderException} indicates that no
 * reader were available during allocation invocation.
 */
class KeypleAllocationNoReaderException : public KeypleException {
public:
    KeypleAllocationNoReaderException(const std::string& message)
    : KeypleException(message) {}

    KeypleAllocationNoReaderException(const std::string&  message,
                                      const std::exception& cause)
    : KeypleException(message, cause) {}
};

}
}
}
}
