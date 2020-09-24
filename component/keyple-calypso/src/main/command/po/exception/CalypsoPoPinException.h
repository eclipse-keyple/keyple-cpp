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

#include "CalypsoPoCommand.h"
#include "CalypsoPoCommandException.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace exception {

/**
 * The exception {@code CalypsoPoPinException} indicates that the input PIN
 * provided is not valid.<br>
 * This can occur during the PIN verification.
 */
class CalypsoPoPinException final : public CalypsoPoCommandException {
public:
    /**
     * @param message the message to identify the exception context
     * @param command the Calypso PO command
     * @param statusCode the status code
     */
    CalypsoPoPinException(
      const std::string& message,
      const std::shared_ptr<CalypsoPoCommand> command,
      const int statusCode)
    : CalypsoPoCommandException(message, command, statusCode) {}
};

}
}
}
}
}
