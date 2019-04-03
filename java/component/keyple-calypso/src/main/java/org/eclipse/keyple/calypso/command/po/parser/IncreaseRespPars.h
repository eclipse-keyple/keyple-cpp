#pragma once

#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/command/AbstractApduResponseParser.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "exceptionhelper.h"
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
                             * Increase (0032) response parser. See specs: Calypso / page 85 / 9.4.4 Increase
                             */
                            class IncreaseRespPars final : public AbstractApduResponseParser {

                            private:
                                static std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> STATUS_TABLE;

                            private:
                                class StaticConstructor : public std::enable_shared_from_this<StaticConstructor> {
                                public:
                                    StaticConstructor();
                                };

                            private:
                                static IncreaseRespPars::StaticConstructor staticConstructor;



                            protected:
                                std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> getStatusTable() override;

                                /**
                                 * Instantiates a new IncreaseRespPars.
                                 */
                            public:
                                IncreaseRespPars();

                                /**
                                 * Returns the new counter value as an int between 0
                                 *
                                 * @return the new value
                                 * @throws java.lang.IllegalStateException - if the counter value is not available from the
                                 *         command response.
                                 */
                                int getNewValue() throw(IllegalStateException);

                                std::string toString();

protected:
                                std::shared_ptr<IncreaseRespPars> shared_from_this() {
                                    return std::static_pointer_cast<IncreaseRespPars>(AbstractApduResponseParser::shared_from_this());
                                }
                            };

                        }
                    }
                }
            }
        }
    }
}
