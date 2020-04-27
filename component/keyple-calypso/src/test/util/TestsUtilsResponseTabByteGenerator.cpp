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

#include "TestsUtilsResponseTabByteGenerator.h"
#include "ByteArrayUtil.h"
#include "ApduResponse.h"
#include "TestsUtilsStatusCodeGenerator.h"

namespace keyple {
namespace calypso {
namespace util {
using ApduResponse   = keyple::core::seproxy::message::ApduResponse;
using ByteArrayUtils = keyple::core::util::ByteArrayUtil;

std::vector<char>
TestsUtilsResponseTabByteGenerator::append(std::vector<char>& arrays)
{
    //std::shared_ptr<ByteArrayOutputStream> * const out =
    //    std::make_shared<ByteArrayOutputStream>();
    //if (arrays != nullptr) {
    //    for (char[] array_Renamed : arrays) {
    //        if (array_Renamed.size() > 0) {
    //            out->write(array_Renamed, 0, array_Renamed.size());
    //        }
    //    }
    //}
    //return out->toByteArray();
    return arrays;
}

std::vector<char>
TestsUtilsResponseTabByteGenerator::generate4MultiRecordsTabByte()
{
    return std::vector<char>{0x01, 0x01, 0x01, 0x01, 0x30, 0x01, 0x01,
                             0x30, 0x01, 0x01, 0x30, 0x01, 0x01, 0x30};
}

std::vector<char>
TestsUtilsResponseTabByteGenerator::generateResponseOkTabByteRev2_4()
{
    return ByteArrayUtils::fromHex(
        "CC 11223344 00112233445566778899AABBCCDDEEFF "
        "00112233445566778899AABBCC 9000");
}

std::vector<char> TestsUtilsResponseTabByteGenerator::generateDataOpenTabByte()
{
    return std::vector<char>{0x7E,  static_cast<char>(0x8F),
                             0x05,  0x75,
                             0x01A, static_cast<char>(0xFF),
                             0x01,  0x01,
                             0x00,  0x30};
}

std::vector<char> TestsUtilsResponseTabByteGenerator::generateFciTabByte()
{
    return std::vector<char>{static_cast<char>(0x6F),
                             0x22,
                             static_cast<char>(0x84),
                             0x08,
                             0x33,
                             0x4D,
                             0x54,
                             0x52,
                             0x2E,
                             0x49,
                             0x43,
                             0x41,
                             static_cast<char>(0xA5),
                             0x16,
                             static_cast<char>(0xBF),
                             0x0C,
                             0x13,
                             static_cast<char>(0xC7),
                             0x08,
                             0x00,
                             0x00,
                             0x00,
                             0x00,
                             0x27,
                             0x4A,
                             static_cast<char>(0x9A),
                             static_cast<char>(0xB7),
                             0x53,
                             0x07,
                             0x0A,
                             0x3C,
                             0x11,
                             0x32,
                             0x14,
                             0x10,
                             0x01};
}

std::vector<char>
TestsUtilsResponseTabByteGenerator::generateTerminalSessionSignatureTabByte()
{
    return std::vector<char>{0x7E,  static_cast<char>(0x8F), 0x05, 0x75,
                             0x01A, static_cast<char>(0xFF), 0x01, 0x01};
}

std::vector<char>
TestsUtilsResponseTabByteGenerator::generateResponseOkTabByteRev3_1()
{
    return std::vector<char>{0x6F,
                             0x22,
                             static_cast<char>(0x84),
                             0x08,
                             0x33,
                             0x4D,
                             0x54,
                             0x1E,
                             0x2E,
                             0x49,
                             0x43,
                             0x41,
                             static_cast<char>(0xA5),
                             0x16,
                             static_cast<char>(0xBF),
                             0x0C,
                             0x13,
                             static_cast<char>(0xC7),
                             0x08,
                             0x00,
                             0x00,
                             0x00,
                             0x00,
                             0x27,
                             0x4A,
                             static_cast<char>(0x9A),
                             static_cast<char>(0xB9),
                             0x53,
                             0x07,
                             0x0A,
                             0x3C,
                             0x11,
                             0x32,
                             0x14,
                             0x10,
                             0x01,
                             static_cast<char>(0x90),
                             0x00};
}

std::vector<char>
TestsUtilsResponseTabByteGenerator::generateResponseOkTabByteRev3_2()
{
    return std::vector<char>{0x6F,
                             0x22,
                             static_cast<char>(0x84),
                             0x08,
                             0x33,
                             0x4D,
                             0x54,
                             0x52,
                             0x2E,
                             0x49,
                             0x1B,
                             0x1A,
                             static_cast<char>(0xA5),
                             0x16,
                             static_cast<char>(0xBF),
                             0x0C,
                             0x13,
                             static_cast<char>(0xC7),
                             0x08,
                             0x00,
                             0x00,
                             0x00,
                             0x00,
                             0x27,
                             0x4A,
                             static_cast<char>(0x9A),
                             static_cast<char>(0xB9),
                             0x53,
                             0x07,
                             0x0A,
                             0x3C,
                             0x11,
                             0x32,
                             0x14,
                             0x10,
                             0x01,
                             static_cast<char>(0x90),
                             0x00};
}

std::shared_ptr<ApduResponse>
TestsUtilsResponseTabByteGenerator::generateApduResponseValidRev2_4()
{
    std::vector<char> cFullStatusCode =
        TestsUtilsStatusCodeGenerator::generateSuccessfulStatusCode();
    std::vector<char> cReponseOK = generateResponseOkTabByteRev2_4();
    std::vector<char> cCmd;

    std::merge(cReponseOK.begin(), cReponseOK.end(), cFullStatusCode.begin(),
               cFullStatusCode.end(), cCmd.begin());
    std::vector<char> cResult = append(cCmd);
    return std::make_shared<ApduResponse>(cResult, nullptr);
}

std::shared_ptr<ApduResponse>
TestsUtilsResponseTabByteGenerator::generateApduResponseValidRev3_1()
{
    std::vector<char> cFullStatusCode =
        TestsUtilsStatusCodeGenerator::generateSuccessfulStatusCode();
    std::vector<char> cReponseOK = generateResponseOkTabByteRev3_1();
    std::vector<char> cCmd;

    cCmd.clear();
    cCmd.insert(cCmd.end(), cReponseOK.begin(), cReponseOK.end());
    cCmd.insert(cCmd.end(), cFullStatusCode.begin(), cFullStatusCode.end());
    std::vector<char> cResult = append(cCmd);
    return std::make_shared<ApduResponse>(cResult, nullptr);
}

std::shared_ptr<ApduResponse>
TestsUtilsResponseTabByteGenerator::generateApduResponseValidRev3_2()
{
    std::vector<char> cFullStatusCode =
        TestsUtilsStatusCodeGenerator::generateSuccessfulStatusCode();
    std::vector<char> cReponseOK = generateResponseOkTabByteRev3_2();
    std::vector<char> cCmd;

    cCmd.clear();
    cCmd.insert(cCmd.end(), cReponseOK.begin(), cReponseOK.end());
    cCmd.insert(cCmd.end(), cFullStatusCode.begin(), cFullStatusCode.end());
    std::vector<char> cResult = append(cCmd);
    return std::make_shared<ApduResponse>(cResult, nullptr);
}

std::shared_ptr<ApduResponse>
TestsUtilsResponseTabByteGenerator::generateApduResponseOpenSessionCmd()
{
    std::vector<char> cFullStatusCode =
        TestsUtilsStatusCodeGenerator::generateSuccessfulStatusCode();
    std::vector<char> cReponseOK = generateDataOpenTabByte();
    std::vector<char> cCmd;

    cCmd.clear();
    cCmd.insert(cCmd.end(), cReponseOK.begin(), cReponseOK.end());
    cCmd.insert(cCmd.end(), cFullStatusCode.begin(), cFullStatusCode.end());
    std::vector<char> cResult = append(cCmd);
    return std::make_shared<ApduResponse>(cResult, nullptr);
}

std::shared_ptr<ApduResponse>
TestsUtilsResponseTabByteGenerator::generateApduResponseOpenSessionCmdError()
{
    std::vector<char> cFullStatusCode =
        TestsUtilsStatusCodeGenerator::generateAccessForbiddenStatusCode();
    std::vector<char> cReponseOK = generateDataOpenTabByte();
    std::vector<char> cCmd;

    cCmd.clear();
    cCmd.insert(cCmd.end(), cReponseOK.begin(), cReponseOK.end());
    cCmd.insert(cCmd.end(), cFullStatusCode.begin(), cFullStatusCode.end());
    std::vector<char> cResult = append(cCmd);
    return std::make_shared<ApduResponse>(cResult, nullptr);
}

std::shared_ptr<ApduResponse> TestsUtilsResponseTabByteGenerator::
    generateApduResponseTerminalSessionSignatureCmd()
{
    std::vector<char> cFullStatusCode =
        TestsUtilsStatusCodeGenerator::generateSuccessfulStatusCode();
    std::vector<char> cReponseOK = generateTerminalSessionSignatureTabByte();
    std::vector<char> cCmd;

    cCmd.clear();
    cCmd.insert(cCmd.end(), cReponseOK.begin(), cReponseOK.end());
    cCmd.insert(cCmd.end(), cFullStatusCode.begin(), cFullStatusCode.end());
    std::vector<char> cResult = append(cCmd);
    return std::make_shared<ApduResponse>(cResult, nullptr);
}

std::shared_ptr<ApduResponse> TestsUtilsResponseTabByteGenerator::
    generateApduResponseTerminalSessionSignatureCmdError()
{
    std::vector<char> cFullStatusCode = TestsUtilsStatusCodeGenerator::
        generateCommandForbiddenOnBinaryFilesStatusCode();
    std::vector<char> cReponseOK = generateTerminalSessionSignatureTabByte();
    std::vector<char> cCmd;

    cCmd.clear();
    cCmd.insert(cCmd.end(), cReponseOK.begin(), cReponseOK.end());
    cCmd.insert(cCmd.end(), cFullStatusCode.begin(), cFullStatusCode.end());
    std::vector<char> cResult = append(cCmd);
    return std::make_shared<ApduResponse>(cResult, nullptr);
}

std::shared_ptr<ApduResponse>
TestsUtilsResponseTabByteGenerator::generateApduResponseFciCmd()
{
    std::vector<char> cFullStatusCode =
        TestsUtilsStatusCodeGenerator::generateSuccessfulStatusCode();
    std::vector<char> cReponseOK = generateFciTabByte();
    std::vector<char> cCmd;

    cCmd.clear();
    cCmd.insert(cCmd.end(), cReponseOK.begin(), cReponseOK.end());
    cCmd.insert(cCmd.end(), cFullStatusCode.begin(), cFullStatusCode.end());
    std::vector<char> cResult = append(cCmd);
    return std::make_shared<ApduResponse>(cResult, nullptr);
}

std::shared_ptr<ApduResponse>
TestsUtilsResponseTabByteGenerator::generateApduResponseFciCmdError()
{
    std::vector<char> cFullStatusCode =
        TestsUtilsStatusCodeGenerator::generateFileNotFoundStatusCode();
    std::vector<char> cReponseOK = generateFciTabByte();
    std::vector<char> cCmd;

    cCmd.clear();
    cCmd.insert(cCmd.end(), cReponseOK.begin(), cReponseOK.end());
    cCmd.insert(cCmd.end(), cFullStatusCode.begin(), cFullStatusCode.end());
    std::vector<char> cResult = append(cCmd);
    return std::make_shared<ApduResponse>(cResult, nullptr);
}
}
}
}
