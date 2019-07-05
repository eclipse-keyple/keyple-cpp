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

#include <string>
#include <vector>
#include <memory>

/* Core */
#include "AbstractMatchingSe.h"
#include "SeResponse.h"
#include "TransmissionMode.h"

/* Calypso */
#include "SamRevision.h"

/* Common */
#include "exceptionhelper.h"
#include "Logger.h"
#include "LoggerFactory.h"


namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace transaction {

using AbstractMatchingSe = org::eclipse::keyple::core::selection::AbstractMatchingSe;
using SamRevision        = org::eclipse::keyple::calypso::command::sam::SamRevision;
using SeResponse         = org::eclipse::keyple::core::seproxy::message::SeResponse;
using MatchingSe         = org::eclipse::keyple::core::selection::AbstractMatchingSe;
using Logger             = org::eclipse::keyple::common::Logger;
using LoggerFactory      = org::eclipse::keyple::common::LoggerFactory;
using TransmissionMode   = org::eclipse::keyple::core::seproxy::protocol::TransmissionMode;

class CalypsoSam : public AbstractMatchingSe {
private:
    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(CalypsoSam));

    SamRevision samRevision;
    std::vector<char> serialNumber = std::vector<char>(4);
    char platform = 0;
    char applicationType = 0;
    char applicationSubType = 0;
    char softwareIssuer = 0;
    char softwareVersion = 0;
    char softwareRevision = 0;

    /**
    * Constructor.
        * 
    * @param selectionResponse the selection response from the SAM
    * @param transmissionMode the current {@link TransmissionMode} (contacts or contactless)
    * @param extraInfo textual information
        */
public:
CalypsoSam(std::shared_ptr<SeResponse> selectionResponse, TransmissionMode transmissionMode, const std::string &extraInfo);

    virtual SamRevision getSamRevision();

    virtual std::vector<char> getSerialNumber();

    virtual char getPlatform();

    virtual char getApplicationType();

    virtual char getApplicationSubType();

    virtual char getSoftwareIssuer();

    virtual char getSoftwareVersion();

    virtual char getSoftwareRevision();

protected:
    std::shared_ptr<CalypsoSam> shared_from_this() {
        return std::static_pointer_cast<CalypsoSam>(AbstractMatchingSe::shared_from_this());
    }
};

}
}
}
}
}
