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
namespace core {
namespace seproxy {
namespace message {
class ApduResponse;
}
}
}
}

namespace keyple {
namespace calypso {
namespace util {

using ApduResponse = keyple::core::seproxy::message::ApduResponse;

class TestsUtilsResponseTabByteGenerator
: public std::enable_shared_from_this<TestsUtilsResponseTabByteGenerator> {
    /**
     * Append the given byte arrays to one big array
     *
     * @param arrays The arrays to append
     * @return The complete array containing the appended data
     */
private:
    static std::vector<uint8_t> append(std::vector<uint8_t>& arrays);

public:
    virtual std::vector<uint8_t> generate4MultiRecordsTabByte();

private:
    static std::vector<uint8_t> generateResponseOkTabByteRev2_4();

    static std::vector<uint8_t> generateDataOpenTabByte();

    static std::vector<uint8_t> generateFciTabByte();

    static std::vector<uint8_t> generateTerminalSessionSignatureTabByte();

    static std::vector<uint8_t> generateResponseOkTabByteRev3_1();

    static std::vector<uint8_t> generateResponseOkTabByteRev3_2();

public:
    static std::shared_ptr<ApduResponse> generateApduResponseValidRev2_4();

    static std::shared_ptr<ApduResponse> generateApduResponseValidRev3_1();

    static std::shared_ptr<ApduResponse> generateApduResponseValidRev3_2();

    static std::shared_ptr<ApduResponse> generateApduResponseOpenSessionCmd();

    static std::shared_ptr<ApduResponse>
    generateApduResponseOpenSessionCmdError();

    static std::shared_ptr<ApduResponse>
    generateApduResponseTerminalSessionSignatureCmd();

    static std::shared_ptr<ApduResponse>
    generateApduResponseTerminalSessionSignatureCmdError();

    static std::shared_ptr<ApduResponse> generateApduResponseFciCmd();

    static std::shared_ptr<ApduResponse> generateApduResponseFciCmdError();
};

}
}
}
