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
#include <vector>

/* Core */
#include "KeypleCoreExport.h"
#include "ChannelControl.h"
#include "MultiSeRequestProcessing.h"
#include "SeRequest.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace event {

using namespace keyple::core::seproxy::message;

/**
 * The abstract class defining the default selection request to be processed when an SE is inserted
 * in an observable reader.
 * <p>
 * The default selection is defined by:
 * <ul>
 * <li>a set of requests corresponding to one or more selection cases
 * <li>a keyple::core::seproxy::MultiSeRequestProcessing indicator specifying whether all planned selections are to
 * be executed or whether to stop at the first one that is successful
 * <li>an indicator to control the physical channel to stipulate whether it should be closed or left
 * open at the end of the selection process
 * </ul>
 * The purpose of this abstract class is to hide the constructor that is defined as public in its
 * implementation {@link keyple::core::seproxy::message::DefaultSelectionsRequest}.
 */
class KEYPLECORE_API AbstractDefaultSelectionsRequest {
public:
    /**
     *
     */
    virtual ~AbstractDefaultSelectionsRequest() {}


    /**
     * @return the list of requests that make up the selection
     */
    virtual const std::vector<std::shared_ptr<SeRequest>>& getSelectionSeRequests() const final;

    /**
     * @return the flag indicating whether the selection process should stop after the first
     *         matching or process all
     */
    virtual const MultiSeRequestProcessing& getMultiSeRequestProcessing() const final;

    /**
     * @return the flag indicating whether the logic channel is to be kept open or closed
     */
    virtual const ChannelControl& getChannelControl() const final;

protected:
    /**
     *
     */
    AbstractDefaultSelectionsRequest(
        const std::vector<std::shared_ptr<SeRequest>>& selectionSeRequests,
        const MultiSeRequestProcessing& multiSeRequestProcessing,
        const ChannelControl& channelControl);

private:
    /**
     *
     */
    const std::vector<std::shared_ptr<SeRequest>> mSelectionSeRequests;

    /**
     *
     */
    const MultiSeRequestProcessing mMultiSeRequestProcessing;

    /**
     *
     */
    const ChannelControl mChannelControl;
};

}
}
}
}
