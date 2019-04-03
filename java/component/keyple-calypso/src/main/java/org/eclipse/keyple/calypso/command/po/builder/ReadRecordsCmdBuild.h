#pragma once

#include "../PoCommandBuilder.h"
#include "../PoSendableInSession.h"
#include "../CalypsoPoCommands.h"
#include "../../PoClass.h"
#include <string>
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
                            using CalypsoPoCommands = org::eclipse::keyple::calypso::command::po::CalypsoPoCommands;
                            using PoCommandBuilder = org::eclipse::keyple::calypso::command::po::PoCommandBuilder;
                            using PoSendableInSession = org::eclipse::keyple::calypso::command::po::PoSendableInSession;

                            /**
                             * The Class ReadRecordsCmdBuild. This class provides the dedicated constructor to build the Read
                             * Records APDU command.
                             */
                            class ReadRecordsCmdBuild final : public PoCommandBuilder, public PoSendableInSession {

                                /** The command. */
                            private:
                                const CalypsoPoCommands command = CalypsoPoCommands::READ_RECORDS;

                                /**
                                 * Instantiates a new read records cmd build.
                                 *
                                 * @param poClass indicates which CLA byte should be used for the Apdu
                                 * @param sfi the sfi top select
                                 * @param firstRecordNumber the record number to read (or first record to read in case of
                                 *        several records)
                                 * @param readJustOneRecord the read just one record
                                 * @param expectedLength the expected length of the record(s)
                                 * @param extraInfo extra information included in the logs (can be null or empty)
                                 * @throws java.lang.IllegalArgumentException - if record number &lt; 1
                                 * @throws java.lang.IllegalArgumentException - if the request is inconsistent
                                 */
                            public:
                                ReadRecordsCmdBuild(PoClass poClass, char sfi, char firstRecordNumber, bool readJustOneRecord, char expectedLength, const std::string &extraInfo) throw(std::invalid_argument);

                                /**
                                 * Instantiates a new read records cmd build without specifying the expected length. This
                                 * constructor is allowed only in contactless mode.
                                 *
                                 * @param poClass indicates which CLA byte should be used for the Apdu
                                 * @param sfi the sfi top select
                                 * @param firstRecordNumber the record number to read (or first record to read in case of
                                 *        several records)
                                 * @param readJustOneRecord the read just one record
                                 * @param extraInfo extra information included in the logs (can be null or empty)
                                 * @throws java.lang.IllegalArgumentException - if record number &lt; 1
                                 * @throws java.lang.IllegalArgumentException - if the request is inconsistent
                                 */
                                ReadRecordsCmdBuild(PoClass poClass, char sfi, char firstRecordNumber, bool readJustOneRecord, const std::string &extraInfo) throw(std::invalid_argument);

                            protected:
                                std::shared_ptr<ReadRecordsCmdBuild> shared_from_this() {
                                    return std::static_pointer_cast<ReadRecordsCmdBuild>(PoCommandBuilder::shared_from_this());
                                }
                            };

                        }
                    }
                }
            }
        }
    }
}
