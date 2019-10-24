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

/* Common */
#include "Exception.h"
#include "Export.h"

namespace keyple {
namespace smartcardio {

using namespace keyple::common;

class EXPORT CardException : public Exception {
public:
    /**
     * New exception to be thrown
     *
     * @param message : message to identify the exception and the context
     */
    CardException(const std::string &message) : Exception(message)
    {
    }

    /**
     * New exception to be thrown
     *
     * @param message : message to identify the exception and the context
     * @param cause : cause of the exception
     */
    CardException(const std::string &message, std::runtime_error cause)
    : Exception(message, cause)
    {
    }

};

}
}

