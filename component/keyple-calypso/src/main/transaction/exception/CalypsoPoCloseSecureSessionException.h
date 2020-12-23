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

#include "CalypsoPoTransactionException.h"

/* Calypso */
#include "CalypsoPoCommandException.h"

namespace keyple {
namespace calypso {
namespace transaction {
namespace exception {

using namespace keyple::calypso::command::po::exception;

/**
 * The exception keyple::calypso::transaction::exception::CalypsoPoCloseSecureSessionException
 * indicates that the PO has refused the secure session closing.<br>
 * This is usually due to an incorrect SAM signature, or that the secure session has been altered by
 * other APDU commands that would have interfered with it.
 * <br>
 * In this case, the PO has rollbacked the data set by cancelling all updates except for PIN
 * verification attempts.
 */
class CalypsoPoCloseSecureSessionException final : public CalypsoPoTransactionException {
public:
    /**
     * Encapsulates the keyple::calypso::command::po::exception::CalypsoPoCommandException
     * exception.
     *
     * @param message message to identify the exception context
     * @param cause the cause
     */
    CalypsoPoCloseSecureSessionException(const std::string& message, const std::exception& cause)
    : CalypsoPoTransactionException(message, cause) {}

    /**
     * @return the cause keyple::calypso::command::po::exception::CalypsoPoCommandException
     */
    const CalypsoPoCommandException& getCause() //override
    {
        std::exception e = CalypsoPoTransactionException::getCause();
        return std::move(dynamic_cast<CalypsoPoCommandException&>(e));
    }
};

}
}
}
}
