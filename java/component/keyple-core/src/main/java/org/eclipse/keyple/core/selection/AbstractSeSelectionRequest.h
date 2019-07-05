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

#pragma once

#include <vector>
#include <memory>

#include "ChannelState.h"
#include "exceptionhelper.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace core { namespace seproxy { class SeSelector; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace core { namespace seproxy { namespace message { class ApduRequest; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace core { namespace seproxy { namespace message { class SeRequest; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace core { namespace command { class AbstractApduResponseParser; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace core { namespace seproxy { namespace message { class SeResponse; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace core { namespace selection { class AbstractMatchingSe; } } } } }

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace selection {

using AbstractApduResponseParser = org::eclipse::keyple::core::command::AbstractApduResponseParser;
using ChannelState               = org::eclipse::keyple::core::seproxy::ChannelState;
using SeSelector                 = org::eclipse::keyple::core::seproxy::SeSelector;
using ApduRequest                = org::eclipse::keyple::core::seproxy::message::ApduRequest;
using SeRequest                  = org::eclipse::keyple::core::seproxy::message::SeRequest;
using SeResponse                 = org::eclipse::keyple::core::seproxy::message::SeResponse;

/**
    * The AbstractSeSelectionRequest class combines a SeSelector with additional helper methods useful
    * to the selection process done in {@link SeSelection}.
    * <p>
    * This class may also be extended to add particular features specific to a SE family.
    */
class AbstractSeSelectionRequest : public std::enable_shared_from_this<AbstractSeSelectionRequest> {
public:
    const std::shared_ptr<SeSelector> seSelector;

    /** optional apdu requests list to be executed following the selection process */
private:
    std::vector<std::shared_ptr<ApduRequest>> seSelectionApduRequestList = std::vector<std::shared_ptr<ApduRequest>>();

    /**
        * the channelState may be accessed from derived classes. Let it with the protected access
        * level.
        */
    const ChannelState channelState;

public:
    AbstractSeSelectionRequest(std::shared_ptr<SeSelector> seSelector, ChannelState channelState);

    /**
        * Returns a selection SeRequest built from the information provided in the constructor and
        * possibly completed with the seSelectionApduRequestList
        *
        * @return the selection SeRequest
        */
    std::shared_ptr<SeRequest> getSelectionRequest();

    virtual std::shared_ptr<SeSelector> getSeSelector();

    /**
        * Add an additional {@link ApduRequest} to be executed after the selection process if it
        * succeeds.
        * <p>
        * If more than one {@link ApduRequest} is added, all will be executed in the order in which
        * they were added.
        *
        * @param apduRequest an {@link ApduRequest}
        */
protected:
    void addApduRequest(std::shared_ptr<ApduRequest> apduRequest);

    /**
        * Return the parser corresponding to the command whose index is provided.
        *
        * @param seResponse the received SeResponse containing the commands raw responses
        * @param commandIndex the command index
        * @return a parser of the type matching the command
        */
public:
    virtual std::shared_ptr<AbstractApduResponseParser> getCommandParser(std::shared_ptr<SeResponse> seResponse, int commandIndex);

    /**
        * Virtual parse method
        * 
        * @param seResponse the SE response received
        * @return a {@link AbstractMatchingSe}
        */
public:
    virtual std::shared_ptr<AbstractMatchingSe> parse(std::shared_ptr<SeResponse> seResponse) = 0;
};

}
}
}
}
}
