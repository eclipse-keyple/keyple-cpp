#pragma once

#include "../../rm/RemoteMethodTx.h"
#include <string>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
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
                            using RemoteMethodTx = org::eclipse::keyple::plugin::remotese::rm::RemoteMethodTx;
                            using namespace org::eclipse::keyple::plugin::remotese::transport;
                            using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::model::KeypleDto;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;

                            class RmDisconnectReaderTx : public RemoteMethodTx<Boolean> {

                            private:
                                static const std::shared_ptr<Logger> logger;


                            public:
                                RmDisconnectReaderTx(const std::string &sessionId, const std::string &nativeReaderName, const std::string &slaveNodeId);

                                Boolean parseResponse(std::shared_ptr<KeypleDto> keypleDto) throw(KeypleRemoteException) override;

                                std::shared_ptr<KeypleDto> dto() override;

protected:
                                std::shared_ptr<RmDisconnectReaderTx> shared_from_this() {
                                    return std::static_pointer_cast<RmDisconnectReaderTx>(org.eclipse.keyple.plugin.remotese.rm.RemoteMethodTx<Boolean>::shared_from_this());
                                }
                            };

                        }
                    }
                }
            }
        }
    }
}
