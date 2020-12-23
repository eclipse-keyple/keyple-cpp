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

#include "AbstractDefaultSelectionsRequest.h"
#include "ChannelControl.h"
#include "KeypleCoreExport.h"
#include "MultiSeRequestProcessing.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using namespace keyple::core::seproxy::event;

/**
 * Class containing the Set of keyple::core::seproxy::message::SeRequest used to make a default
 * selection at the keyple::core::seproxy::event::ObservableReader level.<br>
 * It implements the keyple::core::seproxy::event::AbstractDefaultSelectionsRequest  class by providing
 * it with a public constructor.
 */
class KEYPLECORE_API DefaultSelectionsRequest final
: public AbstractDefaultSelectionsRequest {
public:
    /**
     *
     */
    DefaultSelectionsRequest(
        const std::vector<std::shared_ptr<SeRequest>>& selectionSeRequests,
        const MultiSeRequestProcessing& multiSeRequestProcessing,
        const ChannelControl& channelControl);
};

}
}
}
}
