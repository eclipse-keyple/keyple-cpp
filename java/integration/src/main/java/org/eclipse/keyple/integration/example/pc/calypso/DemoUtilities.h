#pragma once

#include <string>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class SeProxyService; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class SeReader; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderException; } } } } }

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
            namespace integration {
                namespace example {
                    namespace pc {
                        namespace calypso {

                            using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                            using SeReader = org::eclipse::keyple::seproxy::SeReader;
                            using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;

                            class DemoUtilities : public std::enable_shared_from_this<DemoUtilities> {

                            public:
                                static std::string PO_READER_NAME_REGEX;
                                static std::string SAM_READER_NAME_REGEX;

                                /**
                                 * Get the terminals with names that match the expected pattern
                                 *
                                 * @param seProxyService SE Proxy service
                                 * @param pattern Pattern
                                 * @return SeReader
                                 * @throws KeypleReaderException Any error with the card communication
                                 */
                                static std::shared_ptr<SeReader> getReader(std::shared_ptr<SeProxyService> seProxyService, const std::string &pattern) throw(KeypleReaderException);


                            };

                        }
                    }
                }
            }
        }
    }
}
