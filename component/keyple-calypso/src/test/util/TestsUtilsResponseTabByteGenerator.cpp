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

std::vector<uint8_t>
TestsUtilsResponseTabByteGenerator::append(std::vector<uint8_t>& arrays)
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

std::vector<uint8_t>
TestsUtilsResponseTabByteGenerator::generate4MultiRecordsTabByte()
{
    return std::vector<uint8_t>{0x01, 0x01, 0x01, 0x01, 0x30, 0x01, 0x01,
                                0x30, 0x01, 0x01, 0x30, 0x01, 0x01, 0x30};
}

std::vector<uint8_t>
TestsUtilsResponseTabByteGenerator::generateResponseOkTabByteRev2_4()
{
    return ByteArrayUtils::fromHex(
        "CC 11223344 00112233445566778899AABBCCDDEEFF "
        "00112233445566778899AABBCC 9000");
}

std::vector<uint8_t> TestsUtilsResponseTabByteGenerator::generateDataOpenTabByte()
{
    return std::vector<uint8_t>{0x7E, 0x8F, 0x05, 0x75, 0x01A, 0xFF, 0x01, 0x01,
                                0x00, 0x30};
}

std::vector<uint8_t> TestsUtilsResponseTabByteGenerator::generateFciTabByte()
{
    return std::vector<uint8_t>{
        0x6F, 0x22, 0x84, 0x08, 0x33, 0x4D, 0x54, 0x52, 0x2E, 0x49,
        0x43, 0x41, 0xA5, 0x16, 0xBF, 0x0C, 0x13, 0xC7, 0x08, 0x00,
        0x00, 0x00, 0x00, 0x27, 0x4A, 0x9A, 0xB7, 0x53, 0x07, 0x0A,
        0x3C, 0x11, 0x32, 0x14, 0x10, 0x01};
}

std::vector<uint8_t>
TestsUtilsResponseTabByteGenerator::generateTerminalSessionSignatureTabByte()
{
    return std::vector<uint8_t>{
        0x7E, 0x8F, 0x05, 0x75, 0x01A, 0xFF, 0x01, 0x01};
}

std::vector<uint8_t>
TestsUtilsResponseTabByteGenerator::generateResponseOkTabByteRev3_1()
{
    return std::vector<uint8_t>{
        0x6F, 0x22, 0x84, 0x08, 0x33, 0x4D, 0x54, 0x1E, 0x2E, 0x49,
        0x43, 0x41, 0xA5, 0x16, 0xBF, 0x0C, 0x13, 0xC7, 0x08, 0x00,
        0x00, 0x00, 0x00, 0x27, 0x4A, 0x9A, 0xB9, 0x53, 0x07, 0x0A,
        0x3C, 0x11, 0x32, 0x14, 0x10, 0x01, 0x90, 0x00};
}

std::vector<uint8_t>
TestsUtilsResponseTabByteGenerator::generateResponseOkTabByteRev3_2()
{
    return std::vector<uint8_t>{
        0x6F, 0x22, 0x84, 0x08, 0x33, 0x4D, 0x54, 0x52, 0x2E, 0x49,
        0x1B, 0x1A, 0xA5, 0x16, 0xBF, 0x0C, 0x13, 0xC7, 0x08, 0x00,
        0x00, 0x00, 0x00, 0x27, 0x4A, 0x9A, 0xB9, 0x53, 0x07, 0x0A,
        0x3C, 0x11, 0x32, 0x14, 0x10, 0x01, 0x90, 0x00};
}

std::shared_ptr<ApduResponse>
TestsUtilsResponseTabByteGenerator::generateApduResponseValidRev2_4()
{
    std::vector<uint8_t> cFullStatusCode =
        TestsUtilsStatusCodeGenerator::generateSuccessfulStatusCode();
    std::vector<uint8_t> cReponseOK = generateResponseOkTabByteRev2_4();
    std::vector<uint8_t> cCmd;

    std::merge(cReponseOK.begin(), cReponseOK.end(), cFullStatusCode.begin(),
               cFullStatusCode.end(), cCmd.begin());
    std::vector<uint8_t> cResult = append(cCmd);
    return std::make_shared<ApduResponse>(cResult, nullptr);
}

std::shared_ptr<ApduResponse>
TestsUtilsResponseTabByteGenerator::generateApduResponseValidRev3_1()
{
    std::vector<uint8_t> cFullStatusCode =
        TestsUtilsStatusCodeGenerator::generateSuccessfulStatusCode();
    std::vector<uint8_t> cReponseOK = generateResponseOkTabByteRev3_1();
    std::vector<uint8_t> cCmd;

    cCmd.clear();
    cCmd.insert(cCmd.end(), cReponseOK.begin(), cReponseOK.end());
    cCmd.insert(cCmd.end(), cFullStatusCode.begin(), cFullStatusCode.end());
    std::vector<uint8_t> cResult = append(cCmd);
    return std::make_shared<ApduResponse>(cResult, nullptr);
}

std::shared_ptr<ApduResponse>
TestsUtilsResponseTabByteGenerator::generateApduResponseValidRev3_2()
{
    std::vector<uint8_t> cFullStatusCode =
        TestsUtilsStatusCodeGenerator::generateSuccessfulStatusCode();
    std::vector<uint8_t> cReponseOK = generateResponseOkTabByteRev3_2();
    std::vector<uint8_t> cCmd;

    cCmd.clear();
    cCmd.insert(cCmd.end(), cReponseOK.begin(), cReponseOK.end());
    cCmd.insert(cCmd.end(), cFullStatusCode.begin(), cFullStatusCode.end());
    std::vector<uint8_t> cResult = append(cCmd);
    return std::make_shared<ApduResponse>(cResult, nullptr);
}

std::shared_ptr<ApduResponse>
TestsUtilsResponseTabByteGenerator::generateApduResponseOpenSessionCmd()
{
    std::vector<uint8_t> cFullStatusCode =
        TestsUtilsStatusCodeGenerator::generateSuccessfulStatusCode();
    std::vector<uint8_t> cReponseOK = generateDataOpenTabByte();
    std::vector<uint8_t> cCmd;

    cCmd.clear();
    cCmd.insert(cCmd.end(), cReponseOK.begin(), cReponseOK.end());
    cCmd.insert(cCmd.end(), cFullStatusCode.begin(), cFullStatusCode.end());
    std::vector<uint8_t> cResult = append(cCmd);
    return std::make_shared<ApduResponse>(cResult, nullptr);
}

std::shared_ptr<ApduResponse>
TestsUtilsResponseTabByteGenerator::generateApduResponseOpenSessionCmdError()
{
    std::vector<uint8_t> cFullStatusCode =
        TestsUtilsStatusCodeGenerator::generateAccessForbiddenStatusCode();
    std::vector<uint8_t> cReponseOK = generateDataOpenTabByte();
    std::vector<uint8_t> cCmd;

    cCmd.clear();
    cCmd.insert(cCmd.end(), cReponseOK.begin(), cReponseOK.end());
    cCmd.insert(cCmd.end(), cFullStatusCode.begin(), cFullStatusCode.end());
    std::vector<uint8_t> cResult = append(cCmd);
    return std::make_shared<ApduResponse>(cResult, nullptr);
}

std::shared_ptr<ApduResponse> TestsUtilsResponseTabByteGenerator::
    generateApduResponseTerminalSessionSignatureCmd()
{
    std::vector<uint8_t> cFullStatusCode =
        TestsUtilsStatusCodeGenerator::generateSuccessfulStatusCode();
    std::vector<uint8_t> cReponseOK = generateTerminalSessionSignatureTabByte();
    std::vector<uint8_t> cCmd;

    cCmd.clear();
    cCmd.insert(cCmd.end(), cReponseOK.begin(), cReponseOK.end());
    cCmd.insert(cCmd.end(), cFullStatusCode.begin(), cFullStatusCode.end());
    std::vector<uint8_t> cResult = append(cCmd);
    return std::make_shared<ApduResponse>(cResult, nullptr);
}

std::shared_ptr<ApduResponse> TestsUtilsResponseTabByteGenerator::
    generateApduResponseTerminalSessionSignatureCmdError()
{
    std::vector<uint8_t> cFullStatusCode = TestsUtilsStatusCodeGenerator::
        generateCommandForbiddenOnBinaryFilesStatusCode();
    std::vector<uint8_t> cReponseOK = generateTerminalSessionSignatureTabByte();
    std::vector<uint8_t> cCmd;

    cCmd.clear();
    cCmd.insert(cCmd.end(), cReponseOK.begin(), cReponseOK.end());
    cCmd.insert(cCmd.end(), cFullStatusCode.begin(), cFullStatusCode.end());
    std::vector<uint8_t> cResult = append(cCmd);
    return std::make_shared<ApduResponse>(cResult, nullptr);
}

std::shared_ptr<ApduResponse>
TestsUtilsResponseTabByteGenerator::generateApduResponseFciCmd()
{
    std::vector<uint8_t> cFullStatusCode =
        TestsUtilsStatusCodeGenerator::generateSuccessfulStatusCode();
    std::vector<uint8_t> cReponseOK = generateFciTabByte();
    std::vector<uint8_t> cCmd;

    cCmd.clear();
    cCmd.insert(cCmd.end(), cReponseOK.begin(), cReponseOK.end());
    cCmd.insert(cCmd.end(), cFullStatusCode.begin(), cFullStatusCode.end());
    std::vector<uint8_t> cResult = append(cCmd);
    return std::make_shared<ApduResponse>(cResult, nullptr);
}

std::shared_ptr<ApduResponse>
TestsUtilsResponseTabByteGenerator::generateApduResponseFciCmdError()
{
    std::vector<uint8_t> cFullStatusCode =
        TestsUtilsStatusCodeGenerator::generateFileNotFoundStatusCode();
    std::vector<uint8_t> cReponseOK = generateFciTabByte();
    std::vector<uint8_t> cCmd;

    cCmd.clear();
    cCmd.insert(cCmd.end(), cReponseOK.begin(), cReponseOK.end());
    cCmd.insert(cCmd.end(), cFullStatusCode.begin(), cFullStatusCode.end());
    std::vector<uint8_t> cResult = append(cCmd);
    return std::make_shared<ApduResponse>(cResult, nullptr);
}
}
}
}
