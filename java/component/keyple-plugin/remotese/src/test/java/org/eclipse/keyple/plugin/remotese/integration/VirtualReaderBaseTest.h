#pragma once

#include <string>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class TransportFactory; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace nativese { class NativeReaderServiceImpl; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace stub { class StubReader; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace pluginse { class VirtualReader; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace pluginse { class VirtualReaderService; } } } } } }

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
                    namespace integration {


                        using NativeReaderServiceImpl = org::eclipse::keyple::plugin::remotese::nativese::NativeReaderServiceImpl;
                        using VirtualReader = org::eclipse::keyple::plugin::remotese::pluginse::VirtualReader;
                        using VirtualReaderService = org::eclipse::keyple::plugin::remotese::pluginse::VirtualReaderService;
                        using TransportFactory = org::eclipse::keyple::plugin::remotese::transport::TransportFactory;
                        using StubReader = org::eclipse::keyple::plugin::stub::StubReader;
                        using namespace org::junit;
                        using org::junit::rules::TestName;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        /**
                         * Test Virtual Reader Service with stub plugin and hoplink SE
                         */
                        class VirtualReaderBaseTest : public std::enable_shared_from_this<VirtualReaderBaseTest> {

                            // fail :
                            // transmitset : 0,1
                            // transmit : 0,2

                        public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Rule public org.junit.rules.TestName name = new org.junit.rules.TestName();
                            std::shared_ptr<TestName> name = std::make_shared<TestName>();

                        private:
                            static const std::shared_ptr<Logger> logger;

                            // Real objects
                            std::shared_ptr<TransportFactory> factory;
                            std::shared_ptr<NativeReaderServiceImpl> nativeReaderService;
                        public:
                            std::shared_ptr<StubReader> nativeReader;
                            std::shared_ptr<VirtualReader> virtualReader;

                            const std::string NATIVE_READER_NAME = "testStubReader";
                            const std::string CLIENT_NODE_ID = "testClientNodeId";

                            // Spy Object
                            std::shared_ptr<VirtualReaderService> virtualReaderService;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setTup() throws Exception
                            virtual void setTup() throw(std::runtime_error);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @After public void tearDown() throws Exception
                            virtual void tearDown() throw(std::runtime_error);



                        private:
                            std::shared_ptr<StubReader> connectStubReader(const std::string &readerName, const std::string &nodeId) throw(std::runtime_error);

                            void disconnectStubReader(std::shared_ptr<StubReader> nativeReader, const std::string &nodeId) throw(std::runtime_error);

                            std::shared_ptr<VirtualReader> getVirtualReader() throw(std::runtime_error);

                        };

                    }
                }
            }
        }
    }
}
