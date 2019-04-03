#pragma once

#include <string>
#include <vector>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderException; } } } } }
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
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
                        using namespace org::eclipse::keyple::transaction;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        /**
                         * The UseCase_Generic3_GroupedMultiSelection_Pcsc class illustrates the use of the select next
                         * mechanism
                         */
                        class UseCase_Generic4_SequentialMultiSelection_Pcsc : public std::enable_shared_from_this<UseCase_Generic4_SequentialMultiSelection_Pcsc> {
                        protected:
                            static const std::shared_ptr<Logger> logger;

                        private:
                            static void doAndAnalyseSelection(std::shared_ptr<SeSelection> seSelection, std::shared_ptr<MatchingSe> matchingSe, int index) throw(KeypleReaderException);

                            static void main(std::vector<std::string> &args) throw(KeypleBaseException, InterruptedException, IOException, NoStackTraceThrowable);
                        };

                    }
                }
            }
        }
    }
}
