#pragma once

#include <vector>
#include <stdexcept>
#include <memory>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

/* Common */
#include "System.h"

#include "AbstractApduCommandBuilder.h"
#include "ApduRequest.h"
#include "PoRevision.h"
#include "AbstractOpenSessionCmdBuild.h"
#include "AbstractOpenSessionRespPars.h"
#include "ByteArrayUtil.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace keyple { namespace core { namespace command { class AbstractApduCommandBuilder; } } }
namespace keyple { namespace core { namespace seproxy { namespace message { class ApduRequest; } } } }
namespace keyple { namespace calypso { namespace command { namespace po { namespace parser { namespace security { class AbstractOpenSessionRespPars; }}}}}}

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
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace builder {
                            namespace security {

                                using AbstractApduCommandBuilder = keyple::core::command::AbstractApduCommandBuilder;
                                using ApduRequest = keyple::core::seproxy::message::ApduRequest;

                                class OpenSessionCmdBuildTest : public std::enable_shared_from_this<OpenSessionCmdBuildTest> {

                                private:
                                    std::vector<char> samChallenge = std::vector<char> {static_cast<char>(0xA8), 0x31, static_cast<char>(0xC3), 0x3E};

                                    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder;

                                    std::shared_ptr<ApduRequest> apduRequest;


                                public:
                                    virtual void openSessionCmdBuild_rev_2_4_exception();

                                    virtual void openSessionCmdBuild_rev_2_4();

                                    virtual void openSessionCmdBuild_rev_3_1();

                                    virtual void openSessionCmdBuild_rev_3_2();
                                };

                            }
                        }
                    }
                }
            }
        }
