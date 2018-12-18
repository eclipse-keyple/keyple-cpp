#pragma once

#include "../../transport/RemoteMethodInvoker.h"
#include <string>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class ProxyReader; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class KeypleDto; } } } } } }

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
                    namespace nativese {
                        namespace method {

                            using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::KeypleDto;
                            using RemoteMethodInvoker = org::eclipse::keyple::plugin::remotese::transport::RemoteMethodInvoker;
                            using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;

                            class RmDisconnectReaderInvoker : public std::enable_shared_from_this<RmDisconnectReaderInvoker>, public RemoteMethodInvoker {

                            private:
                                const std::shared_ptr<ProxyReader> localReader;
                                const std::string clientNodeId;

                            public:
                                RmDisconnectReaderInvoker(std::shared_ptr<ProxyReader> localReader, const std::string &clientNodeId);

                                std::shared_ptr<KeypleDto> dto() override;
                            };

                        }
                    }
                }
            }
        }
    }
}
