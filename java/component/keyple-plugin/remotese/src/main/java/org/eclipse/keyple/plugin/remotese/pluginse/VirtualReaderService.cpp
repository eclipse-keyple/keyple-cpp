#include "VirtualReaderService.h"
#include "RemoteSePlugin.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "VirtualReaderSessionFactory.h"
#include "../transport/RemoteMethod.h"
#include "method/RmConnectReaderExecutor.h"
#include "method/RmDisconnectReaderExecutor.h"
#include "method/RmEventExecutor.h"
#include "VirtualReader.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderNotFoundException.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace pluginse {
                        using RmConnectReaderExecutor = org::eclipse::keyple::plugin::remotese::pluginse::method::RmConnectReaderExecutor;
                        using RmDisconnectReaderExecutor = org::eclipse::keyple::plugin::remotese::pluginse::method::RmDisconnectReaderExecutor;
                        using RmEventExecutor = org::eclipse::keyple::plugin::remotese::pluginse::method::RmEventExecutor;
                        using namespace org::eclipse::keyple::plugin::remotese::transport;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using SeReader = org::eclipse::keyple::seproxy::SeReader;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> VirtualReaderService::logger = org::slf4j::LoggerFactory::getLogger(VirtualReaderService::typeid);

                        VirtualReaderService::VirtualReaderService(std::shared_ptr<SeProxyService> seProxyService, std::shared_ptr<DtoSender> dtoSender) : dtoSender(dtoSender), plugin(std::make_shared<RemoteSePlugin>(sessionManager, dtoSender)) {

                            // Instantiate Session Manager
                            std::shared_ptr<VirtualReaderSessionFactory> sessionManager = std::make_shared<VirtualReaderSessionFactory>();

                            // Instantiate Plugin
                            seProxyService->addPlugin(this->plugin);
                        }

                        void VirtualReaderService::bindDtoEndpoint(std::shared_ptr<TransportNode> node) {
                            node->setDtoHandler(shared_from_this());
                        }

                        std::shared_ptr<RemoteSePlugin> VirtualReaderService::getPlugin() {
                            return plugin;
                        }

                        std::shared_ptr<TransportDto> VirtualReaderService::onDTO(std::shared_ptr<TransportDto> transportDto) {

                            std::shared_ptr<KeypleDto> keypleDTO = transportDto->getKeypleDTO();
                            RemoteMethod method = RemoteMethod::get(keypleDTO->getAction());
                            logger->trace("onDTO, Remote Method called : {} - isRequest : {} - keypleDto : {}", method, keypleDTO->isRequest(), KeypleDtoHelper::toJson(keypleDTO));



                            switch (method.innerEnumValue) {
                                case org::eclipse::keyple::plugin::remotese::transport::RemoteMethod::InnerEnum::READER_CONNECT:
                                    if (keypleDTO->isRequest()) {
                                        return (std::make_shared<RmConnectReaderExecutor>(this->plugin, this->dtoSender))->execute(transportDto);
                                    }
                                    else {
                                        throw std::make_shared<IllegalStateException>("a READER_CONNECT response has been received by VirtualReaderService");
                                    }
                                case org::eclipse::keyple::plugin::remotese::transport::RemoteMethod::InnerEnum::READER_DISCONNECT:
                                    if (keypleDTO->isRequest()) {
                                        return (std::make_shared<RmDisconnectReaderExecutor>(this->plugin))->execute(transportDto);
                                    }
                                    else {
                                        throw std::make_shared<IllegalStateException>("a READER_DISCONNECT response has been received by VirtualReaderService");
                                    }
                                case org::eclipse::keyple::plugin::remotese::transport::RemoteMethod::InnerEnum::READER_EVENT:
                                    return (std::make_shared<RmEventExecutor>(plugin))->execute(transportDto);
                                case org::eclipse::keyple::plugin::remotese::transport::RemoteMethod::InnerEnum::READER_TRANSMIT:
                                    // can be more general
                                    if (keypleDTO->isRequest()) {
                                        throw std::make_shared<IllegalStateException>("a READER_TRANSMIT request has been received by VirtualReaderService");
                                    }
                                    else {
                                        // dispatch dto to the appropriate reader
                                        try {
                                            // find reader by sessionId
                                            std::shared_ptr<VirtualReader> reader = getReaderBySessionId(keypleDTO->getSessionId());

                                            // process response with the reader rmtx engine
                                            return reader->getRmTxEngine()->onDTO(transportDto);

                                        }
                                        catch (const KeypleReaderNotFoundException &e) {
                                            // reader not found;
                                            throw std::make_shared<IllegalStateException>("Virtual Reader was not found while receiving a transmitSet response", e);
                                        }
                                        catch (const KeypleReaderException &e) {
                                            // reader not found;
                                            throw std::make_shared<IllegalStateException>("Readers list has not been initializated", e);
                                        }
                                    }
                                default:
                                    logger->debug("Default case");
                                    return transportDto->nextTransportDTO(KeypleDtoHelper::NoResponse());
                            }
                        }

                        std::shared_ptr<VirtualReader> VirtualReaderService::getReaderBySessionId(const std::string &sessionId) throw(KeypleReaderNotFoundException, KeypleReaderException) {
                            for (auto reader : plugin->getReaders()) {

                                if ((std::static_pointer_cast<VirtualReader>(reader))->getSession()->getSessionId() == sessionId) {
                                    return std::static_pointer_cast<VirtualReader>(reader);
                                }
                            }
                            throw std::make_shared<KeypleReaderNotFoundException>("Reader session was not found for session : " + sessionId);
                        }
                    }
                }
            }
        }
    }
}
