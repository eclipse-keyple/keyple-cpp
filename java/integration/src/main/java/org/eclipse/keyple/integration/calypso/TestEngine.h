#pragma once

#include <string>
#include <iostream>
#include <stdexcept>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class SeReader; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace integration { namespace calypso { class PoFileStructureInfo; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class SeProxyService; } } } }
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
            namespace integration {
                namespace calypso {

                    using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                    using SeReader = org::eclipse::keyple::seproxy::SeReader;
                    using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;

                    class TestEngine : public std::enable_shared_from_this<TestEngine> {

                    public:
                        static std::shared_ptr<SeReader> poReader, samReader;

                        static std::shared_ptr<PoFileStructureInfo> selectPO() throw(std::invalid_argument, KeypleReaderException);

                    private:
                        static std::shared_ptr<SeReader> getReader(std::shared_ptr<SeProxyService> seProxyService, const std::string &pattern) throw(KeypleReaderException);

                    public:
                        static void configureReaders() throw(IOException, InterruptedException, KeypleBaseException);

                    };

                }
            }
        }
    }
}
