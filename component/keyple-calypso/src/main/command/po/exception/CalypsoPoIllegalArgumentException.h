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

#include "CalypsoPoCommandException.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace exception {

/**
 * The exception {@code CalypsoPoIllegalArgumentException} indicates that the
 * input user data do not allow to build a syntactically correct command (e.g.
 * SFI too large, record number out of bounds,...).
 */
class CalypsoPoIllegalArgumentException final
: public CalypsoPoCommandException {
public:
    /**
     * @param message the message to identify the exception context
     * @param command the Calypso PO command
     */
    CalypsoPoIllegalArgumentException(const std::string& message,
                                      const std::shared_ptr<SeCommand> command)
    : CalypsoPoCommandException(
        message, std::dynamic_pointer_cast<CalypsoPoCommand>(command), nullptr)
    {}
};

}
}
}
}
}
