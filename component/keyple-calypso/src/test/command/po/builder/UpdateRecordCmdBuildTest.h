#pragma once

#include <vector>
#include <stdexcept>
#include <memory>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "AbstractApduCommandBuilder.h"
#include "ApduRequest.h"
#include "PoClass.h"
#include "UpdateRecordCmdBuild.h"

namespace keyple {
namespace core {
namespace command {
class AbstractApduCommandBuilder;
}
}
}
namespace keyple {
namespace core {
namespace seproxy {
namespace message {
class ApduRequest;
}
}
}
}

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

using AbstractApduCommandBuilder =
    keyple::core::command::AbstractApduCommandBuilder;
using ApduRequest = keyple::core::seproxy::message::ApduRequest;

class UpdateRecordCmdBuildTest
: public std::enable_shared_from_this<UpdateRecordCmdBuildTest> {

private:
    const char record_number = 0x01;

    std::vector<char> newRecordData =
        std::vector<char>{0x00, 0x01, 0x02, 0x03, 0x04};

    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder;

    std::shared_ptr<ApduRequest> apduRequest;

public:
    virtual void updateRecordCmdBuild_rev2_4();

    virtual void updateRecordCmdBuild_rev3_1();

    virtual void updateRecordCmdBuild_rev3_2();
};
}
}
}
}
}
