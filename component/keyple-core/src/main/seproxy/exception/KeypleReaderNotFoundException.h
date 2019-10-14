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
#include <memory>

/* Core */
#include "KeypleReaderException.h"

/* Common */
#include "Export.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

/*
    * Exception thrown when {@link org.eclipse.keyple.seproxy.message.ProxyReader} is not found
    */
class EXPORT KeypleReaderNotFoundException : public KeypleReaderException {

    /**
        * Exception thrown when @{@link ProxyReader} is not found
        * 
        * @param readerName : readerName that has not been found
        */
    public:
    KeypleReaderNotFoundException(const std::string &readerName);

    protected:
    std::shared_ptr<KeypleReaderNotFoundException> shared_from_this()
    {
        return std::static_pointer_cast<KeypleReaderNotFoundException>(KeypleReaderException::shared_from_this());
    }
};

} // namespace exception
}     // namespace seproxy
}         // namespace keyple
}             // namespace eclipse
} // namespace org
}
