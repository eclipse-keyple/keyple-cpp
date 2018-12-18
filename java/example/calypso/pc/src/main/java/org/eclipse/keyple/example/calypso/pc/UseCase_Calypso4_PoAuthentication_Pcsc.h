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
                        using namespace org::eclipse::keyple::transaction;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        /**
                         * <h1>Use Case ‘Calypso 4’ – PO Authentication (PC/SC)</h1>
                         * <ul>
                         * <li>
                         * <h2>Scenario:</h2>
                         * <ul>
                         * <li>Check if a ISO 14443-4 SE is in the reader, select a Calypso PO, operate a simple Calypso PO
                         * authentication (open and close a secure session performed with the debit key).
                         * <p>
                         * The SAM messages are handled transparently by the Calypso transaction API.</li>
                         * <li><code>
                         Explicit Selection
                         </code> means that it is the terminal application which start the SE processing.</li>
                         * <li>4 PO messages:
                         * <ul>
                         * <li>1 - SE message to explicitly select the application in the reader</li>
                         * <li>2 - transaction SE message to operate the session opening and a PO read</li>
                         * <li>3 - transaction SE message to operate the reading of a file</li>
                         * <li>4 - transaction SE message to operate the closing opening</li>
                         * </ul>
                         * </li>
                         * </ul>
                         * </li>
                         * </ul>
                         */
                        class UseCase_Calypso4_PoAuthentication_Pcsc : public std::enable_shared_from_this<UseCase_Calypso4_PoAuthentication_Pcsc> {
                        private:
                            static const std::shared_ptr<Logger> logger;

                            static void main(std::vector<std::string> &args) throw(KeypleBaseException, NoStackTraceThrowable);
                        };

                    }
                }
            }
        }
    }
}
