#include "RemoteSePlugin.h"
#include "VirtualReaderSessionFactory.h"
#include "../transport/DtoSender.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderNotFoundException.h"
#include "VirtualReader.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ProxyReader.h"
#include "VirtualReaderSession.h"
#include "../rm/RemoteMethodTxEngine.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/PluginEvent.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ReaderEvent.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/plugin/AbstractObservableReader.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace pluginse {
                        using RemoteMethodTxEngine = org::eclipse::keyple::plugin::remotese::rm::RemoteMethodTxEngine;
                        using DtoSender = org::eclipse::keyple::plugin::remotese::transport::DtoSender;
                        using PluginEvent = org::eclipse::keyple::seproxy::event_Renamed::PluginEvent;
                        using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                        using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
                        using AbstractObservablePlugin = org::eclipse::keyple::seproxy::plugin::AbstractObservablePlugin;
                        using AbstractObservableReader = org::eclipse::keyple::seproxy::plugin::AbstractObservableReader;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> RemoteSePlugin::logger = org::slf4j::LoggerFactory::getLogger(RemoteSePlugin::typeid);
const std::string RemoteSePlugin::PLUGIN_NAME = "RemoteSePlugin";

                        RemoteSePlugin::RemoteSePlugin(std::shared_ptr<VirtualReaderSessionFactory> sessionManager, std::shared_ptr<DtoSender> sender) : org::eclipse::keyple::seproxy::plugin::AbstractObservablePlugin(PLUGIN_NAME), sessionManager(sessionManager), sender(sender), parameters(std::unordered_map<std::string, std::string>()) {
                            logger->info("Init RemoteSePlugin");
                        }

                        std::shared_ptr<VirtualReader> RemoteSePlugin::getReaderByRemoteName(const std::string &remoteName) throw(KeypleReaderNotFoundException) {
                            for (auto virtualReader : readers) {
                                if ((std::static_pointer_cast<VirtualReader>(virtualReader))->getNativeReaderName() == remoteName) {
                                    return std::static_pointer_cast<VirtualReader>(virtualReader);
                                }
                            }
                            throw std::make_shared<KeypleReaderNotFoundException>(remoteName);
                        }

                        std::shared_ptr<ProxyReader> RemoteSePlugin::createVirtualReader(const std::string &clientNodeId, const std::string &nativeReaderName, std::shared_ptr<DtoSender> dtoSender) throw(KeypleReaderException) {
                            logger->debug("createVirtualReader for nativeReader {}", nativeReaderName);

                            // create a new session for the new reader
                            std::shared_ptr<VirtualReaderSession> session = sessionManager->createSession(nativeReaderName, clientNodeId);

                            try {
                                if (getReaderByRemoteName(nativeReaderName) != nullptr) {
                                    throw std::make_shared<KeypleReaderException>("Virtual Reader already exists for reader " + nativeReaderName);
                                };
                            }
                            catch (const KeypleReaderNotFoundException &e) {
                                // no reader found, continue
                            }


                            // check if reader is not already connected (by localReaderName)
                            logger->info("Create a new Virtual Reader with localReaderName {} with session {}", nativeReaderName, session->getSessionId());

                            // Create virtual reader with a remote method engine so the reader can send dto
                            // with a session
                            // and the provided name
                            std::shared_ptr<VirtualReader> * const virtualReader = std::make_shared<VirtualReader>(session, nativeReaderName, std::make_shared<RemoteMethodTxEngine>(sender));
                            readers->add(virtualReader);

                            // notify that a new reader is connected in a separated thread
                            /*
                             * new Thread() { public void run() { } }.start();
                             */
                            notifyObservers(std::make_shared<PluginEvent>(getName(), virtualReader->getName(), PluginEvent::EventType::READER_CONNECTED));

                            return virtualReader;

                        }

                        void RemoteSePlugin::disconnectRemoteReader(const std::string &nativeReaderName) throw(KeypleReaderNotFoundException) {

                            logger->debug("Disconnect Virtual reader {}", nativeReaderName);

                            // retrieve virtual reader to delete
                            std::shared_ptr<VirtualReader> * const virtualReader = std::static_pointer_cast<VirtualReader>(this->getReaderByRemoteName(nativeReaderName));

                            logger->info("Disconnect VirtualReader with name {} with session {}", nativeReaderName);

                            // remove observers of reader
                            virtualReader->clearObservers();

                            // remove reader
                            readers->remove(virtualReader);

                            // send event READER_DISCONNECTED in a separate thread
                            // new Thread() {public void run() { }}.start();

                            notifyObservers(std::make_shared<PluginEvent>(getName(), virtualReader->getName(), PluginEvent::EventType::READER_DISCONNECTED));

                        }

                        void RemoteSePlugin::onReaderEvent(std::shared_ptr<ReaderEvent> event_Renamed, const std::string &sessionId) {
                            logger->debug("OnReaderEvent {}", event_Renamed);
                            logger->debug("Dispatch ReaderEvent to the appropriate Reader : {} sessionId : {}", event_Renamed->getReaderName(), sessionId);
                            try {
                                std::shared_ptr<VirtualReader> virtualReader = std::static_pointer_cast<VirtualReader>(getReaderByRemoteName(event_Renamed->getReaderName()));
                                virtualReader->onRemoteReaderEvent(event_Renamed);

                            }
                            catch (const KeypleReaderNotFoundException &e) {
                                e->printStackTrace();
                            }

                        }

                        std::shared_ptr<SortedSet<std::shared_ptr<AbstractObservableReader>>> RemoteSePlugin::initNativeReaders() throw(KeypleReaderException) {
                            return std::set<std::shared_ptr<AbstractObservableReader>>();
                        }

                        std::shared_ptr<AbstractObservableReader> RemoteSePlugin::fetchNativeReader(const std::string &name) throw(KeypleReaderException) {
                            // should not be call
                            throw std::invalid_argument("fetchNativeReader is not used in this plugin, did you meant to use getReader?");
                        }

                        void RemoteSePlugin::startObservation() {

                        }

                        void RemoteSePlugin::stopObservation() {

                        }

                        std::unordered_map<std::string, std::string> RemoteSePlugin::getParameters() {
                            return parameters;
                        }

                        void RemoteSePlugin::setParameter(const std::string &key, const std::string &value) throw(std::invalid_argument) {
                            parameters.emplace(key, value);
                        }
                    }
                }
            }
        }
    }
}
