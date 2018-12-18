#pragma once

#include "../../PoCommandBuilder.h"
#include "../../CalypsoPoCommands.h"
#include "../../../PoClass.h"
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


                                using PoClass = org::eclipse::keyple::calypso::command::PoClass;
                                using CalypsoPoCommands = org::eclipse::keyple::calypso::command::po::CalypsoPoCommands;
                                using PoCommandBuilder = org::eclipse::keyple::calypso::command::po::PoCommandBuilder;

                                // TODO: Auto-generated Javadoc
                                /**
                                 * This class provides the dedicated constructor to build the Close Secure Session APDU command.
                                 */
                                class CloseSessionCmdBuild final : public PoCommandBuilder {

                                    /** The command. */
                                private:
                                    static constexpr CalypsoPoCommands command = CalypsoPoCommands::CLOSE_SESSION;

                                    /**
                                     * Instantiates a new CloseSessionCmdBuild depending of the revision of the PO.
                                     *
                                     * @param poClass indicates which CLA byte should be used for the Apdu
                                     * @param ratificationAsked the ratification asked
                                     * @param terminalSessionSignature the sam half session signature
                                     * @throws java.lang.IllegalArgumentException - if the signature is null or has a wrong length
                                     * @throws java.lang.IllegalArgumentException - if the command is inconsistent
                                     */
                                public:
                                    CloseSessionCmdBuild(PoClass poClass, bool ratificationAsked, std::vector<char> &terminalSessionSignature) throw(std::invalid_argument);

protected:
                                    std::shared_ptr<CloseSessionCmdBuild> shared_from_this() {
                                        return std::static_pointer_cast<CloseSessionCmdBuild>(org.eclipse.keyple.calypso.command.po.PoCommandBuilder::shared_from_this());
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
