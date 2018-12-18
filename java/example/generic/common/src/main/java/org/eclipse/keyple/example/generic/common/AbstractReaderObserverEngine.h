#pragma once

#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ObservableReader.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace transaction { class SelectionResponse; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace @event { class ReaderEvent; } } } } }

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
                    namespace common {


                        using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                        using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                        using SelectionResponse = org::eclipse::keyple::transaction::SelectionResponse;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        /**
                         * This abstract class is intended to be extended by the applications classes in which the SE
                         * insertion, selection, removal is factorized here.
                         */
                        class AbstractReaderObserverEngine : public std::enable_shared_from_this<AbstractReaderObserverEngine>, public ObservableReader::ReaderObserver {

                        private:
                            static std::shared_ptr<Logger> logger;


                        protected:
                            virtual void processSeMatch(std::shared_ptr<SelectionResponse> selectionResponse) = 0;

                            virtual void processSeInsertion() = 0; // alternative AID selection

                            virtual void processSeRemoval() = 0;

                            virtual void processUnexpectedSeRemoval() = 0;


                        public:
                            bool currentlyProcessingSe = false;


                            virtual void update(std::shared_ptr<ReaderEvent> event_Renamed);
                        };

                    }
                }
            }
        }
    }
}
