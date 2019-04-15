#pragma once

#include "../PoCommandBuilder.h"
#include "../PoModificationCommand.h"
#include "../PoSendableInSession.h"
#include "../CalypsoPoCommands.h"
#include "../../PoClass.h"
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


                            using PoClass = org::eclipse::keyple::calypso::command::PoClass;
                            using namespace org::eclipse::keyple::calypso::command::po;

                            /**
                             * The Class IncreaseCmdBuild. This class provides the dedicated constructor to build the Increase
                             * APDU command.
                             *
                             */
                            class IncreaseCmdBuild final : public PoCommandBuilder, public PoSendableInSession, public PoModificationCommand {

                                /** The command. */
                            private:
                                const CalypsoPoCommands command = CalypsoPoCommands::INCREASE;

                                /**
                                 * Instantiates a new increase cmd build from command parameters.
                                 *
                                 * @param poClass indicates which CLA byte should be used for the Apdu
                                 * @param counterNumber &gt;= 01h: Counters file, number of the counter. 00h: Simulated Counter
                                 *        file.
                                 * @param sfi SFI of the file to select or 00h for current EF
                                 * @param incValue Value to add to the counter (defined as a positive int &lt;= 16777215
                                 *        [FFFFFFh])
                                 * @param extraInfo extra information included in the logs (can be null or empty)
                                 * @throws java.lang.IllegalArgumentException - if the decrement value is out of range
                                 * @throws java.lang.IllegalArgumentException - if the command is inconsistent
                                 */
                            public:
                                IncreaseCmdBuild(PoClass poClass, char sfi, char counterNumber, int incValue, const std::string &extraInfo);

                            protected:
                                std::shared_ptr<IncreaseCmdBuild> shared_from_this() {
                                    return std::static_pointer_cast<IncreaseCmdBuild>(PoCommandBuilder::shared_from_this());
                                }
                            };

                        }
                    }
                }
            }
        }
    }
}
