#pragma once

#include <string>
#include <unordered_map>
#include <stdexcept>
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
                    namespace websocket {

                        using namespace org::eclipse::keyple::plugin::remotese::transport;
                        using org::java_websocket::WebSocket;
                        using org::java_websocket::handshake::ClientHandshake;
                        using org::java_websocket::server::WebSocketServer;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        /**
                         * Web socket server
                         */
                        class WskServer : public WebSocketServer, public ServerNode {

                        private:
                            static const std::shared_ptr<Logger> logger;
                            std::shared_ptr<DtoHandler> dtoHandler;

                            // only for when server is master
                            Boolean isMaster;
                            std::shared_ptr<WebSocket> masterWebSocket;
                            const std::string nodeId;

                        public:
                            WskServer(std::shared_ptr<InetSocketAddress> address, Boolean isMaster, const std::string &nodeId);

                            /*
                             * WebSocketServer
                             */
                            void onOpen(std::shared_ptr<WebSocket> conn, std::shared_ptr<ClientHandshake> handshake) override;

                            void onClose(std::shared_ptr<WebSocket> conn, int code, const std::string &reason, bool remote) override;

                            /**
                             * Incoming message
                             * 
                             * @param conn : websocket connection used
                             * @param message : incoming message
                             */
                            void onMessage(std::shared_ptr<WebSocket> conn, const std::string &message) override;

                            void onError(std::shared_ptr<WebSocket> conn, std::runtime_error ex) override;

                            void onStart() override;


                            /*
                             * TransportNode
                             */

                        private:
                            const std::unordered_map<std::string, std::shared_ptr<void>> sessionId_Connection = std::unordered_map<std::string, std::shared_ptr<void>>();

                            std::shared_ptr<void> getConnection(const std::string &sessionId);

                        public:
                            void setDtoHandler(std::shared_ptr<DtoHandler> stubplugin) override;


                            void sendDTO(std::shared_ptr<TransportDto> transportDto) override;

                            /*
                             * DTO Sender
                             */
                            void sendDTO(std::shared_ptr<KeypleDto> message) override;

                            std::string getNodeId() override;


                            void update(std::shared_ptr<KeypleDto> event_Renamed) override;

protected:
                            std::shared_ptr<WskServer> shared_from_this() {
                                return std::static_pointer_cast<WskServer>(org.java_websocket.server.WebSocketServer::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
