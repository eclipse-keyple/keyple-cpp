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
#include "KeypleSeCommandException.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace exception {

using namespace keyple::core::command::exception;

/**
 * The exception {@code CalypsoPoAccessForbiddenException} indicates that the
 * access is forbidden (e.g. Never access mode, DF is invalidated,...).
 */
class CalypsoPoCommandException : public KeypleSeCommandException {
public:
    /**
     * @param message the message to identify the exception context
     * @param command the Calypso PO command
     * @param statusCode the status code (option)
     */
    CalypsoPoCommandException(
      const std::string& message,
      const std::shared_ptr<CalypsoPoCommand> command,
      const std::shared_ptr<int> statusCode)
    : KeypleSeCommandException(message, command, statusCode) {}

};

}
}
}
}
}
