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
                            using com::sun::net::httpserver::HttpServer;

                            class WsPServer : public std::enable_shared_from_this<WsPServer>, public ServerNode {


                            private:
                                std::shared_ptr<InetSocketAddress> inet;
                                const std::string apiUrl;
                                const std::string pollingUrl;
                                const std::string nodeId;
                                const std::shared_ptr<HttpServer> server;
                                const Integer MAX_CONNECTION = 10;
                                const std::shared_ptr<HttpHandler> keypleDTOEndpoint;
                                const std::shared_ptr<HttpHandler> pollingEndpoint;

                                static const std::shared_ptr<Logger> logger;

                                const std::shared_ptr<BlockingQueue<std::shared_ptr<HttpExchange>>> requestQueue = std::make_shared<LinkedBlockingQueue<std::shared_ptr<HttpExchange>>>();

                                /**
                                 * Constructor
                                 * 
                                 * @param url : hostname url to bind server
                                 * @param port : port to bind server to
                                 * @param apiUrl : api URL to deploy keypleDto listener endpoint
                                 * @param pollingUrl : polling URL to deploy polling listener endpoint
                                 * @throws IOException : if server can not bind given parameters
                                 */
                            public:
                                WsPServer(const std::string &url, Integer port, const std::string &apiUrl, const std::string &pollingUrl, const std::string &nodeId) throw(IOException);

                                void start() override;

                                /*
                                 * TransportNode
                                 */
                                void setDtoHandler(std::shared_ptr<DtoHandler> receiver) override;

                                std::string getNodeId() override;

                                void sendDTO(std::shared_ptr<TransportDto> message) override;

                                void sendDTO(std::shared_ptr<KeypleDto> message) override;

                                void update(std::shared_ptr<KeypleDto> event_Renamed) override;

                                // todo enable this?

                                /**
                                 * Free httpExchange after 15 seconds
                                 * 
                                 * @param queue private void setPollingWorker(final Queue<HttpExchange> queue) {
                                 * 
                                 *        Thread PollingWorker = new Thread() { public void run() {
                                 * 
                                 *        logger.debug("Starting Polling Worker"); try { while (true) {
                                 * 
                                 * 
                                 *        // wait for 15000 Thread.sleep(15000); logger.trace("Clear all HttpExchange waiting in
                                 *        queue");
                                 * 
                                 *        synchronized (queue) { // close all httpEchange while (!queue.isEmpty()) {
                                 *        HttpExchange lastHttpExchange = queue.poll(); lastHttpExchange.close(); } } }
                                 * 
                                 *        } catch (Exception e) { e.printStackTrace(); logger.error("Error in polling worker
                                 *        {}", e.getCause()); }
                                 * 
                                 *        } };
                                 * 
                                 *        PollingWorker.start(); }
                                 */



                            };

                        }
                    }
                }
            }
        }
    }
}
