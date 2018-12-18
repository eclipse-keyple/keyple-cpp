#pragma once

#include "../TransportDto.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class KeypleDto; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace java { class LocalClient; } } } } } } }
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
            namespace plugin {
                namespace remotese {
                    namespace transport {
                        namespace java {

                            using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::KeypleDto;
                            using TransportDto = org::eclipse::keyple::plugin::remotese::transport::TransportDto;

                            class LocalTransportDto : public std::enable_shared_from_this<LocalTransportDto>, public TransportDto {

                            private:
                                const std::shared_ptr<KeypleDto> keypleDto;
                                const std::shared_ptr<LocalClient> theClient;


                            public:
                                LocalTransportDto(std::shared_ptr<KeypleDto> keypleDto, std::shared_ptr<LocalClient> theClient);

                                virtual std::shared_ptr<LocalClient> getTheClient();

                                std::shared_ptr<KeypleDto> getKeypleDTO() override;

                                std::shared_ptr<TransportDto> nextTransportDTO(std::shared_ptr<KeypleDto> keypleDto) override;


                            };

                        }
                    }
                }
            }
        }
    }
}
