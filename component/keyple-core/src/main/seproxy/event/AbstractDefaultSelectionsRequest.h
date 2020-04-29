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

#include <set>
#include <memory>

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
 * The abstract class defining the default selection request to be processed
 * when an SE is inserted in an observable reader.
 * <p>
 * The default selection is defined by:
 * <ul>
 * <li>a set of requests corresponding to one or more selection cases
 * <li>a {@link MultiSeRequestProcessing} indicator specifying whether all
 * planned selections are to be executed or whether to stop at the first one
 * that is successful
 * <li>an indicator to control the physical channel to stipulate whether it
 * should be closed or left open at the end of the selection process
 * </ul>
 */
class KEYPLECORE_API AbstractDefaultSelectionsRequest
: public std::enable_shared_from_this<AbstractDefaultSelectionsRequest> {
public:
    /**
     *
     */
    virtual ~AbstractDefaultSelectionsRequest()
    {
    }

protected:
    /**
     * @return the selection request set
     */
    virtual std::set<std::shared_ptr<SeRequest>>&
    getSelectionSeRequestSet() = 0;

    /**
     * @return the multi SE request mode
     */
    virtual MultiSeRequestProcessing& getMultiSeRequestProcessing() = 0;

    /**
     * @return the channel control
     */
    virtual ChannelControl& getChannelControl() = 0;
};

}
}
}
}
