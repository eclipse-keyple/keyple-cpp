#pragma once

#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/command/AbstractIso7816CommandBuilder.h"
#include "CalypsoPoCommands.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class ApduRequest; } } } } }

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

                        using AbstractIso7816CommandBuilder = org::eclipse::keyple::command::AbstractIso7816CommandBuilder;
                        using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;

                        /**
                         * Superclass for all PO command builders.
                         * <p>
                         * Used directly, this class can serve as low level command builder.
                         */
                        class PoCommandBuilder : public AbstractIso7816CommandBuilder {

                            /**
                             * Constructor dedicated for the building of referenced Calypso commands
                             * 
                             * @param reference a command reference from the Calypso command table
                             * @param request the ApduRequest (the instruction byte will be overwritten)
                             */
                        public:
                            PoCommandBuilder(CalypsoPoCommands reference, std::shared_ptr<ApduRequest> request);

protected:
                            std::shared_ptr<PoCommandBuilder> shared_from_this() {
                                return std::static_pointer_cast<PoCommandBuilder>(org.eclipse.keyple.command.AbstractIso7816CommandBuilder::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
