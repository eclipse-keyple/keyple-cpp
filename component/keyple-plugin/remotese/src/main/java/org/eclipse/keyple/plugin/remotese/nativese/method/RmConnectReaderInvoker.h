#pragma once

#include "../../rm/RemoteMethodInvoker.h"
#include <string>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class ProxyReader; } } } } }
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
                    namespace nativese {
                        namespace method {

                            using RemoteMethodInvoker = org::eclipse::keyple::plugin::remotese::rm::RemoteMethodInvoker;
                            using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::model::KeypleDto;
                            using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Deprecated public class RmConnectReaderInvoker implements org.eclipse.keyple.plugin.remotese.rm.RemoteMethodInvoker
                            class RmConnectReaderInvoker : public std::enable_shared_from_this<RmConnectReaderInvoker>, public RemoteMethodInvoker {

                            private:
                                const std::shared_ptr<ProxyReader> localReader;
                                const std::string clientNodeId;

                            public:
                                RmConnectReaderInvoker(std::shared_ptr<ProxyReader> localReader, const std::string &clientNodeId);

                                std::shared_ptr<KeypleDto> dto() override;
                            };

                        }
                    }
                }
            }
        }
    }
}
