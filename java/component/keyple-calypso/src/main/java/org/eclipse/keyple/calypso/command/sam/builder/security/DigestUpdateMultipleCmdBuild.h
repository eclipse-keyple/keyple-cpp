#pragma once

#include "../../SamCommandBuilder.h"
#include "../../CalypsoSamCommands.h"
#include "../../SamRevision.h"
#include <vector>
#include <stdexcept>
#include <memory>

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
                            namespace security {


                                using CalypsoSamCommands = org::eclipse::keyple::calypso::command::sam::CalypsoSamCommands;
                                using SamCommandBuilder = org::eclipse::keyple::calypso::command::sam::SamCommandBuilder;
                                using SamRevision = org::eclipse::keyple::calypso::command::sam::SamRevision;

                                // TODO: Auto-generated Javadoc
                                /**
                                 * This class provides the dedicated constructor to build the SAM Digest Update Multiple APDU
                                 * command.
                                 *
                                 */
                                class DigestUpdateMultipleCmdBuild : public SamCommandBuilder {

                                    /** The command. */
                                private:
                                    const CalypsoSamCommands command = CalypsoSamCommands::DIGEST_UPDATE_MULTIPLE;

                                    /**
                                     * Instantiates a new DigestUpdateMultipleCmdBuild.
                                     *
                                     * @param revision the revision
                                     * @param digestData the digest data
                                     * @throws java.lang.IllegalArgumentException - if the request is inconsistent
                                     */
                                public:
                                    DigestUpdateMultipleCmdBuild(SamRevision revision, std::vector<char> &digestData);

protected:
                                    std::shared_ptr<DigestUpdateMultipleCmdBuild> shared_from_this() {
                                        return std::static_pointer_cast<DigestUpdateMultipleCmdBuild>(SamCommandBuilder::shared_from_this());
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
