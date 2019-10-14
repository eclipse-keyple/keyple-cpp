#include "LocalTransportDto.h"
#include "../../model/KeypleDto.h"
#include "LocalClient.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace transport {
                        namespace impl {
                            namespace java {
                                using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::model::KeypleDto;
                                using TransportDto = org::eclipse::keyple::plugin::remotese::transport::model::TransportDto;

                                LocalTransportDto::LocalTransportDto(std::shared_ptr<KeypleDto> keypleDto, std::shared_ptr<LocalClient> theClient) : keypleDto(keypleDto), theClient(theClient) {
                                }

                                std::shared_ptr<LocalClient> LocalTransportDto::getTheClient() {
                                    return theClient;
                                }

                                std::shared_ptr<KeypleDto> LocalTransportDto::getKeypleDTO() {
                                    return keypleDto;
                                }

                                std::shared_ptr<TransportDto> LocalTransportDto::nextTransportDTO(std::shared_ptr<KeypleDto> keypleDto) {
                                    return std::make_shared<LocalTransportDto>(keypleDto, theClient);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
