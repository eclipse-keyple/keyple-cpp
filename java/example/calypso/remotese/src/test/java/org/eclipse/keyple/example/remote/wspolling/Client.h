#pragma once

#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/DtoHandler.h"
#include <string>
#include <vector>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
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
                    namespace wspolling {

                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        class Client : public std::enable_shared_from_this<Client> {

                        private:
                            static const std::shared_ptr<Logger> logger;

                            const std::string BASE_URL = "http://localhost:8004";
                            const std::string ENDPOINT_URL = "/keypleDTO";
                            const std::string POLLING_URL = "/polling";



                            void boot();

                        private:
                            class DtoHandlerAnonymousInnerClass : public std::enable_shared_from_this<DtoHandlerAnonymousInnerClass>, public DtoHandler {
                            private:
                                std::shared_ptr<Client> outerInstance;

                            public:
                                DtoHandlerAnonymousInnerClass(std::shared_ptr<Client> outerInstance);

                                std::shared_ptr<TransportDto> onDTO(std::shared_ptr<TransportDto> message);
                            };


                            static void main(std::vector<std::string> &args);


                        };

                    }
                }
            }
        }
    }
}
