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

#include "PoSelectionRequest.h"

/* Core */
#include "AbstractApduResponseParser.h"
#include "AbstractMatchingSe.h"
#include "ApduRequest.h"
#include "SeProtocol.h"
#include "SeResponse.h"
#include "SeSelector.h"

/* Calypso */
#include "CalypsoDesynchronizedExchangesException.h"
#include "CalypsoPo.h"
#include "PoCustomReadCommandBuilder.h"
#include "PoCustomModificationCommandBuilder.h"
#include "ReadRecordsRespPars.h"
#include "ReadRecordsCmdBuild.h"
#include "SelectFileRespPars.h"
#include "SelectFileCmdBuild.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::builder;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::calypso::transaction::exception;
using namespace keyple::core::command;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;

PoSelectionRequest::PoSelectionRequest(std::shared_ptr<PoSelector> poSelector)
: AbstractSeSelectionRequest(poSelector), poClass(PoClass::LEGACY)
{
    /* No AID selector for a legacy Calypso PO */
    if (seSelector->getAidSelector() == nullptr) {
        poClass = PoClass::LEGACY;
    } else {
        poClass = PoClass::ISO;
    }

    logger->trace("Calypso % selector\n", poClass);
}

void PoSelectionRequest::prepareReadRecordFile(const uint8_t sfi,
                                               const int recordNumber)
{
    addCommandBuilder(
        CalypsoPoUtils::prepareReadRecordFile(mPoClass, sfi, recordNumber));
}

void PoSelectionRequest::prepareSelectFile(const std::vector<uint8_t> lid)
{
    addCommandBuilder(CalypsoPoUtils::prepareSelectFile(mPoClass, lid));
}

void PoSelectionRequest::prepareSelectFile(const uint16_t lid)
{
    const std::vector<uint8_t> bLid = {((lid >> 8) & 0xff), (lid & 0xff)};
    prepareSelectFile(bLid);
}

void PoSelectionRequest::prepareSelectFile(const SelectFileControl selectControl)
{
    addCommandBuilder(
        CalypsoPoUtils::prepareSelectFile(mPoClass, selectControl));
}

std::shared_ptr<CalypsoPo> PoSelectionRequest::parse(
    std::shared_ptr<SeResponse> seResponse) override
{
    const std::vector<std::shared_ptr<AbstractPoCommandBuilder<
        AbstractPoResponseParser>>> commandBuilders = getCommandBuilders();
    const std::vector<std::shared_ptr<ApduResponse>>& apduResponses =
        seResponse->getApduResponses();

    if (static_cast<int>(commandBuilders.size()) !=
        static_cast<int>(apduResponses.size()))
        throw CalypsoDesynchronizedExchangesException(
                "Mismatch in the number of requests/responses");
    }

    std::shared_ptr<CalypsoPo> calypsoPo =
        std::make_shared<CalypsoPo>(
            seResponse,
            mSeSelector->getSeProtocol()->getTransmissionMode());

    if (!commandBuilders.empty())
        CalypsoPoUtils::updateCalypsoPo(calypsoPo,
                                        commandBuilders,
                                        apduResponses);


    return calypsoPo;
}

}
}
}
