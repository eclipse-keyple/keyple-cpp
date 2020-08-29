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
 * The exception {@code CalypsoPoAnomalyException} indicates an anomaly in the
 * PO.<br>
 * This can occur if the PO is not Calypso compliant.
 */
class CalypsoPoAnomalyException final
: public CalypsoPoTransactionException {
public:
    /**
     * Encapsulates an unexpected {@link CalypsoPoCommandException} exception.
     *
     * @param message message to identify the exception context
     * @param cause the cause
     */
    CalypsoPoAnomalyException(
      const std::string& message, const std::exception& cause)
    : CalypsoPoTransactionException(message, cause) {}

    /**
     * @return the unexpected cause {@link CalypsoPoCommandException}
     */
    const CalypsoPoCommandException& getCause() override
    {
        return CalypsoPoTransactionException::getCause();
    }
};

}
}
}
}
