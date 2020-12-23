/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#pragma once

#include <list>
#include <string>
#include <stdexcept>
#include <memory>

/* Core */
#include "KeypleException.h"
#include "KeypleCoreExport.h"
#include "SeResponse.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

using namespace keyple::core::seproxy::message;

/**
 * The exception KeypleReaderException is the parent abstract class of
 * all Keyple reader exceptions.
 */
class KEYPLECORE_API KeypleReaderException : public KeypleException {
protected:
    /**
     * @param msg the message to identify the exception context
     */
    KeypleReaderException(const std::string& msg) : KeypleException(msg) {}

    /**
     * Encapsulates a lower level reader exception
     *
     * @param msg message to identify the exception context
     * @param cause the cause
     */
    KeypleReaderException(const std::string& msg, const std::exception& cause)
    : KeypleException(msg, cause) {}
};

}
}
}
}
