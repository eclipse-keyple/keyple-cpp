#pragma once

#include <string>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class ProxyReader; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class SeReader; } } } }

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


                        using SeReader = org::eclipse::keyple::seproxy::SeReader;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                        using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;

                        class NativeReaderService {


                            /**
                             * Connect Physical Local Reader to Remote SE Creates a Session to exchange data with this
                             * Reader with an option to duplex connection
                             */
                        public:
                            virtual std::string connectReader(std::shared_ptr<ProxyReader> localReader, const std::string &clientNodeId) = 0;

                            /**
                             * Disconnect Physical Local Reader from Remote Se Master Server
                             */
                            virtual void disconnectReader(const std::string &sessionId, const std::string &nativeReaderName, const std::string &clientNodeId) = 0;

                            /**
                             * Find a local reader accross plugins
                             * 
                             * @param nativeReaderName
                             * @return
                             * @throws KeypleReaderNotFoundException
                             */
                            virtual std::shared_ptr<SeReader> findLocalReader(const std::string &nativeReaderName) = 0;

                        };

                    }
                }
            }
        }
    }
}
