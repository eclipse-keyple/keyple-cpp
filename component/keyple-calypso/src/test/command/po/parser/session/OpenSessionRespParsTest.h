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

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {
class AbstractOpenSessionRespPars;
}
}
}
}
}
}

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

class OpenSessionRespParsTest
: public std::enable_shared_from_this<OpenSessionRespParsTest> {

private:
    void check(std::shared_ptr<AbstractOpenSessionRespPars> resp);

public:
    virtual void testgetResponse_rev2_4();

    virtual void testgetResponse_rev2_4_no_data();

    virtual void testgetResponse_rev2_4_non_ratified();

    virtual void testgetResponse_rev2_4_no_data_non_ratified();

    virtual void testgetResponse_rev2_4_bad_length_inf();

    virtual void testgetResponse_rev2_4_bad_length_sup();

    virtual void testgetResponse_rev3_1();

    virtual void testgetResponse_rev3_2();
};

}
}
}
}
}
}
