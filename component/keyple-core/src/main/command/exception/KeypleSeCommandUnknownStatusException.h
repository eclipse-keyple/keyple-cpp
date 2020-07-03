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

/* Core */
#include "KeypleSeCommandException.h"

namespace keyple {
namespace core {
namespace command {
namespace exception {

/**
 * The exception {@code KeypleSeCommandUnknownStatusException} indicates that
 * the status code is not referenced.
 */
class KeypleSeCommandUnknownStatusException : public KeypleSeCommandException {
public:
    /**
     * @param message the message to identify the exception context
     * @param command the SE command
     * @param statusCode the status code
     */
    KeypleSeCommandUnknownStatusException(
      const std::string& message, const std::shared_ptr<SeCommand> command,
      const std::shared_ptr<Integer> statusCode)
    : KeypleSeCommandException(message, command, statusCode) {}
};

}
}
}
}
