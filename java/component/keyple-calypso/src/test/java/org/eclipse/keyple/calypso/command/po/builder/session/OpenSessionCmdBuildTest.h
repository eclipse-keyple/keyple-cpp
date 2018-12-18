#pragma once

#include <vector>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace command { class AbstractApduCommandBuilder; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class ApduRequest; } } } } }

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
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace builder {
                            namespace session {



                                using AbstractApduCommandBuilder = org::eclipse::keyple::command::AbstractApduCommandBuilder;
                                using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @RunWith(MockitoJUnitRunner.class) public class OpenSessionCmdBuildTest
                                class OpenSessionCmdBuildTest : public std::enable_shared_from_this<OpenSessionCmdBuildTest> {

                                private:
                                    std::vector<char> const samChallenge = std::vector<char> {static_cast<char>(0xA8), 0x31, static_cast<char>(0xC3), 0x3E};

                                    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder;

                                    std::shared_ptr<ApduRequest> apduRequest;


                                public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalArgumentException.class) public void openSessionCmdBuild_rev_2_4_exception() throws IllegalArgumentException
                                    virtual void openSessionCmdBuild_rev_2_4_exception() throw(std::invalid_argument);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void openSessionCmdBuild_rev_2_4() throws IllegalArgumentException
                                    virtual void openSessionCmdBuild_rev_2_4() throw(std::invalid_argument);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void openSessionCmdBuild_rev_3_1() throws IllegalArgumentException
                                    virtual void openSessionCmdBuild_rev_3_1() throw(std::invalid_argument);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void openSessionCmdBuild_rev_3_2() throws IllegalArgumentException
                                    virtual void openSessionCmdBuild_rev_3_2() throw(std::invalid_argument);
                                };

                            }
                        }
                    }
                }
            }
        }
    }
}
