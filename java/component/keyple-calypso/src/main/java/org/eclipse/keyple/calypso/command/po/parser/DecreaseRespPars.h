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
#include <unordered_map>
#include <vector>
#include <memory>

/* Common */
#include "exceptionhelper.h"

/* Core */
#include "AbstractApduResponseParser_Import.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace parser {

                            using AbstractPoResponseParser = org::eclipse::keyple::calypso::command::po::AbstractPoResponseParser;
                            using ApduResponse             = org::eclipse::keyple::core::seproxy::message::ApduResponse;

                            /**
                             * Decrease (0030) response parser. See specs: Calypso / page 83 / 9.4.2 Decrease
                             */
                            class DecreaseRespPars final : public AbstractPoResponseParser {

                            private:
                                static const std::unordered_map<Integer, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> STATUS_TABLE;

                                private:
                                    class StaticConstructor : public std::enable_shared_from_this<StaticConstructor> {
                                    public:
                                        StaticConstructor();
                                    };

                                private:
                                    static DecreaseRespPars::StaticConstructor staticConstructor;


                            protected:
                                std::unordered_map<Integer, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> getStatusTable() override;

                                /**
                                 * Instantiates a new DecreaseRespPars.
                                 */
                            public:
                                DecreaseRespPars(std::shared_ptr<ApduResponse> response);

                                /**
                                 * Returns the new counter value as an int between 0
                                 *
                                 * @return the new value
                                 * @throws IllegalStateException - if the counter value is not available from the command
                                 *         response.
                                 */
                                int getNewValue();

                                std::string toString();

                            protected:
                                std::shared_ptr<DecreaseRespPars> shared_from_this() {
                                    return std::static_pointer_cast<DecreaseRespPars>(AbstractApduResponseParser::shared_from_this());
                                }
                            };

                        }
                    }
                }
            }
        }
    }
}
