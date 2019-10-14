#include "VirtualReader.h"
#include "VirtualReaderSession.h"
#include "../rm/RemoteMethodTxEngine.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeRequestSet.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeResponseSet.h"
#include "method/RmTransmitTx.h"
#include "../exception/KeypleRemoteException.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeRequest.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeResponse.h"
#include "../exception/KeypleRemoteReaderException.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/SeProtocolSetting.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ReaderEvent.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/DefaultSelectionRequest.h"
#include "method/RmSetDefaultSelectionRequestTx.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace pluginse {
                        using KeypleRemoteException = org::eclipse::keyple::plugin::remotese::exception::KeypleRemoteException;
                        using KeypleRemoteReaderException = org::eclipse::keyple::plugin::remotese::exception::KeypleRemoteReaderException;
                        using RmSetDefaultSelectionRequestTx = org::eclipse::keyple::plugin::remotese::pluginse::method::RmSetDefaultSelectionRequestTx;
                        using RmTransmitTx = org::eclipse::keyple::plugin::remotese::pluginse::method::RmTransmitTx;
                        using RemoteMethodTxEngine = org::eclipse::keyple::plugin::remotese::rm::RemoteMethodTxEngine;
                        using TransportNode = org::eclipse::keyple::plugin::remotese::transport::factory::TransportNode;
                        using DefaultSelectionRequest = org::eclipse::keyple::seproxy::event_Renamed::DefaultSelectionRequest;
                        using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using SeRequest = org::eclipse::keyple::seproxy::message::SeRequest;
                        using SeRequestSet = org::eclipse::keyple::seproxy::message::SeRequestSet;
                        using SeResponse = org::eclipse::keyple::seproxy::message::SeResponse;
                        using SeResponseSet = org::eclipse::keyple::seproxy::message::SeResponseSet;
                        using AbstractObservableReader = org::eclipse::keyple::seproxy::plugin::AbstractObservableReader;
                        using SeProtocolSetting = org::eclipse::keyple::seproxy::protocol::SeProtocolSetting;
                        using TransmissionMode = org::eclipse::keyple::seproxy::protocol::TransmissionMode;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> VirtualReader::logger = org::slf4j::LoggerFactory::getLogger(VirtualReader::typeid);

                        VirtualReader::VirtualReader(std::shared_ptr<VirtualReaderSession> session, const std::string &nativeReaderName, std::shared_ptr<RemoteMethodTxEngine> rmTxEngine) : org::eclipse::keyple::seproxy::plugin::AbstractObservableReader(RemoteSePlugin::PLUGIN_NAME, "remote-" + nativeReaderName), session(session), nativeReaderName(nativeReaderName), rmTxEngine(rmTxEngine) {
                            logger->info("A new virtual reader was created with session {}", session);
                        }

                        TransmissionMode VirtualReader::getTransmissionMode() {
                            logger->error("getTransmissionMode is not implemented yet");
                            return nullptr;
                        }

                        std::string VirtualReader::getNativeReaderName() {
                            return nativeReaderName;
                        }

                        std::shared_ptr<VirtualReaderSession> VirtualReader::getSession() {
                            return session;
                        }

                        std::shared_ptr<RemoteMethodTxEngine> VirtualReader::getRmTxEngine() {
                            return rmTxEngine;
                        }

                        bool VirtualReader::isSePresent() {
                            logger->error("isSePresent is not implemented yet");
                            return false; // not implemented
                        }

                        std::shared_ptr<SeResponseSet> VirtualReader::processSeRequestSet(std::shared_ptr<SeRequestSet> seRequestSet) throw(std::invalid_argument, KeypleReaderException) {

                            std::shared_ptr<RmTransmitTx> transmit = std::make_shared<RmTransmitTx>(seRequestSet, session->getSessionId(), this->getNativeReaderName(), this->getName(), session->getSlaveNodeId());
                            try {
                                rmTxEngine->register_Renamed(transmit);
                                return transmit->get();
                            }
                            catch (const KeypleRemoteException &e) {
                                e->printStackTrace();
                                throw std::static_pointer_cast<KeypleReaderException>(e->getCause());
                            }
                        }

                        std::shared_ptr<SeResponse> VirtualReader::processSeRequest(std::shared_ptr<SeRequest> seRequest) throw(std::invalid_argument, KeypleReaderException) {
                            try {
                                return this->processSeRequestSet(std::make_shared<SeRequestSet>(seRequest))->getSingleResponse();
                            }
                            catch (const KeypleRemoteReaderException &e) {
                                // throw the cause of the RemoteReaderException (a KeypleReaderException)
                                throw std::static_pointer_cast<KeypleReaderException>(e->getCause());
                            }
                        }

                        void VirtualReader::startObservation() {

                        }

                        void VirtualReader::stopObservation() {

                        }

                        void VirtualReader::addSeProtocolSetting(std::shared_ptr<SeProtocolSetting> seProtocolSetting) {
                            logger->error("addSeProtocolSetting is not implemented yet");

                        }

                        void VirtualReader::onRemoteReaderEvent(std::shared_ptr<ReaderEvent> event_Renamed) {
                            std::shared_ptr<VirtualReader> * const thisReader = shared_from_this();

                            logger->debug(" EVENT {} ", event_Renamed->getEventType());

                            if (thisReader->countObservers() > 0) {
                                thisReader->notifyObservers(event_Renamed);
                            }
                            else {
                                logger->debug("An event was received but no observers are declared into VirtualReader : {} {}", thisReader->getName(), event_Renamed->getEventType());
                            }

                        }

                        std::unordered_map<std::string, std::string> VirtualReader::getParameters() {
                            logger->error("getParameters is not implemented yet");
                            return nullptr;
                        }

                        void VirtualReader::setParameter(const std::string &key, const std::string &value) throw(std::invalid_argument) {
                            logger->error("setParameter is not implemented yet");
                        }

                        void VirtualReader::setDefaultSelectionRequest(std::shared_ptr<DefaultSelectionRequest> defaultSelectionRequest, NotificationMode notificationMode) {

                            std::shared_ptr<RmSetDefaultSelectionRequestTx> setDefaultSelectionRequest = std::make_shared<RmSetDefaultSelectionRequestTx>(defaultSelectionRequest, notificationMode, this->getNativeReaderName(), this->getName(), this->getSession()->getSessionId(), session->getSlaveNodeId());

                            try {
                                rmTxEngine->register_Renamed(setDefaultSelectionRequest);
                                setDefaultSelectionRequest->get();
                            }
                            catch (const KeypleRemoteException &e) {
                                logger->error("setDefaultSelectionRequest encounters an exception while communicating with slave", e);
                            }
                        }
                    }
                }
            }
        }
    }
}
