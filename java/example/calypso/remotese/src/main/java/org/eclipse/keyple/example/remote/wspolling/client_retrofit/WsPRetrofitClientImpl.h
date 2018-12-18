#pragma once

#include <string>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace example { namespace remote { namespace wspolling { namespace client_retrofit { class WsPRetrofitClient; } } } } } } }

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
                        namespace client_retrofit {

                            using namespace org::eclipse::keyple::plugin::remotese::transport;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;
                            using retrofit2::Response;

                            /**
                             * Rest client, polls server, based on client_retrofit and callbacks
                             */
                            class WsPRetrofitClientImpl : public std::enable_shared_from_this<WsPRetrofitClientImpl>, public ClientNode {


                            private:
                                static const std::shared_ptr<Logger> logger;

                                const std::string baseUrl;
                                const std::string nodeId;
//JAVA TO C++ CONVERTER NOTE: Fields cannot have the same name as methods:
                                Boolean poll_Renamed;

                                std::shared_ptr<DtoHandler> dtoHandler;

                            public:
                                WsPRetrofitClientImpl(const std::string &baseUrl, const std::string &nodeId);


                                /**
                                 * recursive polling method based on client_retrofit callbacks
                                 * 
                                 * @param nodeId : terminal node Id (ie : androidDevice1)
                                 */
                            private:
                                void startPollingWorker(const std::string &nodeId);

                                void poll(const std::string &nodeId);

                            private:
                                class CallbackAnonymousInnerClass : public Callback<std::shared_ptr<KeypleDto>> {
                                private:
                                    std::shared_ptr<WsPRetrofitClientImpl> outerInstance;

                                    std::string nodeId;
                                    std::shared_ptr<Call<std::shared_ptr<KeypleDto>>> call;

                                public:
                                    CallbackAnonymousInnerClass(std::shared_ptr<WsPRetrofitClientImpl> outerInstance, const std::string &nodeId, std::shared_ptr<Call<std::shared_ptr<KeypleDto>>> call);

                                    void onResponse(std::shared_ptr<Call<std::shared_ptr<KeypleDto>>> call, std::shared_ptr<Response<std::shared_ptr<KeypleDto>>> response) override;

                                    void onFailure(std::shared_ptr<Call<std::shared_ptr<KeypleDto>>> call, std::runtime_error t) override;

protected:
                                    std::shared_ptr<CallbackAnonymousInnerClass> shared_from_this() {
                                        return std::static_pointer_cast<CallbackAnonymousInnerClass>(retrofit2.Callback<KeypleDto>::shared_from_this());
                                    }
                                };


                            private:
                                void stopPollingWorker();


                                void processHttpResponseDTO(std::shared_ptr<Response<std::shared_ptr<KeypleDto>>> response);


                            public:
                                void sendDTO(std::shared_ptr<TransportDto> transportDto) override;

                            private:
                                class CallbackAnonymousInnerClass2 : public Callback<std::shared_ptr<KeypleDto>> {
                                private:
                                    std::shared_ptr<WsPRetrofitClientImpl> outerInstance;

                                    std::shared_ptr<Call<std::shared_ptr<KeypleDto>>> call;

                                public:
                                    CallbackAnonymousInnerClass2(std::shared_ptr<WsPRetrofitClientImpl> outerInstance, std::shared_ptr<Call<std::shared_ptr<KeypleDto>>> call);

                                    void onResponse(std::shared_ptr<Call<std::shared_ptr<KeypleDto>>> call, std::shared_ptr<Response<std::shared_ptr<KeypleDto>>> response) override;

                                    void onFailure(std::shared_ptr<Call<std::shared_ptr<KeypleDto>>> call, std::runtime_error t) override;

protected:
                                    std::shared_ptr<CallbackAnonymousInnerClass2> shared_from_this() {
                                        return std::static_pointer_cast<CallbackAnonymousInnerClass2>(retrofit2.Callback<KeypleDto>::shared_from_this());
                                    }
                                };

                            public:
                                void sendDTO(std::shared_ptr<KeypleDto> message) override;

                                std::string getNodeId() override;

                                void update(std::shared_ptr<KeypleDto> event_Renamed) override;


                                /*
                                 * TransportNode
                                 */
                                void setDtoHandler(std::shared_ptr<DtoHandler> dtoHandler) override;


                                void connect() override;

                                void disconnect() override;



                                static std::shared_ptr<WsPRetrofitClient> getRseAPIClient(const std::string &baseUrl);

                            };

                        }
                    }
                }
            }
        }
    }
}
