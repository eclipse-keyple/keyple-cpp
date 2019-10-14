#include "Client.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/example/remote/wspolling/client/WsPClient.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/TransportDto.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/KeypleDtoHelper.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/example/remote/wspolling/WsPTransportDTO.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace remote {
                    namespace wspolling {
                        using WsPClient = org::eclipse::keyple::example::remote::wspolling::client::WsPClient;
                        using DtoHandler = org::eclipse::keyple::plugin::remotese::transport::DtoHandler;
                        using KeypleDtoHelper = org::eclipse::keyple::plugin::remotese::transport::KeypleDtoHelper;
                        using TransportDto = org::eclipse::keyple::plugin::remotese::transport::TransportDto;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> Client::logger = org::slf4j::LoggerFactory::getLogger(Client::typeid);

                        void Client::boot() {


                            std::shared_ptr<WsPClient> client = std::make_shared<WsPClient>(BASE_URL, ENDPOINT_URL, POLLING_URL, "test1");
                            client->startPollingWorker("node1");
                            client->setDtoHandler(std::make_shared<DtoHandlerAnonymousInnerClass>(shared_from_this()));

                        }

                        Client::DtoHandlerAnonymousInnerClass::DtoHandlerAnonymousInnerClass(std::shared_ptr<Client> outerInstance) {
                            this->outerInstance = outerInstance;
                        }

                        std::shared_ptr<TransportDto> Client::DtoHandlerAnonymousInnerClass::onDTO(std::shared_ptr<TransportDto> message) {
                            return std::make_shared<WsPTransportDTO>(KeypleDtoHelper::NoResponse(), nullptr);
                        }

                        void Client::main(std::vector<std::string> &args) {
                            std::shared_ptr<Client> client = std::make_shared<Client>();
                            client->boot();
                        }
                    }
                }
            }
        }
    }
}
