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

#include "SamSelectionRequest.h"
#include "SeSelector_Import.h"
#include "SeProtocol_Import.h"
#include "CalypsoSam.h"
#include "SeResponse.h"
#include "AbstractSamResponseParser.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command::sam;
using namespace keyple::core::selection;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::message;

SamSelectionRequest::SamSelectionRequest(
  std::shared_ptr<SamSelector> samSelector)
: AbstractSeSelectionRequest(samSelector)
{
}

std::shared_ptr<AbstractMatchingSe> SamSelectionRequest::parse(
    std::shared_ptr<SeResponse> seResponse)
{
    return std::dynamic_pointer_cast<AbstractMatchingSe>(
              std::make_shared<CalypsoSam>(
                  seResponse, seSelector->getSeProtocol().getTransmissionMode(),
                  seSelector->getExtraInfo()));
}

std::shared_ptr<AbstractApduResponseParser>
    SamSelectionRequest::getCommandParser(
        std::shared_ptr<SeResponse> seResponse, int commandIndex)
{
    (void)seResponse;
    (void)commandIndex;

    /* not yet implemented in keyple-calypso */
    // TODO add a generic command parser
    throw IllegalStateException("No parsers available for this request.");
}

}
}
}
