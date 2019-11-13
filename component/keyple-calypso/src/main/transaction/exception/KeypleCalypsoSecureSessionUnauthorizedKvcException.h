/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

#pragma once

#include "KeypleReaderException.h"
#include <string>
#include <memory>

namespace keyple {
namespace calypso {
namespace transaction {
namespace exception {

using namespace keyple::core::seproxy::exception;

/**
 * Thrown when the current PO has an unauthorized KVC
 */
class KeypleCalypsoSecureSessionUnauthorizedKvcException
: public KeypleReaderException {
public:
    /**
     *
     */
    KeypleCalypsoSecureSessionUnauthorizedKvcException(
        const std::string &message);

    KeypleCalypsoSecureSessionUnauthorizedKvcException(
        const KeypleCalypsoSecureSessionUnauthorizedKvcException& o);

    /**
     *
     */
    virtual ~KeypleCalypsoSecureSessionUnauthorizedKvcException() {}

};

}
}
}
}
