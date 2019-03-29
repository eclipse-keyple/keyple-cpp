#pragma once

#include <string>
#include <vector>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleBaseException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class NoStackTraceThrowable; } } } } }

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
                namespace generic_Renamed {
                    namespace pc {


                        using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                        using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
                        using namespace org::eclipse::keyple::transaction;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;


                        /**
                         * <h1>Use Case ‘generic 1’ – Explicit Selection Aid (PC/SC)</h1>
                         * <ul>
                         * <li>
                         * <h2>Scenario:</h2>
                         * <ul>
                         * <li>Check if a ISO 14443-4 SE is in the reader, select a SE (here a Calypso PO).</li>
                         * <li><code>
                         Explicit Selection
                         </code> means that it is the terminal application which start the SE processing.</li>
                         * <li>SE messages:
                         * <ul>
                         * <li>A single SE message to select the application in the reader</li>
                         * </ul>
                         * </li>
                         * </ul>
                         * </li>
                         * </ul>
                         */
                        class UseCase_Generic1_ExplicitSelectionAid_Pcsc : public std::enable_shared_from_this<UseCase_Generic1_ExplicitSelectionAid_Pcsc> {
                        protected:
                            static const std::shared_ptr<Logger> logger;
                        private:
                            static std::string seAid; // Here a Calypso AID


                            static void main(std::vector<std::string> &args) throw(KeypleBaseException, InterruptedException, IOException, NoStackTraceThrowable);
                        };

                    }
                }
            }
        }
    }
}
