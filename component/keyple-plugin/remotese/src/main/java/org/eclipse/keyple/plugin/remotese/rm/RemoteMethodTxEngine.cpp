#include "RemoteMethodTxEngine.h"
#include "RemoteMethodTx.h"
#include "../transport/model/TransportDto.h"
#include "../transport/model/KeypleDto.h"
#include "../exception/KeypleRemoteException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace rm {
                        using KeypleRemoteException = org::eclipse::keyple::plugin::remotese::exception::KeypleRemoteException;
                        using namespace org::eclipse::keyple::plugin::remotese::transport;
                        using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::model::KeypleDto;
                        using TransportDto = org::eclipse::keyple::plugin::remotese::transport::model::TransportDto;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> RemoteMethodTxEngine::logger = org::slf4j::LoggerFactory::getLogger(RemoteMethodTxEngine::typeid);

                        RemoteMethodTxEngine::RemoteMethodTxEngine(std::shared_ptr<DtoSender> sender) : sender(sender) {
                            // this.queue = new LinkedList<RemoteMethodTx>();
                        }

                        std::shared_ptr<TransportDto> RemoteMethodTxEngine::onDTO(std::shared_ptr<TransportDto> message) {
                            std::shared_ptr<KeypleDto> keypleDto = message->getKeypleDTO();
                            if (message->getKeypleDTO()->isRequest()) {
                                throw std::invalid_argument("RemoteMethodTxEngine expects a KeypleDto response. " + message->getKeypleDTO());
                            }
                            if (remoteMethodTx == nullptr) {
                                throw std::make_shared<IllegalStateException>("RemoteMethodTxEngine receives a KeypleDto response but no remoteMethodTx are defined : " + message->getKeypleDTO());
                            }

                            // only one operation is allowed at the time
                            remoteMethodTx->asyncSetResponse(keypleDto);

                            // re init remoteMethod
                            remoteMethodTx.reset();

                            return message->nextTransportDTO(KeypleDtoHelper::NoResponse());
                        }

                        void RemoteMethodTxEngine::register_Renamed(std::shared_ptr<RemoteMethodTx> rm) throw(KeypleRemoteException) {

                            logger->debug("Register rm to engine : {}", rm);
                            remoteMethodTx = rm;

                            rm->setDto(sender);
                        }
                    }
                }
            }
        }
    }
}
