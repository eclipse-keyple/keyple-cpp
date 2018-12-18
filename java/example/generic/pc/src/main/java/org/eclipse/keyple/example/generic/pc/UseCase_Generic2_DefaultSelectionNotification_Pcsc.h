#pragma once

#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ObservableReader.h"
#include <string>
#include <vector>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class SeReader; } } } }
namespace org { namespace eclipse { namespace keyple { namespace transaction { class SeSelection; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleBaseException; } } } } }
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
                    namespace pc {


                        using SeReader = org::eclipse::keyple::seproxy::SeReader;
                        using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                        using ReaderObserver = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader::ReaderObserver;
                        using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                        using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                        using SeSelection = org::eclipse::keyple::transaction::SeSelection;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        /**
                         * <h1>Use Case ‘generic 2’ – Default Selection Notification (PC/SC)</h1>
                         * <ul>
                         * <li>
                         * <h2>Scenario:</h2>
                         * <ul>
                         * <li>Define a default selection of ISO 14443-4 (here a Calypso PO) and set it to an observable
                         * reader, on SE detection in case the selection is successful, notify the terminal application with
                         * the SE information.</li>
                         * <li><code>
                         Default Selection Notification
                         </code> means that the SE processing is automatically started when detected.</li>
                         * <li>PO messages:
                         * <ul>
                         * <li>A single SE message handled at SeReader level</li>
                         * </ul>
                         * </li>
                         * </ul>
                         * </li>
                         * </ul>
                         */
                        class UseCase_Generic2_DefaultSelectionNotification_Pcsc : public std::enable_shared_from_this<UseCase_Generic2_DefaultSelectionNotification_Pcsc>, public ObservableReader::ReaderObserver {
                        protected:
                            static const std::shared_ptr<Logger> logger;
                        private:
                            std::shared_ptr<SeReader> seReader;
                            std::string seAid = "A0000004040125090101";
                            std::shared_ptr<SeSelection> seSelection;
                            /**
                             * This object is used to freeze the main thread while card operations are handle through the
                             * observers callbacks. A call to the notify() method would end the program (not demonstrated
                             * here).
                             */
                            static const std::shared_ptr<void> waitForEnd;

                        public:
                            UseCase_Generic2_DefaultSelectionNotification_Pcsc() throw(KeypleBaseException, InterruptedException);

                            /**
                             * Method invoked in the case of a reader event
                             * 
                             * @param event the reader event
                             */
                            void update(std::shared_ptr<ReaderEvent> event_Renamed) override;

                            /**
                             * main program entry
                             */
                            static void main(std::vector<std::string> &args) throw(InterruptedException, KeypleBaseException);
                        };

                    }
                }
            }
        }
    }
}
