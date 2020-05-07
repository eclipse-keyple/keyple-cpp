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

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "PoRevision.h"

namespace keyple {
namespace calypso {
namespace transaction {
class CalypsoPo;
}
}
}

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command::po;

class CalypsoPoTest : public std::enable_shared_from_this<CalypsoPoTest> {
public:
    /*
     * Building FCI data with the application byte as a variant and initialize
     * PO
     */

    //static std::shared_ptr<CalypsoPo>
    //    getPoApplicationByte(char applicationByte);
    PoRevision getPoApplicationByte(uint8_t applicationByte);

    virtual void computePoRevision();
};
}
}
}
