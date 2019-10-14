#include "WsPTransportDTO.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/KeypleDto.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/DtoSender.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace remote {
                    namespace wspolling {
                        using DtoSender = org::eclipse::keyple::plugin::remotese::transport::DtoSender;
                        using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::KeypleDto;
                        using TransportDto = org::eclipse::keyple::plugin::remotese::transport::TransportDto;

                        WsPTransportDTO::WsPTransportDTO(std::shared_ptr<KeypleDto> dto, std::shared_ptr<DtoSender> node) : dto(dto), node(node) {
                        }

                        std::shared_ptr<KeypleDto> WsPTransportDTO::getKeypleDTO() {
                            return dto;
                        }

                        std::shared_ptr<TransportDto> WsPTransportDTO::nextTransportDTO(std::shared_ptr<KeypleDto> keypleDto) {
                            return std::make_shared<WsPTransportDTO>(keypleDto, nullptr);
                        }
                    }
                }
            }
        }
    }
}
