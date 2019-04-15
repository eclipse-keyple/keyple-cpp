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
                                 * Builder for the SAM Digest Update APDU command. This command have to be sent twice for each
                                 * command executed during a session. First time for the command sent and second time for the answer
                                 * received
                                 */
                                class DigestUpdateCmdBuild : public SamCommandBuilder {

                                    /** The command reference. */

                                private:
                                    const CalypsoSamCommands command = CalypsoSamCommands::DIGEST_UPDATE;

                                    /**
                                     * Instantiates a new DigestUpdateCmdBuild.
                                     *
                                     * @param revision of the SAM
                                     * @param encryptedSession the encrypted session
                                     * @param digestData all bytes from command sent by the PO or response from the command
                                     * @throws java.lang.IllegalArgumentException - if the digest data is null or has a length &gt;
                                     *         255
                                     * @throws java.lang.IllegalArgumentException - if the request is inconsistent
                                     */
                                public:
                                    DigestUpdateCmdBuild(SamRevision revision, bool encryptedSession, std::vector<char> &digestData);

protected:
                                    std::shared_ptr<DigestUpdateCmdBuild> shared_from_this() {
                                        return std::static_pointer_cast<DigestUpdateCmdBuild>(SamCommandBuilder::shared_from_this());
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
