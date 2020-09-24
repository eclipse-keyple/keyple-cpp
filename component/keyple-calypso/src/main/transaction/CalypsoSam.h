/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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
#include "KeypleCalypsoExport.h"
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

class KEYPLECALYPSO_API CalypsoSam : public AbstractMatchingSe {
public:
    /**
     * Constructor.
     *
     * @param selectionResponse the selection response from the SAM
     * @param transmissionMode the current {@link TransmissionMode} (contacts or
     *        contactless)
     */
    CalypsoSam(std::shared_ptr<SeResponse> selectionResponse,
               const TransmissionMode& transmissionMode);

    /**
     *
     */
    virtual ~CalypsoSam() = default;

    /**
     *
     */
    virtual const SamRevision& getSamRevision() const;

    /**
     *
     */
    virtual const std::vector<uint8_t>& getSerialNumber() const;

    /**
     *
     */
    virtual uint8_t getPlatform() const;

    /**
     *
     */
    virtual uint8_t getApplicationType() const;

    /**
     *
     */
    virtual uint8_t getApplicationSubType() const;

    /**
     *
     */
    virtual uint8_t getSoftwareIssuer() const;

    /**
     *
     */
    virtual uint8_t getSoftwareVersion() const;

    /**
     *
     */
    virtual uint8_t getSoftwareRevision() const;

private:
    /**
     *
     */
    const std::shared_ptr<Logger> mLogger =
        LoggerFactory::getLogger(typeid(CalypsoSam));

    /**
     *
     */
    SamRevision mSamRevision;

    /**
     *
     */
    std::vector<uint8_t> mSerialNumber;

    /**
     *
     */
    uint8_t mPlatform = 0;

    /**
     *
     */
    uint8_t mApplicationType = 0;

    /**
     *
     */
    uint8_t mApplicationSubType = 0;

    /**
     *
     */
    uint8_t mSoftwareIssuer = 0;

    /**
     *
     */
    uint8_t mSoftwareVersion = 0;

    /**
     *
     */
    uint8_t mSoftwareRevision = 0;
};

}
}
}
