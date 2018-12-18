#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include "exceptionhelper.h"
#include <memory>

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

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings("PMD.AvoidUsingHardCodedIP") public class Server
                        class Server : public std::enable_shared_from_this<Server> {

                        private:
                            static const std::shared_ptr<Logger> logger;

                            const Integer PORT = 8004;
                            const std::string END_POINT = "/keypleDTO";
                            const std::string POLLING_END_POINT = "/polling";
                            const std::string URL = "0.0.0.0";



                            void boot() throw(IOException);


                            static void main(std::vector<std::string> &args) throw(std::runtime_error);
                        };

                    }
                }
            }
        }
    }
}
