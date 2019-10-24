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

#include "Exception.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

using namespace keyple::common;

/**
 * Base Exception for all Keyple Checked Exception
 */
class KeypleBaseException : public std::enable_shared_from_this<KeypleBaseException>, public Exception {
public:
    /**
     * New exception to be thrown
     *
     * @param message : message to identify the exception and the context
     */
    KeypleBaseException(const std::string &message);

    /**
     * Encapsulate a lower level exception (ie CardException, IOException, HostNotFoundException..)
     *
     * @param message : message to identify the exception and the context
     * @param cause : lower level exception
     */
    KeypleBaseException(const std::string &message, std::runtime_error cause);

private:
    /**
     *
     */
    static constexpr long long serialVersionUID = -500856379312027085LL;


};

}
}
}
}
