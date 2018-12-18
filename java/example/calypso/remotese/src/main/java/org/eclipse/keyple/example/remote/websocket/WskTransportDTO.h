#pragma once

#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/TransportDto.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class KeypleDto; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class DtoSender; } } } } } }
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
                    namespace websocket {

                        using DtoSender = org::eclipse::keyple::plugin::remotese::transport::DtoSender;
                        using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::KeypleDto;
                        using TransportDto = org::eclipse::keyple::plugin::remotese::transport::TransportDto;
                        using org::java_websocket::WebSocket;

                        /**
                         * Web socket Transport DTO
                         */
                        class WskTransportDTO : public std::enable_shared_from_this<WskTransportDTO>, public TransportDto {

                        public:
                            virtual std::shared_ptr<KeypleDto> getDto();


                        private:
                            const std::shared_ptr<KeypleDto> dto;
                            const std::shared_ptr<WebSocket> socketWeb;
                            std::shared_ptr<DtoSender> wskNode;


                        public:
                            WskTransportDTO(std::shared_ptr<KeypleDto> dto, std::shared_ptr<WebSocket> socketWeb);

                            WskTransportDTO(std::shared_ptr<KeypleDto> dto, std::shared_ptr<WebSocket> socketWeb, std::shared_ptr<DtoSender> wskNode);

                            std::shared_ptr<KeypleDto> getKeypleDTO() override;

                            std::shared_ptr<TransportDto> nextTransportDTO(std::shared_ptr<KeypleDto> keypleDto) override;

                            virtual std::shared_ptr<WebSocket> getSocketWeb();


                        };

                    }
                }
            }
        }
    }
}
