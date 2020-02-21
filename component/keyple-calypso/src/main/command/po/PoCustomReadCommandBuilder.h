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

#include "PoSendableInSession.h"
#include "AbstractIso7816CommandBuilder.h"
#include "PoRevision.h"

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
namespace po {

using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

/**
 * Class to build custom (non-referenced) read PO commands
 */
class PoCustomReadCommandBuilder : public AbstractIso7816CommandBuilder,
                                   public PoSendableInSession {
public:
    /**
     * Constructor dedicated to the construction of user-defined commands.
     *
     * Caveat:
     * <ul>
     * <li>the caller has to provide all the command data.</li>
     * <li>Using this method bypasses the security and functional verification
     * mechanisms of the PoTransaction API.
     * <p>
     * It is done at the user's risk.</li>
     * </ul>
     * 
     * @param name the name of the command (will appear in the ApduRequest log)
     * @param request the ApduRequest (the correct instruction byte must be
     *        provided)
     */
    PoCustomReadCommandBuilder(const std::string& name,
                               std::shared_ptr<ApduRequest> request);

    /**
     *
     */
    virtual ~PoCustomReadCommandBuilder()
    {
    }

protected:
    /**
     *
     */
    std::shared_ptr<PoCustomReadCommandBuilder> shared_from_this()
    {
        return std::static_pointer_cast<PoCustomReadCommandBuilder>(
            AbstractIso7816CommandBuilder::shared_from_this());
    }

    /**
     *
     */
    PoRevision defaultRevision = PoRevision::REV3_1;
};

}
}
}
}
