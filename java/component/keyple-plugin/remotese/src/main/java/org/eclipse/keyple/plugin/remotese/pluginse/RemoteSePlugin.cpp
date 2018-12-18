#include "RemoteSePlugin.h"
#include "VirtualReaderSessionFactory.h"
#include "../transport/DtoSender.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeReader.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderNotFoundException.h"
#include "VirtualReader.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ProxyReader.h"
#include "VirtualReaderSession.h"
#include "VirtualReaderSessionImpl.h"
#include "../transport/RemoteMethodTxEngine.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/PluginEvent.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ReaderEvent.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/plugin/AbstractObservableReader.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace pluginse {
                        using DtoSender = org::eclipse::keyple::plugin::remotese::transport::DtoSender;
                        using RemoteMethodTxEngine = org::eclipse::keyple::plugin::remotese::transport::RemoteMethodTxEngine;
                        using SeReader = org::eclipse::keyple::seproxy::SeReader;
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

                        RemoteSePlugin::RemoteSePlugin(std::shared_ptr<VirtualReaderSessionFactory> sessionManager, std::shared_ptr<DtoSender> sender) : org::eclipse::keyple::seproxy::plugin::AbstractObservablePlugin(PLUGIN_NAME), sessionManager(sessionManager), sender(sender) {
                            logger->info("Init RemoteSePlugin");
                        }

                        std::unordered_map<std::string, std::string> RemoteSePlugin::getParameters() {
                            return nullptr;
                        }

                        void RemoteSePlugin::setParameter(const std::string &key, const std::string &value) throw(std::invalid_argument) {
                        }

                        std::shared_ptr<SeReader> RemoteSePlugin::getReaderByRemoteName(const std::string &remoteName) throw(KeypleReaderNotFoundException) {
                            for (auto virtualReader : readers) {
                                if ((std::static_pointer_cast<VirtualReader>(virtualReader))->getNativeReaderName() == remoteName) {
                                    return virtualReader;
                                }
                            }
                            throw std::make_shared<KeypleReaderNotFoundException>(remoteName);
                        }

                        std::shared_ptr<ProxyReader> RemoteSePlugin::createVirtualReader(const std::string &clientNodeId, const std::string &nativeReaderName, std::shared_ptr<DtoSender> dtoSender) throw(KeypleReaderException) {
                            logger->debug("createVirtualReader for nativeReader {}", nativeReaderName);

                            // create a new session for the new reader
                            std::shared_ptr<VirtualReaderSession> session = sessionManager->createSession(nativeReaderName, clientNodeId);

                            // DtoSender sends Dto when the session requires to
                            (std::static_pointer_cast<VirtualReaderSessionImpl>(session))->addObserver(dtoSender);

                            // check if reader is not already connected (by localReaderName)
                            if (!isReaderConnected(nativeReaderName)) {
                                logger->info("Create a new Virtual Reader with localReaderName {} with session {}", nativeReaderName, session->getSessionId());

                                std::shared_ptr<RemoteMethodTxEngine> rmTxEngine = std::make_shared<RemoteMethodTxEngine>(sender);

                                std::shared_ptr<VirtualReader> * const virtualReader = std::make_shared<VirtualReader>(session, nativeReaderName, rmTxEngine);
                                readers->add(virtualReader);

                                // notify that a new reader is connected in a separated thread
                                std::make_shared<ThreadAnonymousInnerClass>(shared_from_this(), virtualReader)
                                .start();

                                return virtualReader;
                            }
                            else {
                                throw std::make_shared<KeypleReaderException>("Virtual Reader already exists");
                            }
                        }

                        RemoteSePlugin::ThreadAnonymousInnerClass::ThreadAnonymousInnerClass(std::shared_ptr<RemoteSePlugin> outerInstance, std::shared_ptr<org::eclipse::keyple::plugin::remotese::pluginse::VirtualReader> virtualReader) {
                            this->outerInstance = outerInstance;
                            this->virtualReader = virtualReader;
                        }

                        void RemoteSePlugin::ThreadAnonymousInnerClass::run() {
                            outerInstance->notifyObservers(std::make_shared<PluginEvent>(outerInstance->getName(), virtualReader->getName(), PluginEvent::EventType::READER_CONNECTED));
                        }

                        void RemoteSePlugin::disconnectRemoteReader(const std::string &nativeReaderName) throw(KeypleReaderNotFoundException) {
                            logger->debug("Disconnect Virtual reader {}", nativeReaderName);


                            // retrieve virtual reader to delete
                            std::shared_ptr<VirtualReader> * const virtualReader = std::static_pointer_cast<VirtualReader>(this->getReaderByRemoteName(nativeReaderName));

                            logger->info("Disconnect VirtualReader with name {} with session {}", nativeReaderName);

                            // remove observers
                            (std::static_pointer_cast<VirtualReaderSessionImpl>(virtualReader->getSession()))->clearObservers();

                            // remove reader
                            readers->remove(virtualReader);

                            // send event READER_DISCONNECTED in a separate thread
                            std::make_shared<ThreadAnonymousInnerClass2>(shared_from_this(), virtualReader)
                            .start();

                        }

                        RemoteSePlugin::ThreadAnonymousInnerClass2::ThreadAnonymousInnerClass2(std::shared_ptr<RemoteSePlugin> outerInstance, std::shared_ptr<org::eclipse::keyple::plugin::remotese::pluginse::VirtualReader> virtualReader) {
                            this->outerInstance = outerInstance;
                            this->virtualReader = virtualReader;
                        }

                        void RemoteSePlugin::ThreadAnonymousInnerClass2::run() {
                            outerInstance->notifyObservers(std::make_shared<PluginEvent>(outerInstance->getName(), virtualReader->getName(), PluginEvent::EventType::READER_DISCONNECTED));
                        }

                        void RemoteSePlugin::onReaderEvent(std::shared_ptr<ReaderEvent> event_Renamed, const std::string &sessionId) {
                            logger->debug("OnReaderEvent {}", event_Renamed);
                            logger->debug("Dispatch ReaderEvent to the appropriate Reader : {} sessionId : {}", event_Renamed->getReaderName(), sessionId);
                            try {
                                // TODO : dispatch events is only managed by remote reader name, should take sessionId
                                // also
                                std::shared_ptr<VirtualReader> virtualReader = std::static_pointer_cast<VirtualReader>(getReaderByRemoteName(event_Renamed->getReaderName()));
                                virtualReader->onRemoteReaderEvent(event_Renamed);

                            }
                            catch (const KeypleReaderNotFoundException &e) {
                                e->printStackTrace();
                            }

                        }

                        Boolean RemoteSePlugin::isReaderConnected(const std::string &name) {
                            for (auto virtualReader : readers) {
                                if ((std::static_pointer_cast<VirtualReader>(virtualReader))->getNativeReaderName() == name) {
                                    return true;
                                }
                            }
                            return false;
                        }

                        std::shared_ptr<SortedSet<std::shared_ptr<AbstractObservableReader>>> RemoteSePlugin::getNativeReaders() throw(KeypleReaderException) {
                            // not necessary
                            return std::set<std::shared_ptr<AbstractObservableReader>>();
                        }

                        std::shared_ptr<AbstractObservableReader> RemoteSePlugin::getNativeReader(const std::string &s) throw(KeypleReaderException) {
                            // should not be call
                            throw std::invalid_argument("Use getReader method instead of getNativeReader");
                        }
                    }
                }
            }
        }
    }
}
