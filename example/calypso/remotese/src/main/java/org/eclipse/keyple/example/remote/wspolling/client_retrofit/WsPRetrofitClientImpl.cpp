#include "WsPRetrofitClientImpl.h"
#include "../WsPTransportDTO.h"
#include "WsPRetrofitClient.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace remote {
                    namespace wspolling {
                        namespace client_retrofit {
                            using WsPTransportDTO = org::eclipse::keyple::example::remote::wspolling::WsPTransportDTO;
                            using namespace org::eclipse::keyple::plugin::remotese::transport;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;
                            using okhttp3::OkHttpClient;
                            using retrofit2::Call;
                            using retrofit2::Callback;
                            using retrofit2::Response;
                            using retrofit2::Retrofit;
                            using retrofit2::converter::gson::GsonConverterFactory;
const std::shared_ptr<org::slf4j::Logger> WsPRetrofitClientImpl::logger = org::slf4j::LoggerFactory::getLogger(WsPRetrofitClientImpl::typeid);

                            WsPRetrofitClientImpl::WsPRetrofitClientImpl(const std::string &baseUrl, const std::string &nodeId) : baseUrl(baseUrl), nodeId(nodeId) {
                            }

                            void WsPRetrofitClientImpl::startPollingWorker(const std::string &nodeId) {

                                logger->trace("Polling clientNodeId {}", nodeId);
                                this->poll_Renamed = true;
                                poll(nodeId);
                            }

                            void WsPRetrofitClientImpl::poll(const std::string &nodeId) {
                                // if poll is activated
                                if (this->poll_Renamed) {
                                    std::shared_ptr<Call<std::shared_ptr<KeypleDto>>> call = getRseAPIClient(baseUrl)->getPolling(nodeId);
                                    call->enqueue(std::make_shared<CallbackAnonymousInnerClass>(shared_from_this(), nodeId, call));
                                }
                                else {
                                    logger->debug("poll is not active, call startPollingWorker to activate again");
                                    // poll is not active, call startPollingWorker to activate again
                                }
                            }

                            WsPRetrofitClientImpl::CallbackAnonymousInnerClass::CallbackAnonymousInnerClass(std::shared_ptr<WsPRetrofitClientImpl> outerInstance, const std::string &nodeId, std::shared_ptr<Call<std::shared_ptr<KeypleDto>>> call) {
                                this->outerInstance = outerInstance;
                                this->nodeId = nodeId;
                                this->call = call;
                            }

                            void WsPRetrofitClientImpl::CallbackAnonymousInnerClass::onResponse(std::shared_ptr<Call<std::shared_ptr<KeypleDto>>> call, std::shared_ptr<Response<std::shared_ptr<KeypleDto>>> response) {
                                int statusCode = response->code();
                                logger->trace("Polling for clientNodeId {} receive a httpResponse http code {}", nodeId, statusCode);
                                outerInstance->processHttpResponseDTO(response);
                                outerInstance->poll(nodeId); // recursive call to restart polling
                            }

                            void WsPRetrofitClientImpl::CallbackAnonymousInnerClass::onFailure(std::shared_ptr<Call<std::shared_ptr<KeypleDto>>> call, std::runtime_error t) {
                                // Log error here since request failed
                                logger->debug("polling ends, start it over, error : " + t.what());
                                outerInstance->poll(nodeId); // recursive call to restart polling
                            }

                            void WsPRetrofitClientImpl::stopPollingWorker() {
                                this->poll_Renamed = false;
                            }

                            void WsPRetrofitClientImpl::processHttpResponseDTO(std::shared_ptr<Response<std::shared_ptr<KeypleDto>>> response) {

                                std::shared_ptr<KeypleDto> responseDTO = response->body();

                                if (!KeypleDtoHelper::isNoResponse(responseDTO)) {
                                    std::shared_ptr<TransportDto> transportDto = std::make_shared<WsPTransportDTO>(responseDTO, shared_from_this());
                                    // connection
                                    std::shared_ptr<TransportDto> * const sendback = this->dtoHandler->onDTO(transportDto);

                                    // if sendBack is not a noresponse (can be a keyple request or keyple response)
                                    if (!KeypleDtoHelper::isNoResponse(sendback->getKeypleDTO())) {
                                        // send the keyple object in a new thread to avoid blocking the polling

                                        sendDTO(sendback);

                                    }
                                }


                            }

                            void WsPRetrofitClientImpl::sendDTO(std::shared_ptr<TransportDto> transportDto) {
                                std::shared_ptr<KeypleDto> keypleDto = transportDto->getKeypleDTO();
                                logger->debug("Ws Client send DTO {}", KeypleDtoHelper::toJson(keypleDto));

                                if (!KeypleDtoHelper::isNoResponse(transportDto->getKeypleDTO())) {

                                    std::shared_ptr<Call<std::shared_ptr<KeypleDto>>> call = getRseAPIClient(baseUrl)->postDto(keypleDto);
                                    call->enqueue(std::make_shared<CallbackAnonymousInnerClass2>(shared_from_this(), call));

                                }
                            }

                            WsPRetrofitClientImpl::CallbackAnonymousInnerClass2::CallbackAnonymousInnerClass2(std::shared_ptr<WsPRetrofitClientImpl> outerInstance, std::shared_ptr<Call<std::shared_ptr<KeypleDto>>> call) {
                                this->outerInstance = outerInstance;
                                this->call = call;
                            }

                            void WsPRetrofitClientImpl::CallbackAnonymousInnerClass2::onResponse(std::shared_ptr<Call<std::shared_ptr<KeypleDto>>> call, std::shared_ptr<Response<std::shared_ptr<KeypleDto>>> response) {
                                int statusCode = response->code();
                                logger->trace("Polling for clientNodeId {} receive a httpResponse http code {}", outerInstance->nodeId, statusCode);
                                outerInstance->processHttpResponseDTO(response);
                            }

                            void WsPRetrofitClientImpl::CallbackAnonymousInnerClass2::onFailure(std::shared_ptr<Call<std::shared_ptr<KeypleDto>>> call, std::runtime_error t) {
                                // Log error here since request failed
                                logger->debug("polling ends, start it over" + t.what());
                                outerInstance->startPollingWorker(outerInstance->nodeId);
                            }

                            void WsPRetrofitClientImpl::sendDTO(std::shared_ptr<KeypleDto> message) {
                                sendDTO(std::make_shared<WsPTransportDTO>(message, nullptr));
                            }

                            std::string WsPRetrofitClientImpl::getNodeId() {
                                return this->nodeId;
                            }

                            void WsPRetrofitClientImpl::update(std::shared_ptr<KeypleDto> event_Renamed) {
                                this->sendDTO(event_Renamed);
                            }

                            void WsPRetrofitClientImpl::setDtoHandler(std::shared_ptr<DtoHandler> dtoHandler) {
                                this->dtoHandler = dtoHandler;
                            }

                            void WsPRetrofitClientImpl::connect() {
                                this->startPollingWorker(nodeId);
                            }

                            void WsPRetrofitClientImpl::disconnect() {
                                this->stopPollingWorker();
                            }

                            std::shared_ptr<WsPRetrofitClient> WsPRetrofitClientImpl::getRseAPIClient(const std::string &baseUrl) {

                                std::shared_ptr<OkHttpClient> * const okHttpClient = (std::make_shared<OkHttpClient::Builder>())->connectTimeout(20, TimeUnit::SECONDS).writeTimeout(20, TimeUnit::SECONDS).readTimeout(30, TimeUnit::SECONDS).build();

                                std::shared_ptr<Retrofit> retrofit = (std::make_shared<Retrofit::Builder>())->client(okHttpClient).baseUrl(baseUrl).addConverterFactory(GsonConverterFactory::create()).build();

                                return retrofit->create(WsPRetrofitClient::typeid);
                            }
                        }
                    }
                }
            }
        }
    }
}
