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

#include <vector>
#include <memory>

/* Calypso */
#include "MatchingSe.h"
#include "PoRevision.h"
#include "PoClass.h"

/* Common */
#include "exceptionhelper.h"
#include "Logger.h"
#include "LoggerFactory.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace calypso { namespace transaction { class PoSelectionRequest; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class SeResponse; } } } } }


namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace transaction {



                    using PoClass       = org::eclipse::keyple::calypso::command::PoClass;
                    using PoRevision    = org::eclipse::keyple::calypso::command::po::PoRevision;
                    using SeResponse    = org::eclipse::keyple::seproxy::message::SeResponse;
                    using MatchingSe    = org::eclipse::keyple::transaction::MatchingSe;
                    using Logger        = org::eclipse::keyple::common::Logger;
                    using LoggerFactory = org::eclipse::keyple::common::LoggerFactory;

                    /**
                     * The CalypsoPo handles the Calypso SE characteristics such as:
                     * <ul>
                     * <li>revision</li>
                     * <li>serial number</li>
                     * <li>session buffer limit</li>
                     * </ul>
                     * TODO Complete with other PO features from the FCI and/or ATR
                     */
                    class CalypsoPo final : public MatchingSe {
                    private:
                        static const std::shared_ptr<Logger> logger;
                        const char bufferSizeIndicator;
                        const int bufferSizeValue;
                        const char platform;
                        const char applicationType;
//JAVA TO C++ CONVERTER NOTE: Fields cannot have the same name as methods:
                        const bool isRev3_2ModeAvailable_Renamed;
//JAVA TO C++ CONVERTER NOTE: Fields cannot have the same name as methods:
                        const bool isRatificationCommandRequired_Renamed;
//JAVA TO C++ CONVERTER NOTE: Fields cannot have the same name as methods:
                        const bool hasCalypsoStoredValue_Renamed;
//JAVA TO C++ CONVERTER NOTE: Fields cannot have the same name as methods:
                        const bool hasCalypsoPin_Renamed;
                        const char applicationSubtypeByte;
                        const char softwareIssuerByte;
                        const char softwareVersion;
                        const char softwareRevision;
//JAVA TO C++ CONVERTER NOTE: Fields cannot have the same name as methods:
                        const bool isDfInvalidated_Renamed;
                        std::vector<char> applicationSerialNumber;
                        PoRevision revision = static_cast<PoRevision>(0);
                        std::vector<char> dfName;
                        static constexpr int PO_REV1_ATR_LENGTH = 20;
                        static constexpr int REV1_PO_DEFAULT_WRITE_OPERATIONS_NUMBER_SUPPORTED_PER_SESSION = 3;
                        static constexpr int REV2_PO_DEFAULT_WRITE_OPERATIONS_NUMBER_SUPPORTED_PER_SESSION = 6;
                        std::vector<char> poAtr;
                        int modificationsCounterMax = 0;
                        bool modificationCounterIsInBytes = true;

                    public:
                        CalypsoPo(std::shared_ptr<PoSelectionRequest> poSelectionRequest);

                        /**
                         * Constructor.
                         * 
                         * @param selectionResponse the response to the selection application command
                         * @param transmissionMode the current {@link TransmissionMode} (contacts or contactless)
                         * @param extraInfo information string
                         */
                    public:
                        CalypsoPo(std::shared_ptr<SeResponse> selectionResponse, TransmissionMode transmissionMode, const std::string &extraInfo);

                        PoRevision getRevision();

                        std::vector<char> getDfName();

                        std::vector<char> getApplicationSerialNumber();

                        std::vector<char> getAtr();

                        bool isModificationsCounterInBytes();

                        int getModificationsCounter();

                        char getBufferSizeIndicator();

                        int getBufferSizeValue();

                        char getPlatformByte();

                        char getApplicationTypeByte();

                        bool isRev3_2ModeAvailable();

                        bool isRatificationCommandRequired();

                        bool hasCalypsoStoredValue();

                        bool hasCalypsoPin();

                        char getApplicationSubtypeByte();

                        char getSoftwareIssuerByte();

                        char getSoftwareVersionByte();

                        char getSoftwareRevisionByte();

                        bool isDfInvalidated();

                        /**
                         * @return the PO class determined from the PO revision
                         */
                        PoClass getPoClass();

                    protected:
                        void reset() override final;

protected:
                        std::shared_ptr<CalypsoPo> shared_from_this() {
                            return std::static_pointer_cast<CalypsoPo>(AbstractMatchingSe::shared_from_this());
                        }
                    };

                }
            }
        }
    }
}
