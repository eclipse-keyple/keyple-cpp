#pragma once

#include "../../AbstractSamCommandBuilder.h"
#include "../../CalypsoSamCommands.h"
#include "../../SamRevision.h"
#include <vector>
#include <stdexcept>
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
                    namespace sam {
                        namespace builder {
                            namespace security {

                                using AbstractSamCommandBuilder = org::eclipse::keyple::calypso::command::sam::AbstractSamCommandBuilder;
                                using CalypsoSamCommands = org::eclipse::keyple::calypso::command::sam::CalypsoSamCommands;
                                using SamRevision = org::eclipse::keyple::calypso::command::sam::SamRevision;

                                /**
                                 * Builder for the SAM Give Random APDU command.
                                 */
                                class GiveRandomCmdBuild : public AbstractSamCommandBuilder {

                                    /** The command reference. */
                                private:
                                    static constexpr CalypsoSamCommands command = CalypsoSamCommands::GIVE_RANDOM;

                                    /**
                                     * Instantiates a new DigestUpdateCmdBuild.
                                     *
                                     * @param revision of the SAM
                                     * @param random the random data
                                     * @throws IllegalArgumentException - if the random data is null or has a length not equal to 8
                                     *
                                     *         TODO implement specific settings for rev < 3
                                     */
                                public:
                                    GiveRandomCmdBuild(SamRevision revision, std::vector<char> &random);

protected:
                                    std::shared_ptr<GiveRandomCmdBuild> shared_from_this() {
                                        return std::static_pointer_cast<GiveRandomCmdBuild>(org.eclipse.keyple.calypso.command.sam.AbstractSamCommandBuilder::shared_from_this());
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
