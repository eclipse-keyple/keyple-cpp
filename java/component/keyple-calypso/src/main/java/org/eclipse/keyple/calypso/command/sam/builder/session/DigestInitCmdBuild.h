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
                            namespace session {


                                using CalypsoSamCommands = org::eclipse::keyple::calypso::command::sam::CalypsoSamCommands;
                                using SamCommandBuilder = org::eclipse::keyple::calypso::command::sam::SamCommandBuilder;
                                using SamRevision = org::eclipse::keyple::calypso::command::sam::SamRevision;

                                /**
                                 * Builder for the SAM Digest Init APDU command.
                                 */
                                class DigestInitCmdBuild : public SamCommandBuilder {

                                    /**
                                     * The command.
                                     */
                                private:
                                    const CalypsoSamCommands command = CalypsoSamCommands::DIGEST_INIT;

                                    /**
                                     * Instantiates a new DigestInitCmdBuild.
                                     *
                                     * @param revision of the SAM
                                     * @param verificationMode the verification mode
                                     * @param rev3_2Mode the rev 3 2 mode
                                     * @param workKeyRecordNumber the work key record number
                                     * @param workKeyKif from the AbstractOpenSessionCmdBuild response
                                     * @param workKeyKVC from the AbstractOpenSessionCmdBuild response
                                     * @param digestData all data out from the AbstractOpenSessionCmdBuild response
                                     * @throws java.lang.IllegalArgumentException - if the work key record number
                                     * @throws java.lang.IllegalArgumentException - if the digest data is null
                                     * @throws java.lang.IllegalArgumentException - if the request is inconsistent
                                     */
                                public:
                                    DigestInitCmdBuild(SamRevision revision, bool verificationMode, bool rev3_2Mode, char workKeyRecordNumber, char workKeyKif, char workKeyKVC, std::vector<char> &digestData);

protected:
                                    std::shared_ptr<DigestInitCmdBuild> shared_from_this() {
                                        return std::static_pointer_cast<DigestInitCmdBuild>(SamCommandBuilder::shared_from_this());
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
