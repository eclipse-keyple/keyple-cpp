#pragma once

#include "../DtoSender.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class DtoHandler; } } } } } }

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
                        namespace factory {


                            using DtoHandler = org::eclipse::keyple::plugin::remotese::transport::DtoHandler;
                            using DtoSender = org::eclipse::keyple::plugin::remotese::transport::DtoSender;

                            /**
                             * TransportNode is a one-point gateway for incoming and outgoing TransportDto. It extend DtoSender
                             * thus sends KeypleDto and contains a DtoHandler for incoming KeypleDto
                             */
                            class TransportNode : public DtoSender {

                            public:
                                virtual void setDtoHandler(std::shared_ptr<DtoHandler> handler) = 0;

                            };

                        }
                    }
                }
            }
        }
    }
}
