#pragma once

#include "../../PoCommandBuilder.h"
#include "../../PoRevision.h"
#include <string>
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
                    namespace po {
                        namespace builder {
                            namespace session {



                                using PoCommandBuilder = org::eclipse::keyple::calypso::command::po::PoCommandBuilder;
                                using PoRevision = org::eclipse::keyple::calypso::command::po::PoRevision;

                                /**
                                 * The Class AbstractOpenSessionCmdBuild. This class provides the dedicated constructor to build the
                                 * Open Secure Session APDU command.
                                 *
                                 */
                                class AbstractOpenSessionCmdBuild : public PoCommandBuilder {

                                    /**
                                     * Instantiates a new AbstractOpenSessionCmdBuild.
                                     *
                                     * @param revision the revision of the PO
                                     * @throws java.lang.IllegalArgumentException - if the key index is 0 and rev is 2.4
                                     * @throws java.lang.IllegalArgumentException - if the request is inconsistent
                                     */
                                public:
                                    AbstractOpenSessionCmdBuild(PoRevision revision);

                                    static std::shared_ptr<AbstractOpenSessionCmdBuild> create(PoRevision revision, char debitKeyIndex, std::vector<char> &sessionTerminalChallenge, char sfi, char recordNb, const std::string &extraInfo);

protected:
                                    std::shared_ptr<AbstractOpenSessionCmdBuild> shared_from_this() {
                                        return std::static_pointer_cast<AbstractOpenSessionCmdBuild>(org.eclipse.keyple.calypso.command.po.PoCommandBuilder::shared_from_this());
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
