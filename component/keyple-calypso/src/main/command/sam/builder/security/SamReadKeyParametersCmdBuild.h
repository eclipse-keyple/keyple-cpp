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

#include "AbstractSamCommandBuilder.h"
#include "CalypsoSamCommand.h"
#include "SamRevision.h"

/* Forward declaration */
namespace keyple { namespace calypso { namespace command { namespace sam {
    namespace parser { namespace security { class SamReadKeyParametersRespPars; } } }
    } } }

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::parser::security;

/**
 * Builder for the SAM Read Key Parameters APDU command.
 */
class SamReadKeyParametersCmdBuild final
: public AbstractSamCommandBuilder<SamReadKeyParametersRespPars> {
public:
    /**
     *
     */
    static const int MAX_WORK_KEY_REC_NUMB = 126;

    /**
     *
     */
    enum SourceRef { WORK_KEY, SYSTEM_KEY };

    /**
     *
     */
    enum NavControl { FIRST, NEXT };

    /**
     *
     */
    SamReadKeyParametersCmdBuild(const SamRevision& revision);

    /**
     *
     */
    SamReadKeyParametersCmdBuild(const SamRevision& revision,
                                 const uint8_t kif);

    /**
     *
     */
    SamReadKeyParametersCmdBuild(const SamRevision& revision, const uint8_t kif,
                                 const uint8_t kvc);

    /**
     *
     */
    SamReadKeyParametersCmdBuild(const SamRevision& revision,
                                 const SourceRef& sourceKeyRef,
                                 const uint8_t recordNumber);

    /**
     *
     */
    SamReadKeyParametersCmdBuild(const SamRevision& revision, const uint8_t kif,
                                 const NavControl& navControl);

    /**
     *
     */
    std::shared_ptr<SamReadKeyParametersRespPars> createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse) override;

private:
    /**
     * The command reference
     */
    const CalypsoSamCommand& mCommand = CalypsoSamCommand::READ_KEY_PARAMETERS;
};

}
}
}
}
}
}
