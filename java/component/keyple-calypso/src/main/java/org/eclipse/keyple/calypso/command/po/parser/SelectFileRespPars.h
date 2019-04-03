#pragma once

#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/command/AbstractApduResponseParser.h"
#include <unordered_map>
#include <vector>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace command { class AbstractApduResponseParser; } } } }
namespace org { namespace eclipse { namespace keyple { namespace command { class StatusProperties; } } } }

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
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace parser {

                            using AbstractApduResponseParser = org::eclipse::keyple::command::AbstractApduResponseParser;

                            /**
                             * This class provides status code properties and the getters to access to the structured fields of
                             * data from response of a Select File response.
                             */
                            class SelectFileRespPars final : public AbstractApduResponseParser {

                            private:
                                static const std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> STATUS_TABLE;

                            private:
                                class StaticConstructor : public std::enable_shared_from_this<StaticConstructor> {
                                public:
                                    StaticConstructor();
                                };

                            private:
                                static SelectFileRespPars::StaticConstructor staticConstructor;


                                /**
                                 * Instantiates a new SelectFileRespPars.
                                 *
                                 */
                            public:
                                SelectFileRespPars();

                                std::vector<char> getSelectionData();

protected:
                                std::shared_ptr<SelectFileRespPars> shared_from_this() {
                                    return std::static_pointer_cast<SelectFileRespPars>(AbstractApduResponseParser::shared_from_this());
                                }
                            };

                        }
                    }
                }
            }
        }
    }
}
