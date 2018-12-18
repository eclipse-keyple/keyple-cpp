#pragma once

#include "../PoCommandBuilder.h"
#include "../CalypsoPoCommands.h"
#include "../../PoClass.h"
#include <vector>
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

                            using PoClass = org::eclipse::keyple::calypso::command::PoClass;
                            using CalypsoPoCommands = org::eclipse::keyple::calypso::command::po::CalypsoPoCommands;
                            using PoCommandBuilder = org::eclipse::keyple::calypso::command::po::PoCommandBuilder;

                            /**
                             * This class provides the dedicated constructor to build the Select File APDU commands.
                             *
                             */
                            class SelectFileCmdBuild final : public PoCommandBuilder {

                            private:
                                static constexpr CalypsoPoCommands command = CalypsoPoCommands::SELECT_FILE;

                            public:
                                enum class SelectControl {
                                    MF,
                                    PATH_FROM_MF,
                                    PATH_FROM_CURRENT_DF
                                };

                            public:
                                enum class SelectOptions {
                                    FCI,
                                    FCP
                                };

                                /**
                                 * Instantiates a new SelectFileCmdBuild.
                                 *
                                 * @param poClass indicates which CLA byte should be used for the Apdu
                                 */
                            public:
                                SelectFileCmdBuild(PoClass poClass, SelectControl selectControl, SelectOptions selectOptions, std::vector<char> &selectData);

protected:
                                std::shared_ptr<SelectFileCmdBuild> shared_from_this() {
                                    return std::static_pointer_cast<SelectFileCmdBuild>(org.eclipse.keyple.calypso.command.po.PoCommandBuilder::shared_from_this());
                                }
                            };

                        }
                    }
                }
            }
        }
    }
}
