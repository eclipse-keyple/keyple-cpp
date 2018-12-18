#pragma once

#include <string>
#include <vector>
#include "exceptionhelper.h"
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
                namespace calypso {
                    namespace pc {

                        using namespace org::eclipse::keyple::example::calypso::pc::stub::se;
                        using namespace org::eclipse::keyple::plugin::stub;

                        class Demo_CalypsoClassic_Stub : public std::enable_shared_from_this<Demo_CalypsoClassic_Stub> {

                            /**
                             * main program entry
                             *
                             * @param args the program arguments
                             * @throws InterruptedException thread exception
                             */
                            static void main(std::vector<std::string> &args) throw(InterruptedException);
                        };

                    }
                }
            }
        }
    }
}
