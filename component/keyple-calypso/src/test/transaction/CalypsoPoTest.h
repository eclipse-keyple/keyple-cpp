#pragma once

#include <string>
#include <memory>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "CalypsoPo.h"
#include "ApduResponse.h"
#include "ByteArrayUtil.h"
#include "SelectionStatus.h"
#include "PoSelectionRequest.h"
#include "ChannelState.h"
#include "SeSelector_Import.h"
#include "SeCommonProtocols.h"
#include "PoRevision.h"
#include "SeResponse.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace keyple { namespace calypso { namespace transaction { class CalypsoPo; } } }

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
                namespace transaction {

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @RunWith(MockitoJUnitRunner.class) public class CalypsoPoTest
                    class CalypsoPoTest : public std::enable_shared_from_this<CalypsoPoTest> {
                        /* Building FCI data with the application byte as a variant and initialize PO */
                    public:
                        //static std::shared_ptr<CalypsoPo> getPoApplicationByte(char applicationByte);
                        PoRevision getPoApplicationByte(char applicationByte);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void computePoRevision()
                        virtual void computePoRevision();
                    };
                }
            }
        }
