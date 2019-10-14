#pragma once

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
            namespace example {
                namespace remote {
                    namespace calypso {

                        using namespace org::eclipse::keyple::seproxy::message;
                        using org::slf4j::Logger;

                        class CommandSample : public std::enable_shared_from_this<CommandSample> {



                        public:
                            static void transmit(std::shared_ptr<Logger> logger, const std::string &remoteReaderName);
                        };

                    }
                }
            }
        }
    }
}
