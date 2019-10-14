#pragma once

#include "../../rm/RemoteMethodTx.h"
#include <string>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class ProxyReader; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace nativese { class NativeReaderService; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace exception { class KeypleRemoteException; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace model { class KeypleDto; } } } } } } }

/********************************************************************************
 * Copyright (c) 2019 Calypso Networks Association https://www.calypsonet-asso.org/
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

                            using KeypleRemoteException = org::eclipse::keyple::plugin::remotese::exception::KeypleRemoteException;
                            using NativeReaderService = org::eclipse::keyple::plugin::remotese::nativese::NativeReaderService;
                            using RemoteMethodTx = org::eclipse::keyple::plugin::remotese::rm::RemoteMethodTx;
                            using namespace org::eclipse::keyple::plugin::remotese::transport;
                            using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::model::KeypleDto;
                            using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;

                            class RmConnectReaderTx : public RemoteMethodTx<std::string> {


                            private:
                                const std::shared_ptr<ProxyReader> localReader;
                                const std::string clientNodeId;
                                const std::shared_ptr<NativeReaderService> nativeReaderService;


                            public:
                                RmConnectReaderTx(const std::string &sessionId, const std::string &nativeReaderName, const std::string &virtualReaderName, const std::string &clientNodeId, std::shared_ptr<ProxyReader> localReader, const std::string &clientNodeId1, std::shared_ptr<NativeReaderService> nativeReaderService);

                            private:
                                static const std::shared_ptr<Logger> logger;

                            public:
                                std::string parseResponse(std::shared_ptr<KeypleDto> keypleDto) throw(KeypleRemoteException) override;

                                std::shared_ptr<KeypleDto> dto() override;

protected:
                                std::shared_ptr<RmConnectReaderTx> shared_from_this() {
                                    return std::static_pointer_cast<RmConnectReaderTx>(org.eclipse.keyple.plugin.remotese.rm.RemoteMethodTx<String>::shared_from_this());
                                }
                            };

                        }
                    }
                }
            }
        }
    }
}
