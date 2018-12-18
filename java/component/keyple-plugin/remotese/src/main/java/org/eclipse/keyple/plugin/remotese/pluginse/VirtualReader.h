#pragma once

#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/plugin/AbstractObservableReader.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/TransmissionMode.h"
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace pluginse { class VirtualReaderSession; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class RemoteMethodTxEngine; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace protocol { class SeProtocolSetting; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace @event { class ReaderEvent; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace transaction { class SelectionRequest; } } } }

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
            namespace plugin {
                namespace remotese {
                    namespace pluginse {

                        using RemoteMethodTxEngine = org::eclipse::keyple::plugin::remotese::transport::RemoteMethodTxEngine;
                        using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using namespace org::eclipse::keyple::seproxy::message;
                        using AbstractObservableReader = org::eclipse::keyple::seproxy::plugin::AbstractObservableReader;
                        using SeProtocolSetting = org::eclipse::keyple::seproxy::protocol::SeProtocolSetting;
                        using TransmissionMode = org::eclipse::keyple::seproxy::protocol::TransmissionMode;
                        using SelectionRequest = org::eclipse::keyple::transaction::SelectionRequest;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        /**
                         * Virtual Reader Behaves like the Remote Reader it emulates
                         */
                        class VirtualReader final : public AbstractObservableReader {

                        private:
                            const std::shared_ptr<VirtualReaderSession> session;
                            const std::string remoteName;
                            const std::shared_ptr<RemoteMethodTxEngine> rmTxEngine;

                            static const std::shared_ptr<Logger> logger;

                            /**
                             * Called by {@link RemoteSePlugin} Creates a new virtual reader
                             * 
                             * @param session Reader Session that helps communicate with
                             *        {@link org.eclipse.keyple.plugin.remotese.transport.TransportNode}
                             * @param nativeReaderName local name of the native reader on slave side
                             */
                        public:
                            VirtualReader(std::shared_ptr<VirtualReaderSession> session, const std::string &nativeReaderName, std::shared_ptr<RemoteMethodTxEngine> rmTxEngine);

                            /**
                             * TODO change this to handle the right transmission mode
                             *
                             * @return the current transmission mode
                             */
                            TransmissionMode getTransmissionMode();

                            /**
                             * Name of the Native Reader
                             *
                             * @return local name of the native reader (on slave device)
                             */
                            std::string getNativeReaderName();

                            std::shared_ptr<VirtualReaderSession> getSession();

                            std::shared_ptr<RemoteMethodTxEngine> getRmTxEngine();


                            bool isSePresent() override;

                            /**
                             * Blocking TransmitSet
                             * 
                             * @param seRequestSet : SeRequestSet to be transmitted
                             * @return seResponseSet : SeResponseSet
                             * @throws IllegalArgumentException
                             * @throws KeypleReaderException
                             */
                            std::shared_ptr<SeResponseSet> processSeRequestSet(std::shared_ptr<SeRequestSet> seRequestSet) throw(std::invalid_argument, KeypleReaderException) override;

                            /**
                             * Blocking Transmit
                             * 
                             * @param seRequest
                             * @return seResponse
                             * @throws IllegalArgumentException
                             * @throws KeypleReaderException
                             */
                            std::shared_ptr<SeResponse> processSeRequest(std::shared_ptr<SeRequest> seRequest) throw(std::invalid_argument, KeypleReaderException) override;


                            void addSeProtocolSetting(std::shared_ptr<SeProtocolSetting> seProtocolSetting) override;

                            /*
                             * PACKAGE PRIVATE
                             */

                            /**
                             * When an event occurs on the Remote LocalReader, notify Observers
                             * 
                             * @param event
                             */
                            void onRemoteReaderEvent(std::shared_ptr<ReaderEvent> event_Renamed);

                        private:
                            class ThreadAnonymousInnerClass : public Thread {
                            private:
                                std::shared_ptr<VirtualReader> outerInstance;

                                std::shared_ptr<ReaderEvent> event_Renamed;
                                std::shared_ptr<org::eclipse::keyple::plugin::remotese::pluginse::VirtualReader> thisReader;

                            public:
                                ThreadAnonymousInnerClass(std::shared_ptr<VirtualReader> outerInstance, std::shared_ptr<ReaderEvent> event_Renamed, std::shared_ptr<org::eclipse::keyple::plugin::remotese::pluginse::VirtualReader> thisReader);

                                void run();

protected:
                                std::shared_ptr<ThreadAnonymousInnerClass> shared_from_this() {
                                    return std::static_pointer_cast<ThreadAnonymousInnerClass>(Thread::shared_from_this());
                                }
                            };


                            /**
                             *
                             * HELPERS
                             */


                        public:
                            std::unordered_map<std::string, std::string> getParameters() override;

                            void setParameter(const std::string &key, const std::string &value) throw(std::invalid_argument) override;

                            void setDefaultSelectionRequest(std::shared_ptr<SelectionRequest> selectionRequest, NotificationMode notificationMode) override;



protected:
                            std::shared_ptr<VirtualReader> shared_from_this() {
                                return std::static_pointer_cast<VirtualReader>(org.eclipse.keyple.seproxy.plugin.AbstractObservableReader::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
