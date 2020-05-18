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

#include <string>
#include <memory>

#include "KeypleBaseException.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

/**
 * The exception {@code CalypsoNoSamResourceAvailableException} indicates that
 * there are no SAM resources available.
 */
class CalypsoNoSamResourceAvailableException : public KeypleBaseException {
public:
    /**
     * @param message the message to identify the exception context
     */
    CalypsoNoSamResourceAvailableException(const std::string& message)
    : KeypleBaseException(message)
    {
    }

    /**
     * @param message the message to identify the exception context
     * @param t the cause
     */
    CalypsoNoSamResourceAvailableException(const std::string& message,
                                          const std::exception& cause)
    : KeypleBaseException(message, cause)
    {
    }
};

}
}
}
}
