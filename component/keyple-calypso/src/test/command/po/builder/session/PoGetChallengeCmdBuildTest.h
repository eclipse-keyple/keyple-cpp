#pragma once

#include <vector>
#include <memory>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "PoClass.h"
#include "PoGetChallengeCmdBuild.h"
#include "AbstractApduCommandBuilder.h"
#include "ApduRequest.h"

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



                                class PoGetChallengeCmdBuildTest : public std::enable_shared_from_this<PoGetChallengeCmdBuildTest> {
                                public:

                                    virtual void POGetChallenge_Rev2_4();

                                    virtual void POGetChallenge_Rev3_1();

                                    virtual void POGetChallenge_Rev3_2();
                                };
                            }
                        }
                    }
                }
            }
        }
