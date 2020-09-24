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

#include <memory>

/* Common */
#include "Integer.h"

/* Core */
#include "SeCommand.h"
#include "KeypleException.h"

namespace keyple {
namespace core {
namespace command {
namespace exception {

using namespace keyple::core::command;
using namespace keyple::core::seproxy::exception;

/**
 * The exception {@code KeypleSeCommandException} is the parent abstract class
 * of all Keyple SE APDU commands exceptions.
 */
class KeypleSeCommandException : public KeypleException {
public:
    /**
     * @return the command
     */
    const std::shared_ptr<SeCommand> getCommand() const
    {
        return mCommand;
    }

    /**
     * @return the status code (nullable)
     */
    int getStatusCode() const
    {
        return mStatusCode;
    }

protected:
    /**
     * @param message the message to identify the exception context
     * @param command the command
     * @param statusCode the status code (optional)
     */
    KeypleSeCommandException(const std::string& message,
                             const std::shared_ptr<SeCommand> command,
                             const int statusCode)
    : KeypleException(message), mCommand(command), mStatusCode(statusCode) {}

private:
    /**
     * The command
     */
    const std::shared_ptr<SeCommand> mCommand;

    /**
     * The status code (optional)
     */
    const int mStatusCode;
};

}
}
}
}
