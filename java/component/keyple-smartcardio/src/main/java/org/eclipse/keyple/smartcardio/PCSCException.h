/********************************************************************************
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * See the NOTICE file(s) distributed with this work for additional information regarding copyright
 * ownership.
 *
 * This program and the accompanying materials are made available under the terms of the Eclipse
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
 *
 * SPDX-License-Identifier: EPL-2.0
 ********************************************************************************/

#pragma once

#include "Exception.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace smartcardio {

/**
 * Base Exceptions thrown in a {@link org.eclipse.keyple.seproxy.ReaderPlugin}
 * context
 */
class PCSCException : public Exception {
public:
    /**
     * 
     */
    long code;

    /**
     * New plugin exception to be thrown
     * 
     * @param message : message to identify the exception and the context
     */

    PCSCException(const std::string &message) : Exception(message)
    {

    }

    /**
     * Encapsulate a lower level plugin exception
     * 
     * @param message : message to add some context to the exception
     * @param cause : lower level exception
     */
    PCSCException(const std::string &message, std::runtime_error cause)
    : Exception(message, cause)
    {

    }
};

}
}
}
}