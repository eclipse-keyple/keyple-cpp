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

#include "SamSelectionRequest.h"

/* Calypso */
#include "CalypsoDesynchronizedExchangesException.h"
#include "UnlockCmdBuild.h"

/* Common */
#include "KeypleStd.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::builder::security;
using namespace keyple::calypso::transaction::exception;
using namespace keyple::common;
using namespace keyple::core::selection;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::message;

using AbstractSamCommandResponse = AbstractSamCommandBuilder<AbstractSamResponseParser>;

SamSelectionRequest::SamSelectionRequest(std::shared_ptr<SamSelector> samSelector)
: AbstractSeSelectionRequest<AbstractSamCommandBuilder<AbstractSamResponseParser>>(samSelector)
{
    const std::vector<uint8_t> data = samSelector->getUnlockData();
    if (!data.empty()) {
        /*
         * A unlock data value has been set, let's add the unlock command to be executed
         * following the selection
         */
        auto unlock = std::make_shared<UnlockCmdBuild>(samSelector->getTargetSamRevision(),
                                                       samSelector->getUnlockData());
        auto cmd = std::reinterpret_pointer_cast<AbstractSamCommandResponse>(unlock);
        addCommandBuilder(cmd);
    }
}

const std::shared_ptr<AbstractMatchingSe> SamSelectionRequest::parse(
    std::shared_ptr<SeResponse> seResponse)
{
    std::vector<std::shared_ptr<AbstractSamCommandResponse>> commandBuilders =
        getCommandBuilders();

    if (commandBuilders.size() == 1) {
        /* An unlock command has been requested */
        std::vector<std::shared_ptr<ApduResponse>> apduResponses = seResponse->getApduResponses();
        if (apduResponses.empty())
            throw CalypsoDesynchronizedExchangesException("Mismatch in the number of " \
                                                          "requests/responses");

        /* Check the SAM response to the unlock command */
        commandBuilders[0]->createResponseParser(apduResponses[0])->checkStatus();
    }

    return std::make_shared<CalypsoSam>(seResponse,
                                        mSeSelector->getSeProtocol()->getTransmissionMode());
}

}
}
}
