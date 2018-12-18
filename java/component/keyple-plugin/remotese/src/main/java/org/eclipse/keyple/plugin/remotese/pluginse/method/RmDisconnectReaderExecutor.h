#pragma once

#include <string>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace pluginse { class RemoteSePlugin; } } } } } }

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
                    namespace pluginse {
                        namespace method {

                            using RemoteSePlugin = org::eclipse::keyple::plugin::remotese::pluginse::RemoteSePlugin;
                            using namespace org::eclipse::keyple::plugin::remotese::transport;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;


                            class RmDisconnectReaderExecutor : public std::enable_shared_from_this<RmDisconnectReaderExecutor>, public RemoteMethodExecutor {

                            private:
                                static const std::shared_ptr<Logger> logger;


                                const std::shared_ptr<RemoteSePlugin> plugin;

                            public:
                                RmDisconnectReaderExecutor(std::shared_ptr<RemoteSePlugin> plugin);


                                std::shared_ptr<TransportDto> execute(std::shared_ptr<TransportDto> transportDto) override;
                            };

                        }
                    }
                }
            }
        }
    }
}
