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

                            // TODO: Auto-generated Javadoc
                            /**
                             * The Class UpdateRecordCmdBuild. This class provides the dedicated constructor to build the Update
                             * Record APDU command.
                             *
                             */
                            class UpdateRecordCmdBuild final : public PoCommandBuilder, public PoSendableInSession, public PoModificationCommand {

                                /** The command. */
                            private:
                                const CalypsoPoCommands command = CalypsoPoCommands::UPDATE_RECORD;

                                /**
                                 * Instantiates a new UpdateRecordCmdBuild.
                                 *
                                 * @param poClass indicates which CLA byte should be used for the Apdu
                                 * @param sfi the sfi to select
                                 * @param recordNumber the record number to update
                                 * @param newRecordData the new record data to write
                                 * @param extraInfo extra information included in the logs (can be null or empty)
                                 * @throws java.lang.IllegalArgumentException - if record number is &lt; 1
                                 * @throws java.lang.IllegalArgumentException - if the request is inconsistent
                                 */
                            public:
                                UpdateRecordCmdBuild(PoClass poClass, char sfi, char recordNumber, std::vector<char> &newRecordData, const std::string &extraInfo) throw(std::invalid_argument);

protected:
                                std::shared_ptr<UpdateRecordCmdBuild> shared_from_this() {
                                    return std::static_pointer_cast<UpdateRecordCmdBuild>(PoCommandBuilder::shared_from_this());
                                }
                            };

                        }
                    }
                }
            }
        }
    }
}
