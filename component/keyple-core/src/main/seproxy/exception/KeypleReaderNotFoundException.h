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

/* Core */
#include "KeypleReaderException.h"

/* Common */
#include "Export.h"
#include "stringhelper.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

/**
 * Exception thrown when {@link org.eclipse.keyple.seproxy.message.ProxyReader}
 * is not found
 */
class EXPORT KeypleReaderNotFoundException : public KeypleReaderException {
public:
    KeypleReaderNotFoundException(const std::string &readerName)
    : KeypleReaderException(
        StringHelper::formatSimple("Reader with name %s was not found",
                                   readerName))
    {
    }
};

}
}
}
}
