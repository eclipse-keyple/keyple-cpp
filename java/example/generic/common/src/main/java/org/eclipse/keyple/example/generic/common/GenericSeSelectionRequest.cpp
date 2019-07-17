/********************************************************************************
 * Copyright (c) 2019 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * See the NOTICE file(s) distributed with this work for additional information regarding copyright
 * ownership.
 *
 * This program and the accompanying materials are made available under the terms of the Eclipse
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
 *
 * SPDX-License-Identifier: EPL-2.0
 ********************************************************************************/

#include "GenericSeSelectionRequest.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace example {
namespace generic {
namespace common {

using namespace org::eclipse::keyple::example::generic::common;

GenericSeSelectionRequest::GenericMatchingSe::GenericMatchingSe(std::shared_ptr<SeResponse> selectionResponse,
                                                                TransmissionMode transmissionMode, std::string& extraInfo)
: AbstractMatchingSe(selectionResponse, transmissionMode, extraInfo)
{
}

GenericSeSelectionRequest::GenericSeSelectionRequest(std::shared_ptr<SeSelector> seSelector, ChannelState channelState)
: AbstractSeSelectionRequest(seSelector, channelState)
{
    transmissionMode = seSelector->getSeProtocol()->getTransmissionMode();
}

std::shared_ptr<AbstractMatchingSe> GenericSeSelectionRequest::parse(std::shared_ptr<SeResponse> seResponse)
{
    return std::make_shared<AbstractMatchingSe>(seResponse, transmissionMode, "Generic Matching SE");
}

}
}
}
}
}
}

