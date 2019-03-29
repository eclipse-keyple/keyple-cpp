#pragma once

#include "../../rm/RemoteMethodParser.h"
#include <string>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace nativese { class NativeReaderServiceImpl; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace exception { class KeypleRemoteReaderException; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace model { class KeypleDto; } } } } } } }

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

                            using KeypleRemoteReaderException = org::eclipse::keyple::plugin::remotese::exception::KeypleRemoteReaderException;
                            using NativeReaderServiceImpl = org::eclipse::keyple::plugin::remotese::nativese::NativeReaderServiceImpl;
                            using RemoteMethodParser = org::eclipse::keyple::plugin::remotese::rm::RemoteMethodParser;
                            using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::model::KeypleDto;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Deprecated public class RmConnectReaderParser implements org.eclipse.keyple.plugin.remotese.rm.RemoteMethodParser<String>
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
