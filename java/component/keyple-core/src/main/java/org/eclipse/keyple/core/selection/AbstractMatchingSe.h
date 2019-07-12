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
#include <memory>

#include "TransmissionMode.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace core { namespace seproxy { namespace message { class SeResponse; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace core { namespace seproxy { namespace message { class SelectionStatus; } } } } } }

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace selection {

using SeResponse = org::eclipse::keyple::core::seproxy::message::SeResponse;
using SelectionStatus = org::eclipse::keyple::core::seproxy::message::SelectionStatus;
using TransmissionMode = org::eclipse::keyple::core::seproxy::protocol::TransmissionMode;

/**
    * AbstractMatchingSe is the class to manage the elements of the result of a selection.
    *
    */
class AbstractMatchingSe : public std::enable_shared_from_this<AbstractMatchingSe> {
private:
    const std::shared_ptr<SeResponse> selectionResponse;
    const TransmissionMode transmissionMode;
    std::shared_ptr<SelectionStatus> selectionStatus;
    const std::string selectionExtraInfo;

public:
    /**
     * Constructor.
     */
    AbstractMatchingSe(std::shared_ptr<SeResponse> selectionResponse,
                       TransmissionMode transmissionMode, const std::string &extraInfo);

    /**
     * Indicates whether the current SE has been identified as selected: the logical channel is open
     * and the selection process returned either a FCI or an ATR
     * 
     * @return true or false
     */
    bool isSelected();

    /**
     * @return the SE {@link SelectionStatus}
     */
    virtual std::shared_ptr<SelectionStatus> getSelectionStatus();

    /**
     * @return the SE {@link TransmissionMode} (contacts or contactless)
     */
    virtual TransmissionMode getTransmissionMode();

    /**
     * @return the selection extra info string
     */
    virtual std::string getSelectionExtraInfo();
};

}
}
}
}
}
