#pragma once

#include "../../SamCommandBuilder.h"
#include "../../CalypsoSamCommands.h"
#include "../../SamRevision.h"
#include <stdexcept>
#include <memory>
#include "stringhelper.h"

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
                        namespace builder {
                            namespace session {

                                using CalypsoSamCommands = org::eclipse::keyple::calypso::command::sam::CalypsoSamCommands;
                                using SamCommandBuilder = org::eclipse::keyple::calypso::command::sam::SamCommandBuilder;
                                using SamRevision = org::eclipse::keyple::calypso::command::sam::SamRevision;

                                /**
                                 * Builder for the SAM Digest Close APDU command.
                                 */
                                class DigestCloseCmdBuild : public SamCommandBuilder {

                                    /** The command. */
                                private:
                                    const CalypsoSamCommands command = CalypsoSamCommands::DIGEST_CLOSE;

                                    /**
                                     * Instantiates a new DigestCloseCmdBuild .
                                     *
                                     * @param revision of the SAM
                                     * @param expectedResponseLength the expected response length
                                     * @throws java.lang.IllegalArgumentException - if the expected response length is wrong.
                                     */
                                public:
                                    DigestCloseCmdBuild(SamRevision revision, char expectedResponseLength);

protected:
                                    std::shared_ptr<DigestCloseCmdBuild> shared_from_this() {
                                        return std::static_pointer_cast<DigestCloseCmdBuild>(SamCommandBuilder::shared_from_this());
                                    }
                                };

                            }
                        }
                    }
                }
            }
        }
    }
}
