#include "RmConnectReaderParser.h"
#include "../NativeReaderServiceImpl.h"
#include "../../transport/KeypleDto.h"
#include "../../transport/KeypleRemoteReaderException.h"
#include "../../transport/KeypleDtoHelper.h"
#include "../../transport/json/JsonParser.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ProxyReader.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/plugin/AbstractSelectionLocalReader.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderNotFoundException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace nativese {
                        namespace method {
                            using NativeReaderServiceImpl = org::eclipse::keyple::plugin::remotese::nativese::NativeReaderServiceImpl;
                            using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::KeypleDto;
                            using KeypleDtoHelper = org::eclipse::keyple::plugin::remotese::transport::KeypleDtoHelper;
                            using KeypleRemoteReaderException = org::eclipse::keyple::plugin::remotese::transport::KeypleRemoteReaderException;
                            using RemoteMethodParser = org::eclipse::keyple::plugin::remotese::transport::RemoteMethodParser;
                            using JsonParser = org::eclipse::keyple::plugin::remotese::transport::json::JsonParser;
                            using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                            using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
                            using AbstractSelectionLocalReader = org::eclipse::keyple::seproxy::plugin::AbstractSelectionLocalReader;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> RmConnectReaderParser::logger = org::slf4j::LoggerFactory::getLogger(RmConnectReaderParser::typeid);

                            RmConnectReaderParser::RmConnectReaderParser(std::shared_ptr<NativeReaderServiceImpl> nativeReaderService) : nativeReaderService(nativeReaderService) {
                            }

                            std::string RmConnectReaderParser::parseResponse(std::shared_ptr<KeypleDto> keypleDto) throw(KeypleRemoteReaderException) {

                                std::string nativeReaderName = keypleDto->getNativeReaderName();

                                // reader connection was a success
                                if (KeypleDtoHelper::containsException(keypleDto)) {
                                    std::runtime_error ex = JsonParser::getGson()->fromJson(keypleDto->getBody(), std::runtime_error::typeid);
                                    throw std::make_shared<KeypleRemoteReaderException>("An exception occurs while calling the remote method Connect Reader", ex);
                                }
                                else {
                                    try {
                                        // observe reader to propagate reader events
                                        std::shared_ptr<ProxyReader> localReader = nativeReaderService->findLocalReader(nativeReaderName);
                                        if (std::dynamic_pointer_cast<AbstractSelectionLocalReader>(localReader) != nullptr) {
                                            logger->debug("Add NativeReaderServiceImpl as an observer for native reader {}", localReader->getName());
                                            (std::static_pointer_cast<AbstractSelectionLocalReader>(localReader))->addObserver(nativeReaderService);
                                        }


                                        // todo sessionId should be stored here

                                    }
                                    catch (const KeypleReaderNotFoundException &e) {
                                        logger->warn("While receiving a confirmation of Rse connection, local reader was not found");
                                    }
                                    return keypleDto->getSessionId();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
