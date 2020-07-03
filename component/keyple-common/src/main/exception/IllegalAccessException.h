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

#include "Exception.h"

namespace keyple {
namespace common {

class IllegalAccessException : public Exception {
public:
    /**
     *
     */
    IllegalAccessException(const std::string& message) : Exception(message) {}

    /**
     *
     */
    IllegalAccessException(
      const std::string& message, const std::exception cause)
    : Exception(message, cause) {}
};

}
}
