#include "WsPClient.h"
#include "../WsPTransportDTO.h"
#include "../../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/TransportDto.h"
#include "../../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/json/JsonParser.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace remote {
                    namespace wspolling {
                        namespace client {
                            using WsPTransportDTO = org::eclipse::keyple::example::remote::wspolling::WsPTransportDTO;
                            using namespace org::eclipse::keyple::plugin::remotese::transport;
                            using JsonParser = org::eclipse::keyple::plugin::remotese::transport::json::JsonParser;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;
                            using com::google::gson::Gson;
                            using com::google::gson::JsonObject;
const std::shared_ptr<org::slf4j::Logger> WsPClient::logger = org::slf4j::LoggerFactory::getLogger(WsPClient::typeid);

                            WsPClient::WsPClient(const std::string &baseUrl, const std::string &keypleDtoEndpoint, const std::string &pollingEndpoint, const std::string &nodeId) : keypleDtoEndpoint(keypleDtoEndpoint), pollingEndpoint(pollingEndpoint), nodeId(nodeId), baseUrl(baseUrl) {
                            }

                            void WsPClient::startPollingWorker(const std::string &nodeId) {

                                logger->info("Start Polling Worker {}", nodeId);

                                std::shared_ptr<Thread> pollThread = std::make_shared<ThreadAnonymousInnerClass>(shared_from_this(), nodeId);

                                pollThread->start();

                            }

                            WsPClient::ThreadAnonymousInnerClass::ThreadAnonymousInnerClass(std::shared_ptr<WsPClient> outerInstance, const std::string &nodeId) {
                                this->outerInstance = outerInstance;
                                this->nodeId = nodeId;
                            }

                            void WsPClient::ThreadAnonymousInnerClass::run() {
                                // Boolean exit = false;
                                while (true) {
                                    try {
                                        logger->debug("Polling clientNodeId {}", nodeId);
                                        std::shared_ptr<JsonObject> httpResponse = outerInstance->httpPoll(outerInstance->getConnection(outerInstance->baseUrl + outerInstance->pollingEndpoint + "?clientNodeId=" + nodeId), "{}");
                                        logger->debug("Polling for clientNodeId {} receive a httpResponse {}", nodeId, httpResponse);
                                        outerInstance->processHttpResponseDTO(httpResponse);
                                    }
                                    catch (const IOException &e) {
                                        logger->debug("Polling for clientNodeId {} didn't receive any response, send it again ");
                                        // e.printStackTrace();
                                    }
                                }
                            }

                            void WsPClient::processHttpResponseDTO(std::shared_ptr<JsonObject> httpResponse) {

                                // is response DTO ?
                                if (KeypleDtoHelper::isKeypleDTO(httpResponse)) {

                                    std::shared_ptr<KeypleDto> responseDTO = KeypleDtoHelper::fromJsonObject(httpResponse);
                                    std::shared_ptr<TransportDto> transportDto = std::make_shared<WsPTransportDTO>(responseDTO, shared_from_this());
                                    // connection
                                    std::shared_ptr<TransportDto> * const sendback = this->dtoHandler->onDTO(transportDto);

                                    // if sendBack is not a noResponse (can be a keyple request or keyple response)
                                    if (!KeypleDtoHelper::isNoResponse(sendback->getKeypleDTO())) {
                                        // send the keyple object in a new thread to avoid blocking the polling
                                        std::make_shared<ThreadAnonymousInnerClass2>(shared_from_this(), sendback)
                                        .start();
                                    }
                                }

                            }

                            WsPClient::ThreadAnonymousInnerClass2::ThreadAnonymousInnerClass2(std::shared_ptr<WsPClient> outerInstance, std::shared_ptr<org::eclipse::keyple::plugin::remotese::transport::TransportDto> sendback) {
                                this->outerInstance = outerInstance;
                                this->sendback = sendback;
                            }

                            void WsPClient::ThreadAnonymousInnerClass2::run() {
                                outerInstance->sendDTO(sendback);
                            }

                            void WsPClient::sendDTO(std::shared_ptr<TransportDto> transportDto) {
                                std::shared_ptr<KeypleDto> keypleDto = transportDto->getKeypleDTO();
                                logger->debug("Ws Client send DTO {}", KeypleDtoHelper::toJson(keypleDto));
                                if (!KeypleDtoHelper::isNoResponse(transportDto->getKeypleDTO())) {
                                    try {
                                        // send keyple dto
                                        std::shared_ptr<JsonObject> httpResponse = httpPOSTJson(getConnection(baseUrl + keypleDtoEndpoint), KeypleDtoHelper::toJson(keypleDto));

                                        processHttpResponseDTO(httpResponse);

                                    }
                                    catch (const IOException &e) {
                                        e->printStackTrace();
                                        // todo manage exception or throw it
                                    }
                                }
                            }

                            void WsPClient::sendDTO(std::shared_ptr<KeypleDto> message) {
                                sendDTO(std::make_shared<WsPTransportDTO>(message, nullptr));
                            }

                            void WsPClient::update(std::shared_ptr<KeypleDto> event_Renamed) {
                                this->sendDTO(event_Renamed);
                            }

                            void WsPClient::setDtoHandler(std::shared_ptr<DtoHandler> dtoHandler) {
                                this->dtoHandler = dtoHandler;
                            }

                            std::string WsPClient::getNodeId() {
                                return nodeId;
                            }

                            void WsPClient::connect() {
                                this->startPollingWorker(nodeId);
                            }

                            void WsPClient::disconnect() {
                                // todo
                            }

                            std::shared_ptr<JsonObject> WsPClient::httpPOSTJson(std::shared_ptr<HttpURLConnection> conn, const std::string &json) throw(IOException) {
                                logger->trace("Url {} HTTP POST  : {} ", conn->getURL(), json);
                                // Encode data
                                conn->setRequestMethod("POST");
                                conn->setDoOutput(true);
                                conn->setRequestProperty("Content-Type", "application/json; charset=UTF-8");
                                conn->setRequestProperty("Accept", "application/json");
                                conn->connect();

                                std::shared_ptr<OutputStreamWriter> osw = std::make_shared<OutputStreamWriter>(conn->getOutputStream());
                                osw->write(json);
                                osw->flush();
                                osw->close();

                                conn->setConnectTimeout(70000);
                                conn->setReadTimeout(70000);


                                int responseCode = conn->getResponseCode();
                                logger->trace("Response code {}", responseCode);
                                std::shared_ptr<JsonObject> jsonObject = parseBody(std::static_pointer_cast<InputStream>(conn->getContent()));
                                logger->trace("Response {}", jsonObject);
                                return jsonObject;
                            }

                            std::shared_ptr<HttpURLConnection> WsPClient::getConnection(const std::string &urlString) throw(IOException) {
                                std::shared_ptr<URL> url = std::make_shared<URL>(urlString);
                                return std::static_pointer_cast<HttpURLConnection>(url->openConnection());
                            }

                            std::shared_ptr<JsonObject> WsPClient::httpPoll(std::shared_ptr<HttpURLConnection> conn, const std::string &json) throw(IOException) {
                                logger->trace("Url {} HTTP GET  : {} ", conn->getURL(), json);
                                // Encode data
                                conn->setRequestMethod("GET");
                                conn->setDoOutput(true);
                                conn->setRequestProperty("Content-Type", "application/json; charset=UTF-8");
                                conn->setRequestProperty("Accept", "application/json");
                                conn->connect();

                                conn->setConnectTimeout(10000);
                                conn->setReadTimeout(10000);

                                int responseCode = conn->getResponseCode();
                                logger->trace("Response code {}", responseCode);
                                std::shared_ptr<JsonObject> jsonObject = parseBody(std::static_pointer_cast<InputStream>(conn->getContent()));
                                logger->trace("Response {}", jsonObject);
                                return jsonObject;
                            }

                            std::shared_ptr<JsonObject> WsPClient::parseBody(std::shared_ptr<InputStream> is) {
                                std::shared_ptr<Scanner> s = (std::make_shared<Scanner>(is))->useDelimiter("\\A");
                                std::string result = s->hasNext() ? s->next() : "";
                                std::shared_ptr<Gson> gson = JsonParser::getGson();
                                return gson->fromJson(result, JsonObject::typeid);
                            }
                        }
                    }
                }
            }
        }
    }
}
