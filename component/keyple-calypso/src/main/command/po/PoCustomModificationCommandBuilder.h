/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#pragma once

#include <string>
#include <memory>

#include "PoModificationCommand.h"
#include "AbstractIso7816CommandBuilder.h"
#include "PoRevision.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class ApduRequest; } } } } }


namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {

using AbstractIso7816CommandBuilder = org::eclipse::keyple::core::command::AbstractIso7816CommandBuilder;
using ApduRequest                   = org::eclipse::keyple::core::seproxy::message::ApduRequest;

/**
 * Class to build custom (non-referenced) modification PO commands
 */
class PoCustomModificationCommandBuilder : public AbstractIso7816CommandBuilder, public PoModificationCommand {

protected:
    /**
     *
     */
    PoRevision defaultRevision = PoRevision::REV3_1;

public:
    /**
     * Constructor dedicated to the construction of user-defined commands.
     *
     * Caveat:
     * <ul>
     * <li>the caller has to provide all the command data.</li>
     * <li>Using this method bypasses the security and functional verification mechanisms of the
     * PoTransaction API.
     * <p>
     * It is done at the user's risk.</li>
     * </ul>
     *
     * @param name the name of the command (will appear in the ApduRequest log)
     * @param request the ApduRequest (the correct instruction byte must be provided)
     */
    PoCustomModificationCommandBuilder(const std::string &name, std::shared_ptr<ApduRequest> request);

    /**
     *
     */
    virtual ~PoCustomModificationCommandBuilder() {}

protected:
    /**
     *
     */
    std::shared_ptr<PoCustomModificationCommandBuilder> shared_from_this()
    {
        return std::static_pointer_cast<PoCustomModificationCommandBuilder>(AbstractIso7816CommandBuilder::shared_from_this());
    }
};

}
}
}
}
}
}
