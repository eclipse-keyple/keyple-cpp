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
#include "SamRevision_Import.h"

/* Common */
#include "Export.h"
#include "exceptionhelper.h"
#include "LoggerFactory.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::core::selection;
using namespace keyple::calypso::command::sam;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::selection;
using namespace keyple::common;
using namespace keyple::core::seproxy::protocol;

class EXPORT CalypsoSam : public AbstractMatchingSe {
public:
    /**
     * Constructor.
     *
     * @param selectionResponse the selection response from the SAM
     * @param transmissionMode the current {@link TransmissionMode} (contacts or contactless)
     * @param extraInfo textual information
     */
    CalypsoSam(std::shared_ptr<SeResponse> selectionResponse,
               TransmissionMode transmissionMode, const std::string& extraInfo);

    /**
     *
     */
    virtual ~CalypsoSam()
    {
    }

    /**
     *
     */
    virtual SamRevision getSamRevision();

    /**
     *
     */
    virtual std::vector<uint8_t> getSerialNumber();

    /**
     *
     */
    virtual uint8_t getPlatform();

    /**
     *
     */
    virtual uint8_t getApplicationType();

    /**
     *
     */
    virtual uint8_t getApplicationSubType();

    /**
     *
     */
    virtual uint8_t getSoftwareIssuer();

    /**
     *
     */
    virtual uint8_t getSoftwareVersion();

    /**
     *
     */
    virtual uint8_t getSoftwareRevision();

protected:
    /**
     *
     */
    std::shared_ptr<CalypsoSam> shared_from_this()
    {
        return std::static_pointer_cast<CalypsoSam>(
            AbstractMatchingSe::shared_from_this());
    }

private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(CalypsoSam));

    /**
     *
     */
    SamRevision samRevision;

    /**
     *
     */
    std::vector<uint8_t> serialNumber = std::vector<uint8_t>(4);

    /**
     *
     */
    uint8_t platform = 0;

    /**
     *
     */
    uint8_t applicationType = 0;

    /**
     *
     */
    uint8_t applicationSubType = 0;

    /**
     *
     */
    uint8_t softwareIssuer = 0;

    /**
     *
     */
    uint8_t softwareVersion = 0;

    /**
     *
     */
    uint8_t softwareRevision = 0;
};

}
}
}
