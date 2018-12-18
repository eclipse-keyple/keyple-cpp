#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleBaseException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class NoStackTraceThrowable; } } } } }

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



                            using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                            using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;

                            class Demo_WriteName : public std::enable_shared_from_this<Demo_WriteName> {

                            private:
                                static const std::shared_ptr<Logger> logger;

                                static void main(std::vector<std::string> &args) throw(KeypleBaseException, NoStackTraceThrowable);
                            };

                        }
                    }
                }
            }
        }
    }
}
