/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

#pragma once

#include <string>
#include <memory>
#include <unordered_set>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "TLV.h"
#include "GetDataFciRespPars.h"
#include "ByteArrayUtil.h"

namespace keyple {
namespace core {
namespace util {

using namespace testing;

class TLVTest : public std::enable_shared_from_this<TLVTest> {
    bool isDfInvalidated_Renamed   = false;
    bool isValidCalypsoFCI_Renamed = false;
    std::vector<char> dfName;
    std::vector<char> applicationSN;
    char siBufferSizeIndicator = 0;
    char siPlatform            = 0;
    char siApplicationType     = 0;
    char siApplicationSubtype  = 0;
    char siSoftwareIssuer      = 0;
    char siSoftwareVersion     = 0;
    char siSoftwareRevision    = 0;

public:
    virtual ~TLVTest(){};

    int sample();
};

}
}
}
