#include "NativeReaderServiceImpl.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../transport/RemoteMethod.h"
#include "method/RmConnectReaderParser.h"
#include "method/RmTransmitExecutor.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ProxyReader.h"
#include "method/RmConnectReaderInvoker.h"
#include "method/RmDisconnectReaderInvoker.h"
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
                        using RmConnectReaderInvoker = org::eclipse::keyple::plugin::remotese::nativese::method::RmConnectReaderInvoker;
                        using RmConnectReaderParser = org::eclipse::keyple::plugin::remotese::nativese::method::RmConnectReaderParser;
                        using RmDisconnectReaderInvoker = org::eclipse::keyple::plugin::remotese::nativese::method::RmDisconnectReaderInvoker;
                        using RmTransmitExecutor = org::eclipse::keyple::plugin::remotese::nativese::method::RmTransmitExecutor;
                        using namespace org::eclipse::keyple::plugin::remotese::transport;
                        using JsonParser = org::eclipse::keyple::plugin::remotese::transport::json::JsonParser;
                        using ReaderPlugin = org::eclipse::keyple::seproxy::ReaderPlugin;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                        using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                        using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                        using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
                        using AbstractObservableReader = org::eclipse::keyple::seproxy::plugin::AbstractObservableReader;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> NativeReaderServiceImpl::logger = org::slf4j::LoggerFactory::getLogger(NativeReaderServiceImpl::typeid);

                        NativeReaderServiceImpl::NativeReaderServiceImpl(std::shared_ptr<DtoSender> dtoSender) : dtoSender(dtoSender), seProxyService(SeProxyService::getInstance()) {
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
                                case org::eclipse::keyple::plugin::remotese::transport::RemoteMethod::InnerEnum::READER_CONNECT:
                                    // process READER_CONNECT response
                                    if (keypleDTO->isRequest()) {
                                        throw std::make_shared<IllegalStateException>("a READER_CONNECT request has been received by NativeReaderService");
                                    }
                                    else {
                                        try {
                                            std::shared_ptr<RemoteMethodParser<std::string>> rmConnectReaderParser = std::make_shared<RmConnectReaderParser>(shared_from_this());
                                            std::string sessionId = rmConnectReaderParser->parseResponse(keypleDTO);
                                            logger->info("Native Reader {} has been connected to Master with sessionId {}", keypleDTO->getNativeReaderName(), sessionId);
                                        }
                                        catch (const KeypleRemoteReaderException &e) {
                                            e->printStackTrace();
                                        }
                                        out = transportDto->nextTransportDTO(KeypleDtoHelper::NoResponse());
                                    }
                                    break;

                                case org::eclipse::keyple::plugin::remotese::transport::RemoteMethod::InnerEnum::READER_DISCONNECT:
                                    // process READER_DISCONNECT response
                                    if (keypleDTO->isRequest()) {
                                        throw std::make_shared<IllegalStateException>("a READER_DISCONNECT request has been received by NativeReaderService");
                                    }
                                    else {
                                        logger->info("Native Reader {} has been disconnected from Master", keypleDTO->getNativeReaderName());
                                        out = transportDto->nextTransportDTO(KeypleDtoHelper::NoResponse());
                                    }
                                    break;


                                case org::eclipse::keyple::plugin::remotese::transport::RemoteMethod::InnerEnum::READER_TRANSMIT:
                                    // must be a request
                                    if (keypleDTO->isRequest()) {
                                        std::shared_ptr<RemoteMethodExecutor> rmTransmit = std::make_shared<RmTransmitExecutor>(shared_from_this());
                                        out = rmTransmit->execute(transportDto);
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

                            logger->debug("onDto response to be sent {}", KeypleDtoHelper::toJson(out->getKeypleDTO()));
                            return out;


                        }

                        void NativeReaderServiceImpl::connectReader(std::shared_ptr<ProxyReader> localReader, const std::string &clientNodeId) throw(KeypleRemoteException) {
                            logger->info("connectReader {} from device {}", localReader->getName(), clientNodeId);
                            dtoSender->sendDTO((std::make_shared<RmConnectReaderInvoker>(localReader, clientNodeId))->dto());
                        }

                        void NativeReaderServiceImpl::disconnectReader(std::shared_ptr<ProxyReader> localReader, const std::string &clientNodeId) throw(KeypleRemoteException) {
                            logger->info("disconnectReader {} from device {}", localReader->getName(), clientNodeId);

                            dtoSender->sendDTO((std::make_shared<RmDisconnectReaderInvoker>(localReader, clientNodeId))->dto());

                            // stop propagating the local reader events
                            (std::static_pointer_cast<AbstractObservableReader>(localReader))->removeObserver(shared_from_this());

                        }

                        std::shared_ptr<ProxyReader> NativeReaderServiceImpl::findLocalReader(const std::string &nativeReaderName) throw(KeypleReaderNotFoundException) {
                            logger->debug("Find local reader by name {} in {} plugin(s)", nativeReaderName, seProxyService->getPlugins()->size());
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
                            logger->info("update Reader Event {}", event_Renamed->getEventType());

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
