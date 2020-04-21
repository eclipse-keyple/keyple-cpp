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
 * The CalypsoPo class gathers all the information about the current PO
 * retrieved from the response to the select application command.
 * <p>
 * An instance of CalypsoPo can be obtained by casting the AbstractMatchingSe
 * object from the selection process (e.g. (CalypsoPo)
 * matchingSelection.getMatchingSe())
 * <p>
 * The various information contained in CalypsoPo is accessible by getters and
 * includes:
 * <ul>
 * <li>The application identification fields (revision/version, class, DF name,
 * serial number, ATR, issuer)
 * <li>The indication of the presence of optional features (Stored Value, PIN,
 * Rev3.2 mode, ratification management)
 * <li>The management information of the modification buffer
 * <li>The invalidation status
 * </ul>
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
              TransmissionMode transmissionMode, const std::string& extraInfo);

    /**
     * The PO revision indicates the generation of the product presented.
     * <p>
     * It will also have an impact on the internal construction of some commands
     * to take into account the specificities of the different POs.
     *
     * @return an enum giving the identified PO revision
     */
    PoRevision getRevision();

    /**
     * The DF name is the name of the application DF as defined in ISO/IEC
     * 7816-4.
     * <p>
     * It also corresponds to the complete representation of the target covered
     * by the AID value provided in the selection command.
     * <p>
     * The AID selects the application by specifying all or part of the targeted
     * DF Name (5 bytes minimum).
     *
     * @return a byte array containing the DF Name bytes (5 to 16 bytes)
     */
    const std::vector<uint8_t>& getDfName() const;

    /**
     * The serial number for the application, is unique ID for the PO.
     * <p>
     * It is also used for key derivation.
     *
     * @return a byte array containing the Application Serial Number (8 bytes)
     */
    const std::vector<uint8_t>& getApplicationSerialNumber() const;

    /**
     * The Answer To Reset is sent by the PO is ISO7816-3 mode and in
     * contactless mode for PC/SC readers.
     * <p>
     * When the ATR is obtained in contactless mode, it is in fact reconstructed
     * by the reader from information obtained from the lower communication
     * layers. Therefore, it may differ from one reader to another depending on
     * the interpretation that has been made by the manufacturer of the PC/SC
     * standard.
     * <p>
     * This field is not interpreted in the Calypso module.
     *
     * @return a byte array containing the ATR (variable length)
     */
    const std::vector<uint8_t>& getAtr() const;

    /**
     * Specifies whether the change counter allowed in session is established in
     * number of operations or number of bytes modified.
     * <p>
     * This varies depending on the revision of the PO.
     *
     * @return true if the counter is number of bytes
     */
    bool isModificationsCounterInBytes();

    /**
     * Indicates the maximum number of changes allowed in session.
     * <p>
     * This number can be a number of operations or a number of commands (see
     * isModificationsCounterInBytes)
     *
     * @return the maximum number of modifications allowed
     */
    int getModificationsCounter();

    /**
     * This field is directly from the Startup Information zone of the PO.
     * <p>
     * When the modification counter is in number of operations, it is the
     * maximum number of operations allowed.
     * <p>
     * When the modification counter is in bytes, it is used to determine the
     * maximum number of modified bytes allowed. (see the formula in the PO
     * specification)
     *
     * @return the buffer size indicator byte
     */
    char getBufferSizeIndicator();

    /**
     * The buffer size value is the raw interpretation of the buffer size
     * indicator to provide a number of bytes.
     * <p>
     * The revision number must be taken into account at the same time to be
     * accurate.
     * <p>
     * It is better to use getModificationsCounter and
     * isModificationsCounterInBytes
     *
     * @return the buffer size value evaluated from the buffer size indicator
     */
    int getBufferSizeValue();

    /**
     * The platform identification byte is the reference of the chip
     *
     * @return the platform identification byte
     */
    char getPlatformByte();

    /**
     * The Application Type byte determines the Calypso Revision and various
     * options
     *
     * @return the Application Type byte
     */
    char getApplicationTypeByte();

    /**
     * Indicates whether the 3.2 mode is supported or not.
     * <p>
     * This boolean is interpreted from the Application Type byte
     *
     * @return true if the revision 3.2 mode is supported
     */
    bool isRev3_2ModeAvailable();

    /**
     * Indicates if the ratification is done on deselect (ratification command
     * not necessary)
     * <p>
     * This boolean is interpreted from the Application Type byte
     *
     * @return true if the ratification command is required
     */
    bool isRatificationCommandRequired();

    /**
     * Indicates whether the PO has the Calypso Stored Value feature.
     * <p>
     * This boolean is interpreted from the Application Type byte
     *
     * @return true if the PO has the Stored Value feature
     */
    bool hasCalypsoStoredValue();

    /**
     * Indicates whether the PO has the Calypso PIN feature.
     * <p>
     * This boolean is interpreted from the Application Type byte
     *
     * @return true if the PO has the PIN feature
     */
    bool hasCalypsoPin();

    /**
     * The Application Subtype indicates to the terminal a reference to the file
     * structure of the Calypso DF.
     *
     * @return the Application Subtype byte
     */
    char getApplicationSubtypeByte();

    /**
     * The Software Issuer byte indicates the entity responsible for the
     * software of the selected application.
     *
     * @return the Software Issuer byte
     */
    char getSoftwareIssuerByte();

    /**
     * The Software Version field may be set to any fixed value by the Software
     * Issuer of the Calypso application.
     *
     * @return the Software Version byte
     */
    char getSoftwareVersionByte();

    /**
     * The Software Revision field may be set to any fixed value by the Software
     * Issuer of the Calypso application.
     *
     * @return the Software Revision byte
     */
    char getSoftwareRevisionByte();

    /**
     * Indicated whether the PO has been invalidated or not.
     * <p>
     * An invalidated PO has 6283 as status word in response to the Select
     * Application command.
     *
     * @return true if the PO has been invalidated.
     */
    bool isDfInvalidated();

    /**
     * The PO class is the ISO7816 class to be used with the current PO.
     * <p>
     * It determined from the PO revision
     * <p>
     * Two classes are possible: LEGACY and ISO.
     *
     * @return the PO class determined from the PO revision
     */
    PoClass getPoClass();

protected:
    /**
     *
     */
    std::shared_ptr<CalypsoPo> shared_from_this()
    {
        return std::static_pointer_cast<CalypsoPo>(
            AbstractMatchingSe::shared_from_this());
    }

private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(CalypsoPo));

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
    static constexpr int
        REV1_PO_DEFAULT_WRITE_OPERATIONS_NUMBER_SUPPORTED_PER_SESSION = 3;

    /**
     *
     */
    static constexpr int
        REV2_PO_DEFAULT_WRITE_OPERATIONS_NUMBER_SUPPORTED_PER_SESSION = 6;

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
