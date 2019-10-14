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
                    namespace rm {


                        using KeypleRemoteReaderException = org::eclipse::keyple::plugin::remotese::exception::KeypleRemoteReaderException;
                        using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::model::KeypleDto;

                        template<typename T>
                        class RemoteMethodParser {

                        public:
                            virtual T parseResponse(std::shared_ptr<KeypleDto> keypleDto) = 0;
                        };

                    }
                }
            }
        }
    }
}
