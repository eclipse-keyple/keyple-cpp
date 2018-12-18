#pragma once

#include <string>
#include <memory>

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
            namespace example {
                namespace remote {
                    namespace wspolling {
                        namespace server {

                            using namespace org::eclipse::keyple::plugin::remotese::transport;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;
                            using com::sun::net::httpserver::HttpExchange;
                            using com::sun::net::httpserver::HttpHandler;


                            /**
                             * Endpoint for receiving KeypleDTO from clients
                             */
                            class EndpointKeypleDTO : public std::enable_shared_from_this<EndpointKeypleDTO>, public HttpHandler, public TransportNode {


                            private:
                                const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(EndpointKeypleDTO::typeid);

                                std::shared_ptr<DtoHandler> dtoHandler;
                                const std::shared_ptr<DtoSender> dtoSender;
                                const std::string nodeId;

                            public:
                                EndpointKeypleDTO(std::shared_ptr<DtoSender> dtoSender, const std::string &nodeId);

                                void handle(std::shared_ptr<HttpExchange> t) throw(IOException) override;

                                /*
                                 * TransportNode
                                 */
                                void setDtoHandler(std::shared_ptr<DtoHandler> receiver) override;


                                void sendDTO(std::shared_ptr<TransportDto> message) override;

                                void sendDTO(std::shared_ptr<KeypleDto> message) override;

                                std::string getNodeId() override;


                            private:
                                void setHttpResponse(std::shared_ptr<HttpExchange> t, std::shared_ptr<KeypleDto> resp) throw(IOException);

                                std::string parseBodyToString(std::shared_ptr<InputStream> is);

                            public:
                                void update(std::shared_ptr<KeypleDto> event_Renamed) override;
                            };

                        }
                    }
                }
            }
        }
    }
}
