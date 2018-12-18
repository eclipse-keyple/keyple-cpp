#pragma once

#include <string>
#include <vector>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleBaseException; } } } } }

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


                        using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;

                        class Demo_CalypsoClassic_Pcsc : public std::enable_shared_from_this<Demo_CalypsoClassic_Pcsc> {
                            /**
                             * This object is used to freeze the main thread while card operations are handle through the
                             * observers callbacks. A call to the notify() method would end the program (not demonstrated
                             * here).
                             */
                        private:
                            static const std::shared_ptr<void> waitForEnd;

                            /**
                             * main program entry
                             *
                             * @param args the program arguments
                             * @throws KeypleBaseException setParameter exception
                             * @throws InterruptedException thread exception
                             */
                            static void main(std::vector<std::string> &args) throw(KeypleBaseException, InterruptedException);
                        };

                    }
                }
            }
        }
    }
}
