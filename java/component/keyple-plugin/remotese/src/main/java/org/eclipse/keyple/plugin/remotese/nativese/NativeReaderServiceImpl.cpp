#include "NativeReaderServiceImpl.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../rm/RemoteMethodTxEngine.h"
#include "../transport/factory/TransportNode.h"
#include "../transport/model/TransportDto.h"
#include "../transport/model/KeypleDto.h"
#include "../rm/RemoteMethod.h"
#include "../rm/RemoteMethodExecutor.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ProxyReader.h"
#include "../exception/KeypleRemoteException.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/plugin/AbstractObservableReader.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderNotFoundException.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ReaderEvent.h"
#include "../transport/json/JsonParser.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace nativese {
                        using KeypleRemoteException = org::eclipse::keyple::plugin::remotese::exception::KeypleRemoteException;
                        using namespace org::eclipse::keyple::plugin::remotese::nativese::method;
                        using RemoteMethod = org::eclipse::keyple::plugin::remotese::rm::RemoteMethod;
                        using RemoteMethodExecutor = org::eclipse::keyple::plugin::remotese::rm::RemoteMethodExecutor;
                        using RemoteMethodTxEngine = org::eclipse::keyple::plugin::remotese::rm::RemoteMethodTxEngine;
                        using namespace org::eclipse::keyple::plugin::remotese::transport;
                        using TransportNode = org::eclipse::keyple::plugin::remotese::transport::factory::TransportNode;
                        using JsonParser = org::eclipse::keyple::plugin::remotese::transport::json::JsonParser;
                        using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::model::KeypleDto;
                        using TransportDto = org::eclipse::keyple::plugin::remotese::transport::model::TransportDto;
                        using ReaderPlugin = org::eclipse::keyple::seproxy::ReaderPlugin;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                        using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                        using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
                        using AbstractObservableReader = org::eclipse::keyple::seproxy::plugin::AbstractObservableReader;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> NativeReaderServiceImpl::logger = org::slf4j::LoggerFactory::getLogger(NativeReaderServiceImpl::typeid);

                        NativeReaderServiceImpl::NativeReaderServiceImpl(std::shared_ptr<DtoSender> dtoSender) : dtoSender(dtoSender), seProxyService(SeProxyService::getInstance()), rmTxEngine(std::make_shared<RemoteMethodTxEngine>(dtoSender)) {
                            // this.nseSessionManager = new NseSessionManager();
                        }

                        void NativeReaderServiceImpl::bindDtoEndpoint(std::shared_ptr<TransportNode> node) {
                            node->setDtoHandler(shared_from_this()); // incoming traffic
                        }

                        std::shared_ptr<TransportDto> NativeReaderServiceImpl::onDTO(std::shared_ptr<TransportDto> transportDto) {

                            std::shared_ptr<KeypleDto> keypleDTO = transportDto->getKeypleDTO();
                            std::shared_ptr<TransportDto> out;

                            logger->trace("onDto {}", KeypleDtoHelper::toJson(keypleDTO));

                            RemoteMethod method = RemoteMethod::get(keypleDTO->getAction());
                            logger->debug("Remote Method called : {} - isRequest : {}", method, keypleDTO->isRequest());

                            switch (method.innerEnumValue) {
                                case RemoteMethod::InnerEnum::READER_CONNECT:
                                    // process READER_CONNECT response
                                    if (keypleDTO->isRequest()) {
                                        throw std::make_shared<IllegalStateException>("a READER_CONNECT request has been received by NativeReaderService");
                                    }
                                    else {
                                        // send DTO to TxEngine
                                        out = this->rmTxEngine->onDTO(transportDto);
                                    }
                                    break;

                                case RemoteMethod::InnerEnum::READER_DISCONNECT:
                                    // process READER_DISCONNECT response
                                    if (keypleDTO->isRequest()) {
                                        throw std::make_shared<IllegalStateException>("a READER_DISCONNECT request has been received by NativeReaderService");
                                    }
                                    else {
                                        // send DTO to TxEngine
                                        out = this->rmTxEngine->onDTO(transportDto);
                                    }
                                    break;


                                case RemoteMethod::InnerEnum::READER_TRANSMIT:
                                    // must be a request
                                    if (keypleDTO->isRequest()) {
                                        std::shared_ptr<RemoteMethodExecutor> rmTransmit = std::make_shared<RmTransmitExecutor>(shared_from_this());
                                        out = rmTransmit->execute(transportDto);
                                    }
                                    else {
                                        throw std::make_shared<IllegalStateException>("a READER_TRANSMIT response has been received by NativeReaderService");
                                    }
                                    break;

                                case RemoteMethod::InnerEnum::DEFAULT_SELECTION_REQUEST:
                                    // must be a request
                                    if (keypleDTO->isRequest()) {
                                        std::shared_ptr<RmSetDefaultSelectionRequestExecutor> rmSetDefaultSelectionRequest = std::make_shared<RmSetDefaultSelectionRequestExecutor>(shared_from_this());
                                        out = rmSetDefaultSelectionRequest->execute(transportDto);
                                    }
                                    else {
                                        throw std::make_shared<IllegalStateException>("a READER_TRANSMIT response has been received by NativeReaderService");
                                    }
                                    break;

                                default:
                                    logger->warn("**** ERROR - UNRECOGNIZED ****");
                                    logger->warn("Receive unrecognized message action : {} {} {} {}", keypleDTO->getAction(), keypleDTO->getSessionId(), keypleDTO->getBody(), keypleDTO->isRequest());
                                    throw std::make_shared<IllegalStateException>("a  ERROR - UNRECOGNIZED request has been received by NativeReaderService");
                            }

                            logger->trace("onDto response to be sent {}", KeypleDtoHelper::toJson(out->getKeypleDTO()));
                            return out;


                        }

                        std::string NativeReaderServiceImpl::connectReader(std::shared_ptr<ProxyReader> localReader, const std::string &clientNodeId) throw(KeypleReaderException) {

                            logger->info("connectReader {} from device {}", localReader->getName(), clientNodeId);

                            std::shared_ptr<RmConnectReaderTx> connect = std::make_shared<RmConnectReaderTx>(nullptr, localReader->getName(), nullptr, clientNodeId, localReader, clientNodeId, shared_from_this());
                            try {
                                rmTxEngine->register_Renamed(connect);
                                return connect->get();
                            }
                            catch (const KeypleRemoteException &e) {
                                throw std::make_shared<KeypleReaderException>("An error occured while calling connectReader", e);
                            }

                        }

                        void NativeReaderServiceImpl::disconnectReader(const std::string &sessionId, const std::string &nativeReaderName, const std::string &clientNodeId) throw(KeypleReaderException) {
                            logger->info("disconnectReader {} from device {}", nativeReaderName, clientNodeId);

                            std::shared_ptr<RmDisconnectReaderTx> disconnect = std::make_shared<RmDisconnectReaderTx>(sessionId, nativeReaderName, clientNodeId);

                            try {
                                rmTxEngine->register_Renamed(disconnect);
                                Boolean status = disconnect->get();
                                std::shared_ptr<ProxyReader> nativeReader = findLocalReader(nativeReaderName);
                                if (std::dynamic_pointer_cast<AbstractObservableReader>(nativeReader) != nullptr) {
                                    // stop propagating the local reader events
                                    (std::static_pointer_cast<AbstractObservableReader>(nativeReader))->removeObserver(shared_from_this());
                                }
                            }
                            catch (const KeypleRemoteException &e) {
                                throw std::make_shared<KeypleReaderException>("An error occured while calling connectReader", e);
                            }
                            catch (const KeypleReaderNotFoundException &e) {
                                e->printStackTrace();
                            }


                        }

                        std::shared_ptr<ProxyReader> NativeReaderServiceImpl::findLocalReader(const std::string &nativeReaderName) throw(KeypleReaderNotFoundException) {
                            logger->trace("Find local reader by name {} in {} plugin(s)", nativeReaderName, seProxyService->getPlugins()->size());
                            for (auto plugin : seProxyService->getPlugins()) {
                                try {
                                    return std::static_pointer_cast<ProxyReader>(plugin->getReader(nativeReaderName));
                                }
                                catch (const KeypleReaderNotFoundException &e) {
                                    // continue
                                }
                            }
                            throw std::make_shared<KeypleReaderNotFoundException>(nativeReaderName);
                        }

                        void NativeReaderServiceImpl::update(std::shared_ptr<ReaderEvent> event_Renamed) {
                            logger->info("NativeReaderServiceImpl listens for event from native Reader - Received Event {}", event_Renamed->getEventType());

                            // retrieve last sessionId known for this reader
                            // String sessionId = nseSessionManager.getLastSession(event.getReaderName());

                            // construct json data
                            std::string data = JsonParser::getGson()->toJson(event_Renamed);

                            try {
                                dtoSender->sendDTO(std::make_shared<KeypleDto>(RemoteMethod::READER_EVENT.getName(), data, true, nullptr, event_Renamed->getReaderName(), nullptr, this->dtoSender->getNodeId()));
                            }
                            catch (const KeypleRemoteException &e) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                logger->error("Event " + event_Renamed->toString() + " could not be sent though Remote Service Interface", e);
                            }
                        }
                    }
                }
            }
        }
    }
}
