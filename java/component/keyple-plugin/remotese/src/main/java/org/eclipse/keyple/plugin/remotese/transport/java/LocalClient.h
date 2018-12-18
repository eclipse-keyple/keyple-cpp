#pragma once

#include "../ClientNode.h"
#include <string>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace java { class LocalServer; } } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class DtoHandler; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class KeypleDto; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class TransportDto; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class KeypleRemoteException; } } } } } }

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
                        namespace java {

                            using namespace org::eclipse::keyple::plugin::remotese::transport;
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

                                void update(std::shared_ptr<KeypleDto> event_Renamed) override;


                                void connect() override;

                                void disconnect() override;

                            };

                        }
                    }
                }
            }
        }
    }
}
