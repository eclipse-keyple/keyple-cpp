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
#include "CalypsoSamCommandException.h"

namespace keyple {
namespace calypso {
namespace transaction {
namespace exception {

using namespace keyple::calypso::command::sam::builder::exception;

/**
 * The exception {@code CalypsoSamAnomalyException} indicates an anomaly in the
 * SAM.<br> This can occur if the SAM is not Calypso compliant.
 */
class CalypsoSamAnomalyException final : public CalypsoPoTransactionException {
public:
    /**
     * Encapsulates an unexpected
     * keyple::calypso::command::sam::exception::CalypsoSamCommandException exception.
     *
     * @param message message to identify the exception context
     * @param cause the cause
     */
    CalypsoSamAnomalyException(
      const std::string& message, const std::exception& cause)
    : CalypsoPoTransactionException(message, cause) {}

    /**
     * @return the unexpected cause
     *         keyple::calypso::command::sam::exception::CalypsoSamCommandException
     */
    const CalypsoSamCommandException& getCause() override
    {
        return CalypsoPoTransactionException::getCause();
    }
};

}
}
}
}
