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
#include "KeypleSeCommandException.h"

/* Calypso */
#include "CalypsoSamCommand.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace exception {

using namespace keyple::calypso::command::sam;
using namespace keyple::core::command::exception;

/**
 * The exception {@code CalypsoSamCommandException} is the parent abstract class
 * of all Keyple SAM APDU commands exceptions.
 */
class CalypsoSamCommandException : public KeypleSeCommandException {
protected:
    /**
     * @param message the message to identify the exception context
     * @param command the Calypso SAM command
     * @param statusCode the status code (optional)
     */
    CalypsoSamCommandException(const std::string& message,
                               const std::shared_ptr<CalypsoSamCommand> command,
                               const int statusCode)
    : KeypleSeCommandException(message, command, statusCode) {}
};

}
}
}
}
}
}
