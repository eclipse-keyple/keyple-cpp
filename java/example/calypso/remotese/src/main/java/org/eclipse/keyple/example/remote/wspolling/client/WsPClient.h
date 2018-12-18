#pragma once

#include <string>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class TransportDto; } } } } } }

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
                        namespace client {

                            using namespace org::eclipse::keyple::plugin::remotese::transport;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;
                            using com::google::gson::JsonObject;

                            /**
                             * Rest client, polls server, based on java.net client and multi threads
                             */
                            class WsPClient : public std::enable_shared_from_this<WsPClient>, public ClientNode {


                            private:
                                static const std::shared_ptr<Logger> logger;

                                const std::string keypleDtoEndpoint;
                                const std::string pollingEndpoint;
                                const std::string nodeId;
                                const std::string baseUrl;

                                std::shared_ptr<DtoHandler> dtoHandler;

                            public:
                                WsPClient(const std::string &baseUrl, const std::string &keypleDtoEndpoint, const std::string &pollingEndpoint, const std::string &nodeId);



                                virtual void startPollingWorker(const std::string &nodeId);

                            private:
                                class ThreadAnonymousInnerClass : public Thread {
                                private:
                                    std::shared_ptr<WsPClient> outerInstance;

                                    std::string nodeId;

                                public:
                                    ThreadAnonymousInnerClass(std::shared_ptr<WsPClient> outerInstance, const std::string &nodeId);

                                    void run();

protected:
                                    std::shared_ptr<ThreadAnonymousInnerClass> shared_from_this() {
                                        return std::static_pointer_cast<ThreadAnonymousInnerClass>(Thread::shared_from_this());
                                    }
                                };


                            private:
                                void processHttpResponseDTO(std::shared_ptr<JsonObject> httpResponse);

                            private:
                                class ThreadAnonymousInnerClass2 : public Thread {
                                private:
                                    std::shared_ptr<WsPClient> outerInstance;

                                    std::shared_ptr<org::eclipse::keyple::plugin::remotese::transport::TransportDto> sendback;

                                public:
                                    ThreadAnonymousInnerClass2(std::shared_ptr<WsPClient> outerInstance, std::shared_ptr<org::eclipse::keyple::plugin::remotese::transport::TransportDto> sendback);

                                    void run() override;

protected:
                                    std::shared_ptr<ThreadAnonymousInnerClass2> shared_from_this() {
                                        return std::static_pointer_cast<ThreadAnonymousInnerClass2>(Thread::shared_from_this());
                                    }
                                };


                            public:
                                void sendDTO(std::shared_ptr<TransportDto> transportDto) override;

                                void sendDTO(std::shared_ptr<KeypleDto> message) override;

                                void update(std::shared_ptr<KeypleDto> event_Renamed) override;


                                /*
                                 * TransportNode
                                 */
                                void setDtoHandler(std::shared_ptr<DtoHandler> dtoHandler) override;

                                std::string getNodeId() override;


                                void connect() override;

                                void disconnect() override;


                            private:
                                std::shared_ptr<JsonObject> httpPOSTJson(std::shared_ptr<HttpURLConnection> conn, const std::string &json) throw(IOException);

                                std::shared_ptr<HttpURLConnection> getConnection(const std::string &urlString) throw(IOException);


                                std::shared_ptr<JsonObject> httpPoll(std::shared_ptr<HttpURLConnection> conn, const std::string &json) throw(IOException);

                                std::shared_ptr<JsonObject> parseBody(std::shared_ptr<InputStream> is);

                            };

                        }
                    }
                }
            }
        }
    }
}
