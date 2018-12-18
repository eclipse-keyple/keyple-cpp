#pragma once

#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/command/AbstractIso7816CommandBuilder.h"
#include "SamRevision.h"
#include <string>
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
                    namespace sam {

                        using AbstractIso7816CommandBuilder = org::eclipse::keyple::command::AbstractIso7816CommandBuilder;
                        using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;

                        /**
                         * Class to build custom (non-referenced) SAM commands
                         */
                        class SamCustomCommandBuilder : public AbstractIso7816CommandBuilder {

                        protected:
                            SamRevision defaultRevision = SamRevision::S1D; // 94

                            /**
                             * Constructor dedicated to the construction of user-defined commands.
                             *
                             * Caveat: the caller has to provide all the command data.
                             *
                             * @param name the name of the command (will appear in the ApduRequest log)
                             * @param request the ApduRequest (the correct instruction byte must be provided)
                             */
                        public:
                            SamCustomCommandBuilder(const std::string &name, std::shared_ptr<ApduRequest> request);

protected:
                            std::shared_ptr<SamCustomCommandBuilder> shared_from_this() {
                                return std::static_pointer_cast<SamCustomCommandBuilder>(org.eclipse.keyple.command.AbstractIso7816CommandBuilder::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
