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

#include <memory>

#include "AbstractDefaultSelectionsRequest.h"
#include "ChannelControl.h"
#include "MultiSeRequestProcessing.h"
#include "ObservableReader_Import.h"

/* Forward class declarations */
namespace keyple { namespace core { namespace seproxy { namespace message {
    class SeRequestSet; } } } }

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using namespace keyple::core::seproxy::event;

/**
 * Class containing the Set of {@link SeRequest} used to make a default
 * selection at the {@link ObservableReader} level.
 */
class DefaultSelectionsRequest final : public AbstractDefaultSelectionsRequest {
public:
    /**
     *
     */
    DefaultSelectionsRequest(
        std::set<std::shared_ptr<SeRequest>>& selectionSeRequestSet,
        MultiSeRequestProcessing multiSeRequestProcessing,
        ChannelControl channelControl);

    /**
     *
     */
    DefaultSelectionsRequest(
        std::set<std::shared_ptr<SeRequest>>& selectionSeRequestSet);

    /**
     *
     */
    MultiSeRequestProcessing& getMultiSeRequestProcessing() override;

    /**
     *
     */
    ChannelControl& getChannelControl() override;

    /**
     *
     */
    std::set<std::shared_ptr<SeRequest>>& getSelectionSeRequestSet() override;

private:
    /**
     *
     */
    std::set<std::shared_ptr<SeRequest>>& selectionSeRequestSet;

    /**
     *
     */
    MultiSeRequestProcessing multiSeRequestProcessing;

    /**
     *
     */
    ChannelControl channelControl;
};

}
}
}
}
