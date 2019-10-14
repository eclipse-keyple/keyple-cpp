#pragma once

#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace model { class KeypleDto; } } } } } } }

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
                        namespace model {


                            /**
                             * Message DTO with an embedded KeypleDto enriched with common layer information For instance, can
                             * embed web socket connection information
                             */
                            class TransportDto {


                                /**
                                 * Retrieve the embedded Keyple DTO
                                 * 
                                 * @return embedded Keyple DTO
                                 */
                            public:
                                virtual std::shared_ptr<KeypleDto> getKeypleDTO() = 0;

                                /**
                                 * Embed a Keyple DTO into a new TransportDto with transport information
                                 * 
                                 * @param keypleDto : keyple DTO to be embedded
                                 * @return Transport DTO with embedded keyple DTO
                                 */
                                virtual std::shared_ptr<TransportDto> nextTransportDTO(std::shared_ptr<KeypleDto> keypleDto) = 0;


                                /**
                                 * Get the sender Object to send back a response if needed
                                 * 
                                 * @return DtoSender if any
                                 */
                                // not in used
                                // DtoSender getDtoSender();



                            };

                        }
                    }
                }
            }
        }
    }
}
