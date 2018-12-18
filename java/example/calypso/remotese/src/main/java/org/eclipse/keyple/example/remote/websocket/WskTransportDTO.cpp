#include "WskTransportDTO.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/KeypleDto.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/DtoSender.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace remote {
                    namespace websocket {
                        using DtoSender = org::eclipse::keyple::plugin::remotese::transport::DtoSender;
                        using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::KeypleDto;
                        using TransportDto = org::eclipse::keyple::plugin::remotese::transport::TransportDto;
                        using org::java_websocket::WebSocket;

                        std::shared_ptr<KeypleDto> WskTransportDTO::getDto() {
                            return dto;
                        }

                        WskTransportDTO::WskTransportDTO(std::shared_ptr<KeypleDto> dto, std::shared_ptr<WebSocket> socketWeb) : dto(dto), socketWeb(socketWeb) {
                        }

                        WskTransportDTO::WskTransportDTO(std::shared_ptr<KeypleDto> dto, std::shared_ptr<WebSocket> socketWeb, std::shared_ptr<DtoSender> wskNode) : dto(dto), socketWeb(socketWeb) {
                            this->wskNode = wskNode;
                        }

                        std::shared_ptr<KeypleDto> WskTransportDTO::getKeypleDTO() {
                            return dto;
                        }

                        std::shared_ptr<TransportDto> WskTransportDTO::nextTransportDTO(std::shared_ptr<KeypleDto> keypleDto) {

                            return std::make_shared<WskTransportDTO>(keypleDto, this->socketWeb);
                        }

                        std::shared_ptr<WebSocket> WskTransportDTO::getSocketWeb() {
                            return socketWeb;
                        }
                    }
                }
            }
        }
    }
}
