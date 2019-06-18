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
#include <typeinfo>
#include <memory>
#include <typeindex>

#include "MatchingSe.h"
#include "ChannelState.h"

/* Common */
#include "Export.h"
#include "Logger.h"
#include "LoggerFactory.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace core { namespace seproxy { class SeSelector; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace core { namespace seproxy { namespace message { class ApduRequest; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace core { namespace seproxy { namespace protocol { class SeProtocol; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace core { namespace seproxy { namespace message { class SeRequest; } } } } } }

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace transaction {

using ChannelState  = org::eclipse::keyple::core::seproxy::ChannelState;
using SeSelector    = org::eclipse::keyple::core::seproxy::SeSelector;
using ApduRequest   = org::eclipse::keyple::core::seproxy::message::ApduRequest;
using SeRequest     = org::eclipse::keyple::core::seproxy::message::SeRequest;
using SeProtocol    = org::eclipse::keyple::core::seproxy::protocol::SeProtocol;
using Logger        = org::eclipse::keyple::common::Logger;
using LoggerFactory = org::eclipse::keyple::common::LoggerFactory;

/**
    * The SeSelectionRequest class combines a SeSelector with additional helper methods useful to the
    * selection process done in {@link SeSelection}.
    * <p>
    * This class may also be extended to add particular features specific to a SE family.
    */
class EXPORT SeSelectionRequest : public std::enable_shared_from_this<SeSelectionRequest> {
private:
    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(SeSelectionRequest));

    std::shared_ptr<SeSelector> seSelector;
    std::type_index matchingClass;
    std::type_index selectionClass;
    /** optional apdu requests list to be executed following the selection process */
    std::vector<std::shared_ptr<ApduRequest>> seSelectionApduRequestList = std::vector<std::shared_ptr<ApduRequest>>();
    /**
        * the channelState and protocolFlag may be accessed from derived classes. Let them with the
        * protected access level.
        */
protected:
    const ChannelState channelState;
    const std::shared_ptr<SeProtocol> protocolFlag;

public:
    SeSelectionRequest(std::shared_ptr<SeSelector> seSelector, ChannelState channelState, std::shared_ptr<SeProtocol> protocolFlag);

    /**
        * Returns a selection SeRequest built from the information provided in the constructor and
        * possibly completed with the seSelectionApduRequestList
        *
        * @return the selection SeRequest
        */
public:
    std::shared_ptr<SeRequest> getSelectionRequest();

public:
    virtual std::shared_ptr<SeSelector> getSeSelector();

    /**
        * The matchingClass is the MatchingSe class or one of its extensions
        * <p>
        * It is used in SeSelection to determine what kind of MatchingSe is to be instantiated.
        *
        * This method must be called in the classes that extend SeSelector in order to specify the
        * expected class derived from MatchingSe in return to the selection process.
        *
        * @param matchingClass the expected class for this SeSelector
        */
protected:
    void setMatchingClass(const std::type_info& matchingClass);

    /**
        * The selectionClass is the SeSelector class or one of its extensions
        * <p>
        * It is used in SeSelection to determine what kind of SeSelector is to be used as argument to
        * the matchingClass constructor.
        *
        * This method must be called in the classes that extend SeSelector in order to specify the
        * expected class derived from SeSelector used as an argument to derived form of MatchingSe.
        *
        * @param selectionClass the argument for the constructor of the matchingClass
        */
    void setSelectionClass(const std::type_info& selectionClass);

public:
    /**
        * The default value for the selectionClass (unless setSelectionClass is used) is
        * SeSelector.class
        * 
        * @return the current selectionClass
        */
    std::type_index& getSelectionClass();

    /**
        * The default value for the matchingClass (unless setMatchingClass is used) is MatchingSe.class
        *
        * @return the current matchingClass
        */
    std::type_index& getMatchingClass();
protected:
    /**
        * Add an additional {@link ApduRequest} to be executed after the selection process if it
        * succeeds.
        * <p>
        * If more than one {@link ApduRequest} is added, all will be executed in the order in which
        * they were added.
        * 
        * @param apduRequest an {@link ApduRequest}
        */
    void addApduRequest(std::shared_ptr<ApduRequest> apduRequest);

public:
    std::string toString();
};

}
}
}
}
}
