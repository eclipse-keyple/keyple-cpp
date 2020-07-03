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
#include "KeypleCoreExport.h"
#include "stringhelper.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

/**
 * The exception {@code KeypleReaderNotFoundException} indicates that the reader
 * is not found by its name, generally when it is not connected to the terminal.
 */
class KEYPLECORE_API KeypleReaderNotFoundException
: public KeypleReaderException {
public:
    /**
     * @param readerName the name of the reader that has not been found
     */
    KeypleReaderNotFoundException(const std::string& readerName)
    : KeypleReaderException(StringHelper::formatSimple(
          "Reader with name %s was not found", readerName))
    {
    }
};

}
}
}
}
