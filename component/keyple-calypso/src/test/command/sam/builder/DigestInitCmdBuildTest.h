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
#include <stdexcept>
#include <memory>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

class DigestInitCmdBuildTest
: public std::enable_shared_from_this<DigestInitCmdBuildTest> {
public:
    virtual void digestInitCmd_inconsistent();

    virtual void digestInitCmd_inconsistent_digestNull();

    virtual void digestInitCmd();
};
}
}
}
}
}
}
