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

#include "CalypsoPoTransactionException.h"

namespace keyple {
namespace calypso {
namespace transaction {
namespace exception {

/**
 * The exception {@code CalypsoPoTransactionIllegalStateException} indicates an
 * improper use of the {@link PoTransaction} API.
 */
class CalypsoPoTransactionIllegalStateException final
: public CalypsoPoTransactionException {
public:
    /**
     * @param message the message to identify the exception context
     */
    CalypsoPoTransactionIllegalStateException(const std::string& message)
    : CalypsoPoTransactionException(message) {}
};

}
}
}
}
