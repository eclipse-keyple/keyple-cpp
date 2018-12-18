#pragma once

#include <string>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class ProxyReader; } } } } }

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


                        using KeypleRemoteException = org::eclipse::keyple::plugin::remotese::transport::KeypleRemoteException;
                        using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;

                        class NativeReaderService {


                            /**
                             * Connect Physical Local Reader to Remote SE Creates a Session to exchange data with this
                             * Reader with an option to duplex connection
                             */
                        public:
                            virtual void connectReader(std::shared_ptr<ProxyReader> localReader, const std::string &clientNodeId) = 0;

                            virtual void disconnectReader(std::shared_ptr<ProxyReader> localReader, const std::string &clientNodeId) = 0;


                        };

                    }
                }
            }
        }
    }
}
