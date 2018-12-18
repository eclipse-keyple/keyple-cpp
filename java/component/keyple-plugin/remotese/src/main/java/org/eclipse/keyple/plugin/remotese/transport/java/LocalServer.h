#pragma once

#include "../ServerNode.h"
#include <string>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace java { class LocalClient; } } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class DtoHandler; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class TransportDto; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class KeypleDto; } } } } } }

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
                             * Server side of the 1 to 1 local transport for unit testing purposes only one server, only one
                             * client initied by the {@link LocalTransportFactory}
                             */
                            class LocalServer : public std::enable_shared_from_this<LocalServer>, public ServerNode {

                            private:
                                static const std::shared_ptr<Logger> logger;

                                std::shared_ptr<LocalClient> theClient;
                                std::shared_ptr<DtoHandler> dtoHandler;

                            public:
                                LocalServer();

                                virtual void onLocalMessage(std::shared_ptr<TransportDto> transportDto);

                                void start() override;

                                void setDtoHandler(std::shared_ptr<DtoHandler> handler) override;

                                void sendDTO(std::shared_ptr<TransportDto> transportDto) override;

                                void sendDTO(std::shared_ptr<KeypleDto> keypleDto) override;

                                std::string getNodeId() override;

                                void update(std::shared_ptr<KeypleDto> event_Renamed) override;
                            };

                        }
                    }
                }
            }
        }
    }
}
