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

#include "KeypleReaderException.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

/**
    * Application selection failure in {@link ProxyReader} by AID or ATR
    */
class KeypleApplicationSelectionException : public KeypleReaderException {


public:
    KeypleApplicationSelectionException(const std::string &message);

    KeypleApplicationSelectionException(const std::string &message, std::runtime_error cause);


protected:
    std::shared_ptr<KeypleApplicationSelectionException> shared_from_this() {
        return std::static_pointer_cast<KeypleApplicationSelectionException>(KeypleReaderException::shared_from_this());
    }
};

}
}
}
}
}
}
