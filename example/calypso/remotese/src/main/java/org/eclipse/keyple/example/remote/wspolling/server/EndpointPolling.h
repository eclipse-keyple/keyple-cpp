#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "exceptionhelper.h"
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
                             * Endpoint for polling, used to send keypleDto to polling clients
                             */

                            class EndpointPolling : public std::enable_shared_from_this<EndpointPolling>, public HttpHandler, public TransportNode {


                            private:
                                const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(EndpointPolling::typeid);

                                std::shared_ptr<DtoHandler> dtoHandler;
                                const std::string nodeId;
                                const std::shared_ptr<BlockingQueue<std::shared_ptr<HttpExchange>>> requestQueue;

                            public:
                                EndpointPolling(std::shared_ptr<BlockingQueue<std::shared_ptr<HttpExchange>>> requestQueue, const std::string &nodeId);

                                std::string getNodeId() override;

                                void handle(std::shared_ptr<HttpExchange> t) override;

                                /*
                                 * TransportNode
                                 */
                                void setDtoHandler(std::shared_ptr<DtoHandler> receiver) override;


                                void sendDTO(std::shared_ptr<TransportDto> message) override;

                                void sendDTO(std::shared_ptr<KeypleDto> message) override;



                                void update(std::shared_ptr<KeypleDto> event_Renamed) override;


                            private:
                                std::unordered_map<std::string, std::string> queryToMap(const std::string &query);

                                void setHttpResponse(std::shared_ptr<HttpExchange> t, std::shared_ptr<KeypleDto> resp) throw(IOException);
                            };

                        }
                    }
                }
            }
        }
    }
}
