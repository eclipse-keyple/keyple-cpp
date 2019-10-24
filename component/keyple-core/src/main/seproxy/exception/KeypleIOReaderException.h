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

#include <string>
#include <stdexcept>
#include <memory>

/* Common */
#include "Export.h"

/* Core */
#include "KeypleReaderException.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

/**
 * Exception thrown when IO operations failed in a {@link ProxyReader}
 */
class EXPORT KeypleIOReaderException : public KeypleReaderException {
public:
    /**
     * New exception to be thrown
     *
     * @param message : message to identify the exception and the context
     */
    KeypleIOReaderException(const std::string &message);

    /**
     * Encapsulate a lower level reader exception
     *
     * @param message : message to add some context to the exception
     * @param cause : lower level exception
     */
    KeypleIOReaderException(const std::string &message, std::runtime_error cause);

protected:
    /**
     *
     */
    std::shared_ptr<KeypleIOReaderException> shared_from_this()
    {
        return std::static_pointer_cast<KeypleIOReaderException>(KeypleReaderException::shared_from_this());
    }
};

}
}
}
}
