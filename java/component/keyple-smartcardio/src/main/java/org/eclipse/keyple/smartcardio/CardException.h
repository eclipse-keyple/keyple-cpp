/*
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* All rights reserved. This program and the accompanying materials are made available under the
* terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
* available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*/

#pragma once

/* Common */
#include "Exception.h"
#include "Export.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace smartcardio {

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
}
}

