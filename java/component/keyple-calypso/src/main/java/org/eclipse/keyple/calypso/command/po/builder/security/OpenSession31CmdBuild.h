#pragma once

#include "AbstractOpenSessionCmdBuild.h"
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
                            namespace security {



                                class OpenSession31CmdBuild final : public AbstractOpenSessionCmdBuild {
                                    /**
                                     * Instantiates a new AbstractOpenSessionCmdBuild.
                                     *
                                     * @param keyIndex the key index
                                     * @param samChallenge the sam challenge returned by the SAM Get Challenge APDU command
                                     * @param sfiToSelect the sfi to select
                                     * @param recordNumberToRead the record number to read
                                     * @param extraInfo extra information included in the logs (can be null or empty)
                                     * @throws java.lang.IllegalArgumentException - if the request is inconsistent
                                     */
                                public:
                                    OpenSession31CmdBuild(char keyIndex, std::vector<char> &samChallenge, char sfiToSelect, char recordNumberToRead, const std::string &extraInfo);

protected:
                                    std::shared_ptr<OpenSession31CmdBuild> shared_from_this() {
                                        return std::static_pointer_cast<OpenSession31CmdBuild>(AbstractOpenSessionCmdBuild::shared_from_this());
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
