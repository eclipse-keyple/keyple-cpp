#include "RmTransmitExecutor.h"
#include "../NativeReaderServiceImpl.h"
#include "../../transport/model/TransportDto.h"
#include "../../transport/model/KeypleDto.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeResponseSet.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeRequestSet.h"
#include "../../transport/json/JsonParser.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ProxyReader.h"
#include "../../rm/RemoteMethod.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace nativese {
                        namespace method {
                            using NativeReaderServiceImpl = org::eclipse::keyple::plugin::remotese::nativese::NativeReaderServiceImpl;
                            using RemoteMethod = org::eclipse::keyple::plugin::remotese::rm::RemoteMethod;
                            using RemoteMethodExecutor = org::eclipse::keyple::plugin::remotese::rm::RemoteMethodExecutor;
                            using namespace org::eclipse::keyple::plugin::remotese::transport;
                            using JsonParser = org::eclipse::keyple::plugin::remotese::transport::json::JsonParser;
                            using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::model::KeypleDto;
                            using TransportDto = org::eclipse::keyple::plugin::remotese::transport::model::TransportDto;
                            using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                            using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
                            using SeRequestSet = org::eclipse::keyple::seproxy::message::SeRequestSet;
                            using SeResponseSet = org::eclipse::keyple::seproxy::message::SeResponseSet;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> RmTransmitExecutor::logger = org::slf4j::LoggerFactory::getLogger(RmTransmitExecutor::typeid);

                            RmTransmitExecutor::RmTransmitExecutor(std::shared_ptr<NativeReaderServiceImpl> nativeReaderService) : nativeReaderService(nativeReaderService) {
                            }

                            std::shared_ptr<TransportDto> RmTransmitExecutor::execute(std::shared_ptr<TransportDto> transportDto) {
                                std::shared_ptr<KeypleDto> keypleDto = transportDto->getKeypleDTO();
                                std::shared_ptr<TransportDto> out = nullptr;
                                std::shared_ptr<SeResponseSet> seResponseSet = nullptr;

                                // Extract info from keypleDto
                                std::shared_ptr<SeRequestSet> seRequestSet = JsonParser::getGson()->fromJson(keypleDto->getBody(), SeRequestSet::typeid);
                                std::string nativeReaderName = keypleDto->getNativeReaderName();
                                logger->trace("Execute locally seRequestSet : {}", seRequestSet);

                                try {
                                    // find native reader by name
                                    std::shared_ptr<ProxyReader> reader = nativeReaderService->findLocalReader(nativeReaderName);

                                    // execute transmitSet
                                    seResponseSet = reader->transmitSet(seRequestSet);

                                    // prepare response
                                    std::string parseBody = JsonParser::getGson()->toJson(seResponseSet, SeResponseSet::typeid);
                                    out = transportDto->nextTransportDTO(std::make_shared<KeypleDto>(RemoteMethod::READER_TRANSMIT.getName(), parseBody, false, keypleDto->getSessionId(), nativeReaderName, keypleDto->getVirtualReaderName(), keypleDto->getNodeId()));

                                }
                                catch (const KeypleReaderException &e) {
                                    // if an exception occurs, send it into a keypleDto to the Master
                                    out = transportDto->nextTransportDTO(KeypleDtoHelper::ExceptionDTO(RemoteMethod::READER_TRANSMIT.getName(), e, keypleDto->getSessionId(), nativeReaderName, keypleDto->getVirtualReaderName(), keypleDto->getNodeId()));
                                }

                                return out;
                            }
                        }
                    }
                }
            }
        }
    }
}
