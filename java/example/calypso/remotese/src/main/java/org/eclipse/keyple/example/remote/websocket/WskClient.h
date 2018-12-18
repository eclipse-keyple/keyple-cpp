#pragma once

#include <string>
#include <stdexcept>
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
                        using org::java_websocket::client::WebSocketClient;
                        using org::java_websocket::handshake::ServerHandshake;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        /**
                         * Web socket client
                         */
                        class WskClient : public WebSocketClient, public ClientNode {

                        private:
                            static const std::shared_ptr<Logger> logger;
                            std::shared_ptr<DtoHandler> dtoHandler;
                            const std::string nodeId;

                        public:
                            WskClient(std::shared_ptr<URI> url, const std::string &nodeId);

                            void onOpen(std::shared_ptr<ServerHandshake> handshakedata) override;

                            void onMessage(const std::string &message) override;

                            void onClose(int code, const std::string &reason, bool remote) override;

                            void onError(std::runtime_error ex) override;

                            void sendDTO(std::shared_ptr<TransportDto> transportDto) override;

                            void sendDTO(std::shared_ptr<KeypleDto> keypleDto) override;

                            std::string getNodeId() override;

                            void setDtoHandler(std::shared_ptr<DtoHandler> receiver) override;


                            // observer of keypleDTOSenders
                            void update(std::shared_ptr<KeypleDto> event_Renamed) override;

                            void disconnect() override;

protected:
                            std::shared_ptr<WskClient> shared_from_this() {
                                return std::static_pointer_cast<WskClient>(org.java_websocket.client.WebSocketClient::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
