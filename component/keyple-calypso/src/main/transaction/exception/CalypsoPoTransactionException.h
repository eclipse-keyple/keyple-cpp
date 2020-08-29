/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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

#include "KeypleException.h"

namespace keyple {
namespace calypso {
namespace transaction {
namespace exception {

using namespace keyple::core::seproxy::exception;

/**
 * The exception {@code CalypsoPoTransactionException} is the parent abstract
 * class of all Keyple PO transaction functional exceptions.
 */
class CalypsoPoTransactionException : public KeypleException {
protected:
    /**
     * @param message the message to identify the exception context
     */
     CalypsoPoTransactionException(const std::string& message)
    : CalypsoPoTransactionException(message) {}

    /**
     * Encapsulates a lower level PO transaction exception
     *
     * @param message message to identify the exception context
     * @param cause the cause
     */
    CalypsoPoTransactionException(
      const std::string& message, const std::exception& cause)
    : CalypsoPoTransactionException(message, cause) {}
};

}
}
}
}
