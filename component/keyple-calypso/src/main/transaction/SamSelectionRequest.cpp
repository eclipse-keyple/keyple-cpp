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

#include "SamSelectionRequest.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command::sam;
using namespace keyple::core::selection;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::message;

SamSelectionRequest::SamSelectionRequest(
  std::shared_ptr<SamSelector> samSelector)
: AbstractSeSelectionRequest<AbstractApduCommandBuilder>(samSelector) {}

const std::shared_ptr<AbstractMatchingSe> SamSelectionRequest::parse(
    std::shared_ptr<SeResponse> seResponse)
{
    return std::make_shared<CalypsoSam>(
               seResponse, mSeSelector->getSeProtocol()->getTransmissionMode());
}

}
}
}
