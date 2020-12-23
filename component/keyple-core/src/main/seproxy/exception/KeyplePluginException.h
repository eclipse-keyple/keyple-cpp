/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#pragma once

#include "KeypleException.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

/**
 * The exception KeyplePluginException is the parent abstract class of all Keyple plugins
 * exceptions.
 */
class KeyplePluginException : public KeypleException {
protected:
    /**
     * @param msg the message to identify the exception context
     */
    KeyplePluginException(const std::string& msg) : KeypleException(msg) {}

    /**
     * Encapsulates a lower level plugin exception
     *
     * @param msg message to identify the exception context
     * @param cause the cause
     */
    KeyplePluginException(const std::string& msg, const std::exception& cause)
    : KeypleException(msg, cause) {}
};

}
}
}
}
