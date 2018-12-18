#include "RmTransmitTx.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeResponseSet.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeRequestSet.h"
#include "../../transport/json/JsonParser.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace pluginse {
                        namespace method {
                            using namespace org::eclipse::keyple::plugin::remotese::transport;
                            using JsonParser = org::eclipse::keyple::plugin::remotese::transport::json::JsonParser;
                            using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                            using SeRequestSet = org::eclipse::keyple::seproxy::message::SeRequestSet;
                            using SeResponseSet = org::eclipse::keyple::seproxy::message::SeResponseSet;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> RmTransmitTx::logger = org::slf4j::LoggerFactory::getLogger(RmTransmitTx::typeid);

                            RmTransmitTx::RmTransmitTx(std::shared_ptr<SeRequestSet> seRequestSet, const std::string &sessionId, const std::string &nativeReaderName, const std::string &virtualReaderName, const std::string &clientNodeId) : seRequestSet(seRequestSet), sessionId(sessionId), nativeReaderName(nativeReaderName), virtualReaderName(virtualReaderName), clientNodeId(clientNodeId) {
                            }

                            std::shared_ptr<KeypleDto> RmTransmitTx::dto() {
                                return std::make_shared<KeypleDto>(RemoteMethod::READER_TRANSMIT.getName(), JsonParser::getGson()->toJson(seRequestSet, SeRequestSet::typeid), true, this->sessionId, this->nativeReaderName, this->virtualReaderName, this->clientNodeId);
                            }

                            std::shared_ptr<SeResponseSet> RmTransmitTx::parseResponse(std::shared_ptr<KeypleDto> keypleDto) throw(KeypleRemoteException) {

                                logger->trace("KeypleDto : {}", keypleDto);
                                if (KeypleDtoHelper::containsException(keypleDto)) {
                                    logger->trace("KeypleDto contains an exception: {}", keypleDto);
                                    std::shared_ptr<KeypleReaderException> ex = JsonParser::getGson()->fromJson(keypleDto->getBody(), KeypleReaderException::typeid);
                                    throw std::make_shared<KeypleRemoteException>("An exception occurs while calling the remote method transmitSet", ex);
                                }
                                else {
                                    logger->trace("KeypleDto contains a response: {}", keypleDto);
                                    return JsonParser::getGson()->fromJson(keypleDto->getBody(), SeResponseSet::typeid);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
