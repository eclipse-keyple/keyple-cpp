#pragma once

#include "../../transport/RemoteMethodParser.h"
#include <string>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace nativese { class NativeReaderServiceImpl; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class KeypleDto; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class KeypleRemoteReaderException; } } } } } }

/********************************************************************************
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * See the NOTICE file(s) distributed with this work for additional information regarding copyright
 * ownership.
 *
 * This program and the accompanying materials are made available under the terms of the Eclipse
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
 *
 * SPDX-License-Identifier: EPL-2.0
 ********************************************************************************/
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace nativese {
                        namespace method {

                            using NativeReaderServiceImpl = org::eclipse::keyple::plugin::remotese::nativese::NativeReaderServiceImpl;
                            using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::KeypleDto;
                            using KeypleRemoteReaderException = org::eclipse::keyple::plugin::remotese::transport::KeypleRemoteReaderException;
                            using RemoteMethodParser = org::eclipse::keyple::plugin::remotese::transport::RemoteMethodParser;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;


                            class RmConnectReaderParser : public std::enable_shared_from_this<RmConnectReaderParser>, public RemoteMethodParser<std::string> {

                            private:
                                static const std::shared_ptr<Logger> logger;

                                const std::shared_ptr<NativeReaderServiceImpl> nativeReaderService;

                            public:
                                RmConnectReaderParser(std::shared_ptr<NativeReaderServiceImpl> nativeReaderService);


                                std::string parseResponse(std::shared_ptr<KeypleDto> keypleDto) throw(KeypleRemoteReaderException) override;
                            };

                        }
                    }
                }
            }
        }
    }
}
