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
#include "MatchingSe.h"
#include "SeResponse.h"

/* Calypso */
#include "SamRevision.h"
#include "SamSelectionRequest.h"

/* Common */
#include "exceptionhelper.h"
#include "Logger.h"
#include "LoggerFactory.h"


namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace transaction {
                    namespace sam {

                        using SamRevision   = org::eclipse::keyple::calypso::command::sam::SamRevision;
                        using SeResponse    = org::eclipse::keyple::seproxy::message::SeResponse;
                        using MatchingSe    = org::eclipse::keyple::transaction::MatchingSe;
                        using Logger        = org::eclipse::keyple::common::Logger;
                        using LoggerFactory = org::eclipse::keyple::common::LoggerFactory;

                        class CalypsoSam : public MatchingSe {
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
                             * Constructor
                             * 
                             * @param samSelectionRequest the SAM selection request
                             */
                        public:
                            CalypsoSam(std::shared_ptr<SamSelectionRequest> samSelectionRequest);

                            /**
                             * Analyze the ATR contained in the selection status and extract the relevant information.
                             * 
                             * @param selectionResponse the selection SeResponse
                             */
                            void setSelectionResponse(std::shared_ptr<SeResponse> selectionResponse) override;

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
                                return std::static_pointer_cast<CalypsoSam>(MatchingSe::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
