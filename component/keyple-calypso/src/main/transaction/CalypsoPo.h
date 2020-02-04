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

#include <vector>
#include <memory>

/* Calypso */
#include "AbstractMatchingSe.h"
#include "PoRevision.h"
#include "PoClass.h"

/* Core */
#include "TransmissionMode.h"

/* Common */
#include "Export.h"
#include "exceptionhelper.h"
#include "Logger.h"
#include "LoggerFactory.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::core::selection;
using namespace keyple::core::seproxy::message;
using namespace keyple::common;
using namespace keyple::core::seproxy::protocol;

/**
 * The CalypsoPo handles the Calypso SE characteristics such as:
 * <ul>
 * <li>revision</li>
 * <li>serial number</li>
 * <li>session buffer limit</li>
 * </ul>
 * TODO Complete with other PO features from the FCI and/or ATR
 */
class EXPORT CalypsoPo final : public AbstractMatchingSe {
public:
    /**
     * Constructor.
     *
     * @param selectionResponse the response to the selection application
     *        command
     * @param transmissionMode the current {@link TransmissionMode} (contacts or
     *        contactless)
     * @param extraInfo information string
     */
    CalypsoPo(std::shared_ptr<SeResponse> selectionResponse,
              TransmissionMode transmissionMode, const std::string &extraInfo);

    /**
     *
     */
    PoRevision getRevision();

    /**
     *
     */
    const std::vector<uint8_t>& getDfName() const;

    /**
     *
     */
    const std::vector<uint8_t>& getApplicationSerialNumber() const;

    /**
     *
     */
    const std::vector<uint8_t>& getAtr() const;

    /**
     *
     */
    bool isModificationsCounterInBytes();

    /**
     *
     */
    int getModificationsCounter();

    /**
     *
     */
    char getBufferSizeIndicator();

    /**
     *
     */
    int getBufferSizeValue();

    /**
     *
     */
    char getPlatformByte();

    /**
     *
     */
    char getApplicationTypeByte();

    /**
     *
     */
    bool isRev3_2ModeAvailable();

    /**
     *
     */
    bool isRatificationCommandRequired();

    /**
     *
     */
    bool hasCalypsoStoredValue();

    /**
     *
     */
    bool hasCalypsoPin();

    /**
     *
     */
    char getApplicationSubtypeByte();

    /**
     *
     */
    char getSoftwareIssuerByte();

    /**
     *
     */
    char getSoftwareVersionByte();

    /**
     *
     */
    char getSoftwareRevisionByte();

    /**
     *
     */
    bool isDfInvalidated();

    /**
     * @return the PO class determined from the PO revision
     */
    PoClass getPoClass();

protected:
    /**
     *
     */
    std::shared_ptr<CalypsoPo> shared_from_this()
    {
        return std::static_pointer_cast<CalypsoPo>(AbstractMatchingSe::shared_from_this());
    }

private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(CalypsoPo));

    /**
     *
     */
    char bufferSizeIndicator;

    /**
     *
     */
    int bufferSizeValue;

    /**
     *
     */
    char platform;

    /**
     *
     */
    char applicationType;

    /**
     *
     */
    bool isRev3_2ModeAvailable_Renamed;

    /**
     *
     */
    bool isRatificationCommandRequired_Renamed;

    /**
     *
     */
    bool hasCalypsoStoredValue_Renamed;

    /**
     *
     */
    bool hasCalypsoPin_Renamed;

    /**
     *
     */
    char applicationSubtypeByte;

    /**
     *
     */
    char softwareIssuerByte;

    /**
     *
     */
    char softwareVersion;

    /**
     *
     */
    char softwareRevision;

    /**
     *
     */
    bool isDfInvalidated_Renamed;

    /**
     *
     */
    std::vector<uint8_t> applicationSerialNumber;

    /**
     *
     */
    PoRevision revision = static_cast<PoRevision>(0);

    /**
     *
     */
    std::vector<uint8_t> dfName;

    /**
     *
     */
    static constexpr int PO_REV1_ATR_LENGTH = 20;

    /**
     *
     */
    static constexpr int REV1_PO_DEFAULT_WRITE_OPERATIONS_NUMBER_SUPPORTED_PER_SESSION = 3;

    /**
     *
     */
    static constexpr int REV2_PO_DEFAULT_WRITE_OPERATIONS_NUMBER_SUPPORTED_PER_SESSION = 6;

    /**
     *
     */
    const std::vector<uint8_t>& poAtr;

    /**
     *
     */
    int modificationsCounterMax = 0;

    /**
     *
     */
    bool modificationCounterIsInBytes = true;

};

}
}
}
