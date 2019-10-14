#pragma once

#include "../../factory/ClientNode.h"
#include <string>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace impl { namespace java { class LocalServer; } } } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class DtoHandler; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace model { class KeypleDto; } } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace model { class TransportDto; } } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace exception { class KeypleRemoteException; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace factory { class ClientNode; } } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace factory { class ConnectCallback; } } } } } } }

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
                    namespace transport {
                        namespace impl {
                            namespace java {

                                using KeypleRemoteException = org::eclipse::keyple::plugin::remotese::exception::KeypleRemoteException;
                                using namespace org::eclipse::keyple::plugin::remotese::transport;
                                using ClientNode = org::eclipse::keyple::plugin::remotese::transport::factory::ClientNode;
                                using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::model::KeypleDto;
                                using TransportDto = org::eclipse::keyple::plugin::remotese::transport::model::TransportDto;
                                using org::slf4j::Logger;
                                using org::slf4j::LoggerFactory;

                                /**
                                 * Client side of the 1 to 1 local transport for unit testing purposes only one server, only one
                                 * client initied by the {@link LocalTransportFactory}
                                 */
                                class LocalClient : public std::enable_shared_from_this<LocalClient>, public ClientNode {

                                private:
                                    static const std::shared_ptr<Logger> logger;
                                    const std::shared_ptr<LocalServer> theServer;
                                    std::shared_ptr<DtoHandler> dtoHandler;

                                public:
                                    LocalClient(std::shared_ptr<LocalServer> server);

                                    virtual void onLocalMessage(std::shared_ptr<KeypleDto> keypleDto);

                                    void setDtoHandler(std::shared_ptr<DtoHandler> handler) override;

                                    void sendDTO(std::shared_ptr<TransportDto> transportDto) override;

                                    void sendDTO(std::shared_ptr<KeypleDto> keypleDto) throw(KeypleRemoteException) override;

                                    std::string getNodeId() override;

                                    /*
                                     * @Override public void update(KeypleDto event) { try { sendDTO(event); } catch
                                     * (KeypleRemoteException e) { // Error is not propagated
                                     * logger.error("Exception while sending event throw KeypleRemoteInterface", e); } }
                                     */


                                    void connect(std::shared_ptr<ClientNode::ConnectCallback> connectCallback) override;

                                    void disconnect() override;

                                };

                            }
                        }
                    }
                }
            }
        }
    }
}
