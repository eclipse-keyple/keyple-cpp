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

#include <memory>

/* Calypso */
#include "CalypsoSam.h"
#include "SamSelector.h"

/* Core */
#include "AbstractSamCommandBuilder.h"
#include "AbstractSamResponseParser.h"
#include "AbstractSeSelectionRequest.h"
#include "KeypleCalypsoExport.h"
#include "SeProtocol.h"
#include "SeResponse.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command::sam;
using namespace keyple::core::command;
using namespace keyple::core::selection;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::seproxy::message;

/**
 * Specialized selection request to manage the specific characteristics of Calypso SAMs<br>
 * Beyond the creation of a keyple::calypso::transaction::CalypsoSam object, this class also allows
 * to execute a command to Unlock the SAM if unlockData are present in the
 * keyple::calypso::transaction::SamSelector.<br>
 * This unlock command is currently the only one allowed during the SAM selection process.
 */
class KEYPLECALYPSO_API SamSelectionRequest
: public AbstractSeSelectionRequest<AbstractSamCommandBuilder<AbstractSamResponseParser>> {
public:
    /**
     * Create a keyple::calypso::transaction::SamSelectionRequest
     *
     * @param samSelector the SAM selector
     */
    explicit SamSelectionRequest(std::shared_ptr<SamSelector> samSelector);

    /**
     * Create a CalypsoSam object containing the selection data received from the plugin<br>
     * If an Unlock command has been prepared, its status is checked.
     *
     * @param seResponse the SE response received
     * @return a keyple::calypso::transaction::CalypsoSam
     * @throw CalypsoDesynchronizedExchangesException if the APDU SAM exchanges are out of sync
     * @throw CalypsoSamCommandException if the SAM has responded with an error status
     *
     * Return type should be
     *     std::shared_ptr<CalypsoSam>
     * ... but invalid covariant return type
     */
    const std::shared_ptr<AbstractMatchingSe> parse(std::shared_ptr<SeResponse> seResponse)
        override;
};

}
}
}
