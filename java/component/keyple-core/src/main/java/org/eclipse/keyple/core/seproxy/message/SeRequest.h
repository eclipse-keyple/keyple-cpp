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
#include "Protocol.h"
#include "ChannelState.h"
#include "SeSelector.h"
#include "ApduRequest.h"

/* Common */
#include "Export.h"
#include "Serializable.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using org::eclipse::keyple::core::seproxy::ChannelState;
using org::eclipse::keyple::core::seproxy::protocol::Protocol;
using org::eclipse::keyple::core::seproxy::protocol::SeProtocol;
using org::eclipse::keyple::core::seproxy::SeSelector;

/**
    * List of APDU requests that will result in a {@link SeResponse}
    *
    * @see SeResponse
    */
class EXPORT SeRequest final : public std::enable_shared_from_this<SeRequest>, public Serializable {

public:
    static constexpr long long serialVersionUID = 6018469841127325812LL;

    /**
        * SE seSelector is either an AID or an ATR regular expression
        */
private:
    std::shared_ptr<SeSelector> seSelector;

    /**
        * contains a group of APDUCommand to operate on the selected SE application by the SE reader.
        */
    std::vector<std::shared_ptr<ApduRequest>> apduRequests;


    /**
        * the protocol flag is used to target specific SE technologies for a given request
        */
    std::shared_ptr<SeProtocol> protocolFlag = std::dynamic_pointer_cast<SeProtocol>(std::make_shared<Protocol>(Protocol::ANY));

    /**
        * the final logical channel status: the SE reader may kept active the logical channel of the SE
        * application after processing the group of APDU commands otherwise the SE reader will close
        * the logical channel of the SE application after processing the group of APDU commands (i.e.
        * after the receipt of the last APDU response).
        */
    ChannelState channelState = static_cast<ChannelState>(0);

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
        * @param protocolFlag the expected protocol for the SE (may be set to Protocol.ANY if no check
        *        is needed)
        */
public:
    SeRequest(std::shared_ptr<SeSelector> seSelector, std::vector<std::shared_ptr<ApduRequest>> &apduRequests, ChannelState channelState, std::shared_ptr<SeProtocol> protocolFlag);

    /**
        * Constructor to be used when the SE is already selected (without {@link SeSelector})
        *
        * @param apduRequests a list of ApudRequest
        * @param channelState a flag to tell if the channel has to be closed at the end
        */
    SeRequest(std::vector<std::shared_ptr<ApduRequest>> &apduRequests, ChannelState channelState);


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
        * Gets the protocol flag of the request
        *
        * @return protocolFlag
        */
    std::shared_ptr<SeProtocol> getProtocolFlag();

    std::string toString() override;
};

}
}
}
}
}
}
