#pragma once

#include <string>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class SeProxyService; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class SeReader; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleBaseException; } } } } }

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
                namespace generic_Renamed {
                    namespace pc {

                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using SeReader = org::eclipse::keyple::seproxy::SeReader;
                        using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;

                        class ReaderUtilities : public std::enable_shared_from_this<ReaderUtilities> {
                            /**
                             * Get the terminal which names match the expected pattern
                             *
                             * @param seProxyService SE Proxy service
                             * @param pattern Pattern
                             * @return SeReader
                             * @throws KeypleReaderException Readers are not initialized
                             */
                        public:
                            static std::shared_ptr<SeReader> getReaderByName(std::shared_ptr<SeProxyService> seProxyService, const std::string &pattern) throw(KeypleReaderException);

                            /**
                             * Get a fully configured contactless proxy reader
                             * 
                             * @param seProxyService the current SeProxyService
                             * @return the targeted SeReader to do contactless communications
                             * @throws KeypleBaseException in case of an error while retrieving the reader or setting its
                             *         parameters
                             */
                            static std::shared_ptr<SeReader> getDefaultContactLessSeReader(std::shared_ptr<SeProxyService> seProxyService) throw(KeypleBaseException);

                            /**
                             * Sets the reader parameters for contactless secure elements
                             * 
                             * @param reader the reader to configure
                             * @throws KeypleBaseException in case of an error while settings the parameters
                             */
                            static void setContactlessSettings(std::shared_ptr<SeReader> reader) throw(KeypleBaseException);

                            /**
                             * Sets the reader parameters for contacts secure elements
                             *
                             * @param reader the reader to configure
                             * @throws KeypleBaseException in case of an error while settings the parameters
                             */
                            static void setContactsSettings(std::shared_ptr<SeReader> reader) throw(KeypleBaseException);
                        };

                    }
                }
            }
        }
    }
}
