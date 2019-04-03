#pragma once

#include "../PoCommandBuilder.h"
#include "../PoModificationCommand.h"
#include "../PoSendableInSession.h"
#include "../CalypsoPoCommands.h"
#include "../../PoClass.h"
#include <string>
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
                            using namespace org::eclipse::keyple::calypso::command::po;

                            // TODO: Auto-generated Javadoc
                            /**
                             * The Class AppendRecordCmdBuild. This class provides the dedicated constructor to build the Update
                             * Record APDU command.
                             *
                             */
                            class AppendRecordCmdBuild final : public PoCommandBuilder, public PoSendableInSession, public PoModificationCommand {

                                /** The command. */
                            private:
                                const CalypsoPoCommands command = CalypsoPoCommands::APPEND_RECORD;

                                /**
                                 * Instantiates a new UpdateRecordCmdBuild.
                                 *
                                 * @param poClass indicates which CLA byte should be used for the Apdu
                                 * @param sfi the sfi to select
                                 * @param newRecordData the new record data to write
                                 * @param extraInfo extra information included in the logs (can be null or empty)
                                 * @throws java.lang.IllegalArgumentException - if the command is inconsistent
                                 */
                            public:
                                AppendRecordCmdBuild(PoClass poClass, char sfi, std::vector<char> &newRecordData, const std::string &extraInfo);

protected:
                                std::shared_ptr<AppendRecordCmdBuild> shared_from_this() {
                                    return std::static_pointer_cast<AppendRecordCmdBuild>(PoCommandBuilder::shared_from_this());
                                }
                            };

                        }
                    }
                }
            }
        }
    }
}
