#pragma once

#include "../CalypsoBuilderParser.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace calypso { namespace command { namespace sam { class AbstractSamCommandBuilder; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace calypso { namespace command { namespace sam { class AbstractSamResponseParser; } } } } } }

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
                    namespace sam {

                        using CalypsoBuilderParser = org::eclipse::keyple::calypso::command::CalypsoBuilderParser;

                        class SamBuilderParser : public std::enable_shared_from_this<SamBuilderParser>, public CalypsoBuilderParser<std::shared_ptr<AbstractSamCommandBuilder>, std::shared_ptr<AbstractSamResponseParser>> {
                        private:
                            const std::shared_ptr<AbstractSamCommandBuilder> samCommandBuilder;
                            std::shared_ptr<AbstractSamResponseParser> samResponseParser;

                        public:
                            SamBuilderParser(std::shared_ptr<AbstractSamCommandBuilder> samCommandBuilder);

                            virtual std::shared_ptr<AbstractSamCommandBuilder> getCommandBuilder();

                            virtual std::shared_ptr<AbstractSamResponseParser> getResponseParser();

                            virtual void setResponseParser(std::shared_ptr<AbstractSamResponseParser> poResponseParser);
                        };

                    }
                }
            }
        }
    }
}
