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

#include <string>
#include <memory>

#include "KeyplePluginException.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

/**
 * The exception {@code KeyplePluginInstantiationException} indicates that the
 * plugin cannot be instantiated, possibly due to a problem with one of the
 * associated readers.
 */
class KeyplePluginInstantiationException : public KeyplePluginException {
public:
    /**
     * @param message the message to identify the exception context
     */
    KeyplePluginInstantiationException(const std::string& msg)
    : KeyplePluginException(msg)
    {
    }

    /**
     * Encapsulates a lower level plugin exception
     *
     * @param message message to identify the exception context
     * @param cause the cause
     */
    KeyplePluginInstantiationException(
      const std::string& msg, std::exception& cause)
    : KeyplePluginException(msg, cause)
    {
    }
};

}
}
}
}
