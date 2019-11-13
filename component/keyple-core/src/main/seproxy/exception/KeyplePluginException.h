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

#include "KeypleBaseException.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

/**
 * Base Exceptions thrown in a {@link org.eclipse.keyple.seproxy.ReaderPlugin}
 * context
 */
class KeyplePluginException : public KeypleBaseException {
public:
    /**
     * New plugin exception to be thrown
     *
     * @param message : message to identify the exception and the context
     */
    KeyplePluginException(const std::string &msg)
    : KeypleBaseException(msg)
    {

    }

    /**
     * Encapsulate a lower level plugin exception
     *
     * @param message : message to add some context to the exception
     * @param cause : lower level exception
     */
    KeyplePluginException(const std::string &msg, std::runtime_error cause)
    : KeypleBaseException(msg, cause)
    {

    }
};

}
}
}
}
