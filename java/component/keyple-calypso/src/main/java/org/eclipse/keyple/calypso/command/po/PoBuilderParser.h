#pragma once

#include "../CalypsoBuilderParser.h"
#include "AbstractPoCommandBuilder.h"
#include "AbstractPoResponseParser.h"
#include <memory>

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
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {

                        using CalypsoBuilderParser = org::eclipse::keyple::calypso::command::CalypsoBuilderParser;

                        /**
                         * The PoBuilderParser class contains the builder of a {@link PoSendableInSession} command
                         * <p>
                         * A setter allows to associate the parser object.
                         */
                        template<typename T>
                        class PoBuilderParser : public std::enable_shared_from_this<PoBuilderParser>, public CalypsoBuilderParser<std::shared_ptr<AbstractPoCommandBuilder>, std::shared_ptr<AbstractPoResponseParser>> {
                        private:
                            const std::shared_ptr<AbstractPoCommandBuilder> poCommandBuilder;
                            std::shared_ptr<AbstractPoResponseParser> poResponseParser;

                        public:
                            PoBuilderParser(std::shared_ptr<AbstractPoCommandBuilder> poCommandBuilder) : poCommandBuilder(poCommandBuilder) {
                            }

                            virtual std::shared_ptr<AbstractPoCommandBuilder> getCommandBuilder() {
                                return poCommandBuilder;
                            }

                            virtual std::shared_ptr<AbstractPoResponseParser> getResponseParser() {
                                return poResponseParser;
                            }

                            virtual void setResponseParser(std::shared_ptr<AbstractPoResponseParser> poResponseParser) {
                                this->poResponseParser = poResponseParser;
                            }
                        };

                    }
                }
            }
        }
    }
}
