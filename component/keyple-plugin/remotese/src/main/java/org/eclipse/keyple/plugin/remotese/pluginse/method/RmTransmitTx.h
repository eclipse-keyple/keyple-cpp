#pragma once

#include "../../rm/RemoteMethodTx.h"
#include <string>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class SeResponseSet; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class SeRequestSet; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace model { class KeypleDto; } } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace exception { class KeypleRemoteException; } } } } } }

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
                    namespace pluginse {
                        namespace method {

                            using KeypleRemoteException = org::eclipse::keyple::plugin::remotese::exception::KeypleRemoteException;
                            using RemoteMethodTx = org::eclipse::keyple::plugin::remotese::rm::RemoteMethodTx;
                            using namespace org::eclipse::keyple::plugin::remotese::transport;
                            using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::model::KeypleDto;
                            using SeRequestSet = org::eclipse::keyple::seproxy::message::SeRequestSet;
                            using SeResponseSet = org::eclipse::keyple::seproxy::message::SeResponseSet;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;

                            class RmTransmitTx : public RemoteMethodTx<std::shared_ptr<SeResponseSet>> {

                            private:
                                static const std::shared_ptr<Logger> logger;

                                const std::shared_ptr<SeRequestSet> seRequestSet;



                            public:
                                RmTransmitTx(std::shared_ptr<SeRequestSet> seRequestSet, const std::string &sessionId, const std::string &nativeReaderName, const std::string &virtualReaderName, const std::string &clientNodeId);

                                std::shared_ptr<KeypleDto> dto() override;


                                std::shared_ptr<SeResponseSet> parseResponse(std::shared_ptr<KeypleDto> keypleDto) throw(KeypleRemoteException) override;



protected:
                                std::shared_ptr<RmTransmitTx> shared_from_this() {
                                    return std::static_pointer_cast<RmTransmitTx>(org.eclipse.keyple.plugin.remotese.rm.RemoteMethodTx<org.eclipse.keyple.seproxy.message.SeResponseSet>::shared_from_this());
                                }
                            };

                        }
                    }
                }
            }
        }
    }
}
