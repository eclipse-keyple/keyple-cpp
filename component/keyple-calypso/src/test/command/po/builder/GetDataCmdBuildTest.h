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

#include <vector>
#include <memory>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "PoClass.h"
#include "GetDataFciCmdBuild.h"
#include "AbstractApduCommandBuilder.h"
#include "ApduRequest.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

class GetDataCmdBuildTest
: public std::enable_shared_from_this<GetDataCmdBuildTest> {
public:
    virtual void getDataFCICmdBuild();

    virtual void getDataFCICmdBuild2();
};
}
}
}
}
}
