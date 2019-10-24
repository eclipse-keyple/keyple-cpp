/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#pragma once

#include <string>
#include <vector>
#include <memory>

#include "stringhelper.h"

/* Core */
#include "ChannelState.h"
#include "ApduRequest.h"
#include "SeProtocol_Import.h"

/* Common */
#include "Export.h"
#include "Serializable.h"

/* Forward declaration */
namespace keyple { namespace core { namespace seproxy { class SeSelector; }}}

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::protocol;

/**
 * List of APDU requests that will result in a {@link SeResponse}
 *
 * @see SeResponse
 */
class EXPORT SeRequest final : public std::enable_shared_from_this<SeRequest>, public Serializable {
public:
    /**
     * The constructor called by a ProxyReader in order to open a logical channel, to send a set of
     * APDU commands to a SE application, or both of them.
     *
     * @param seSelector the SeSelector containing the selection information to process the SE
     *        selection
     * @param apduRequests a optional list of {@link ApduRequest} to execute after a successful
     *        selection process
     * @param channelState the channel management parameter allowing to close or keep the channel
     *        open after the request execution
     */
    SeRequest(std::shared_ptr<SeSelector> seSelector, std::vector<std::shared_ptr<ApduRequest>> &apduRequests, ChannelState channelState);

    /**
     * Constructor to be used when the SE is already selected (without {@link SeSelector})
     *
     * @param apduRequests a list of ApudRequest
     * @param channelState a flag to tell if the channel has to be closed at the end
     */
    SeRequest(std::vector<std::shared_ptr<ApduRequest>> &apduRequests, ChannelState channelState);

    /**
     *
     */
    virtual ~SeRequest() {}

    /**
     * Gets the SE seSelector.
     *
     * @return the current SE seSelector
     */
    std::shared_ptr<SeSelector> getSeSelector();

    /**
     * Gets the apdu requests.
     *
     * @return the group of APDUs to be transmitted to the SE application for this instance of
     *         SERequest.
     */
    std::vector<std::shared_ptr<ApduRequest>> getApduRequests();

    /**
     * Define if the channel should be kept open after the the {@link SeRequestSet} has been
     * executed.
     *
     * @return If the channel should be kept open
     */
    bool isKeepChannelOpen();


    /**
     *
     */
    std::string toString() override;

    /**
     *¬
     */
    friend std::ostream &operator<<(std::ostream &os, SeRequest &se)
    {
        os << se.toString();

        return os;
    }

private:
    /**
     * SE seSelector is either an AID or an ATR regular expression
     */
    std::shared_ptr<SeSelector> seSelector;

    /**
     * contains a group of APDUCommand to operate on the selected SE application by the SE reader.
     */
    std::vector<std::shared_ptr<ApduRequest>> apduRequests;


    /**
     * the final logical channel status: the SE reader may kept active the logical channel of the SE
     * application after processing the group of APDU commands otherwise the SE reader will close
     * the logical channel of the SE application after processing the group of APDU commands (i.e.
     * after the receipt of the last APDU response).
     */
    ChannelState channelState = static_cast<ChannelState>(0);

};

}
}
}
}
