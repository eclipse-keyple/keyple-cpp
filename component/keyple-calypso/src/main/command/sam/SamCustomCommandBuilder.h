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

#include <string>
#include <memory>

#include "AbstractIso7816CommandBuilder.h"
#include "SamRevision.h"

/* Forward declaration */
namespace keyple {
namespace seproxy {
namespace message {
class ApduRequest;
}
}
}

namespace keyple {
namespace calypso {
namespace command {
namespace sam {

using namespace keyple::command;
using namespace keyple::seproxy::message;

/**
 * Class to build custom (non-referenced) SAM commands
 */
class SamCustomCommandBuilder : public AbstractIso7816CommandBuilder {
public:
    /**
     * Constructor dedicated to the construction of user-defined commands.
     *
     * Caveat: the caller has to provide all the command data.
     *
     * @param name the name of the command (will appear in the ApduRequest log)
     * @param request the ApduRequest (the correct instruction byte must be
     *        provided)
     */
    SamCustomCommandBuilder(const std::string& name,
                            std::shared_ptr<ApduRequest> request);

protected:
    /**
     *
     */
    SamRevision defaultRevision = SamRevision::S1D; // 94

    /**
     *
     */
    std::shared_ptr<SamCustomCommandBuilder> shared_from_this()
    {
        return std::static_pointer_cast<SamCustomCommandBuilder>(
            sAbstractIso7816CommandBuilder::shared_from_this());
    }
};

}
}
}
}
