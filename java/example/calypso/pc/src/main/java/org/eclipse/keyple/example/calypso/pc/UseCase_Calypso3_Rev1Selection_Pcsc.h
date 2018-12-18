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
                namespace calypso {
                    namespace pc {


                        using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                        using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        /**
                         * <h1>Use Case ‘Calypso 3’ – Rev1 Selection Atr (PC/SC)</h1>
                         * <ul>
                         * <li>
                         * <h2>Scenario:</h2>
                         * <ul>
                         * <li>Check if a B' protocol SE is in the reader, select a Calypso PO Rev1 (ATR selection), select
                         * the DF RT (ticketing), operate a simple Calypso PO transaction (simple plain read, not involving
                         * a Calypso SAM).</li>
                         * <li><code>
                         Explicit Selection
                         </code> means that it is the terminal application which start the SE processing.</li>
                         * <li>PO messages:
                         * <ul>
                         * <li>A first SE message to do an ATR based selection and DF selection of the SE in the reader</li>
                         * <li>A second SE message to operate the simple Calypso transaction</li>
                         * </ul>
                         * </li>
                         * </ul>
                         * </li>
                         * </ul>
                         */
                        class UseCase_Calypso3_Rev1Selection_Pcsc : public std::enable_shared_from_this<UseCase_Calypso3_Rev1Selection_Pcsc> {
                        protected:
                            static const std::shared_ptr<Logger> logger;
                        private:
                            static std::string poAtrRegex;
                            static std::string poDfRtPath;


                            static void main(std::vector<std::string> &args) throw(KeypleBaseException, InterruptedException, IOException, NoStackTraceThrowable);
                        };

                    }
                }
            }
        }
    }
}
