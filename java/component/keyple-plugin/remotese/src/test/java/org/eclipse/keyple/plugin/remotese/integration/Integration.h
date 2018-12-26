#pragma once

#include <string>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace pluginse { class VirtualReaderService; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class TransportNode; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace nativese { class NativeReaderServiceImpl; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderNotFoundException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace stub { class StubReader; } } } } }
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
            namespace plugin {
                namespace remotese {
                    namespace integration {


                        using NativeReaderServiceImpl = org::eclipse::keyple::plugin::remotese::nativese::NativeReaderServiceImpl;
                        using VirtualReaderService = org::eclipse::keyple::plugin::remotese::pluginse::VirtualReaderService;
                        using TransportDto = org::eclipse::keyple::plugin::remotese::transport::TransportDto;
                        using TransportNode = org::eclipse::keyple::plugin::remotese::transport::TransportNode;
                        using StubReader = org::eclipse::keyple::plugin::stub::StubReader;
                        using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                        using org::mockito::stubbing::Answer;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        class Integration : public std::enable_shared_from_this<Integration> {

                        private:
                            static const std::shared_ptr<Logger> logger;


                            /**
                             * Create a Virtual Reader Service
                             * 
                             * @param node
                             * @return
                             */
                        public:
                            static std::shared_ptr<VirtualReaderService> bindMaster(std::shared_ptr<TransportNode> node);

                            /**
                             * Create a Native Reader Service
                             * 
                             * @param node
                             * @return
                             */
                            static std::shared_ptr<NativeReaderServiceImpl> bindSlave(std::shared_ptr<TransportNode> node);

                            /**
                             * Create a Spy Native Reader Service
                             * 
                             * @param node
                             * @return
                             */
                            static std::shared_ptr<NativeReaderServiceImpl> bindSlaveSpy(std::shared_ptr<TransportNode> node);

                            /**
                             * Create a Stub reader
                             * 
                             * @param stubReaderName
                             * @return
                             * @throws InterruptedException
                             * @throws KeypleReaderNotFoundException
                             */
                            static std::shared_ptr<StubReader> createStubReader(const std::string &stubReaderName) throw(InterruptedException, KeypleReaderNotFoundException);

                            /**
                             * Create a mock method for onDto() that checks that keypleDto contains an exception
                             * 
                             * @return
                             */
                            static std::shared_ptr<Answer<std::shared_ptr<TransportDto>>> assertContainsException();

                        private:
                            class AnswerAnonymousInnerClass : public Answer<std::shared_ptr<TransportDto>> {
                            public:
                                std::shared_ptr<TransportDto> answer(std::shared_ptr<InvocationOnMock> invocationOnMock) throw(std::runtime_error) override;

protected:
                                std::shared_ptr<AnswerAnonymousInnerClass> shared_from_this() {
                                    return std::static_pointer_cast<AnswerAnonymousInnerClass>(org.mockito.stubbing.Answer<org.eclipse.keyple.plugin.remotese.transport.TransportDto>::shared_from_this());
                                }
                            };

                        };

                    }
                }
            }
        }
    }
}
