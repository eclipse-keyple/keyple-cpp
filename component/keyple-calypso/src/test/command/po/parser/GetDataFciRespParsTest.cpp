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

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "GetDataFciRespPars.h"

/* Core */
#include "ApduResponse.h"
#include "ByteArrayUtil.h"

/* Common */
#include "stringhelper.h"

using namespace testing;

using namespace keyple::calypso::command::po::parser;
using namespace keyple::common;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::util;

TEST(GetDataFciRespParsTest, testFCIparser_FCI_OK)
{
    const std::string dfName = "00112233445566778899";
    const std::string appSerialNumber = "AABBCCDDEEFF0011";

    uint8_t siBufferSizeIndicator = 11;
    uint8_t siPlatform = 0x55;
    uint8_t siApplicationType = 0xAA;
    uint8_t siApplicationSubtype = 0x55;
    uint8_t siSoftwareIssuer = 0xAA;
    uint8_t siSoftwareVersion = 0x55;
    uint8_t siSoftwareRevision = 0xAA;

    std::stringstream startupInfo;
    startupInfo << StringHelper::uint8ToHexString(siBufferSizeIndicator)
                << StringHelper::uint8ToHexString(siPlatform)
                << StringHelper::uint8ToHexString(siApplicationType)
                << StringHelper::uint8ToHexString(siApplicationSubtype)
                << StringHelper::uint8ToHexString(siSoftwareIssuer)
                << StringHelper::uint8ToHexString(siSoftwareVersion)
                << StringHelper::uint8ToHexString(siSoftwareRevision);

    std::shared_ptr<ApduResponse> apduResponse =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex("6F 24 84 0A " +
                                   dfName +
                                   " A5 16 BF0C 13 C7 08 " +
                                   appSerialNumber +
                                   " 53 07 " +
                                   startupInfo.str() +
                                   "9000"),
            nullptr);

    GetDataFciRespPars parser1(apduResponse, nullptr);

    /* DF not invalidated */
    ASSERT_FALSE(parser1.isDfInvalidated());
    /* Expected dfName */
    ASSERT_EQ(parser1.getDfName(), ByteArrayUtil::fromHex(dfName));
    /* Expected Application Serial Number */
    ASSERT_EQ(parser1.getApplicationSerialNumber(),
              ByteArrayUtil::fromHex(appSerialNumber));
    /* Expected StartupInfo */
    ASSERT_EQ(parser1.getDiscretionaryData(),
              ByteArrayUtil::fromHex("0B55AA55AA55AA"));

    /* Change startup information */
    siBufferSizeIndicator = 16;
    siPlatform = 0xAA;
    siApplicationType = 0x55;
    siApplicationSubtype = 0xAA;
    siSoftwareIssuer = 0x55;
    siSoftwareVersion = 0xAA;
    siSoftwareRevision = 0x55;

    /* Clear stringstream before reusing it */
    startupInfo.str(std::string());
    startupInfo.clear();

    startupInfo << StringHelper::uint8ToHexString(siBufferSizeIndicator)
                << StringHelper::uint8ToHexString(siPlatform)
                << StringHelper::uint8ToHexString(siApplicationType)
                << StringHelper::uint8ToHexString(siApplicationSubtype)
                << StringHelper::uint8ToHexString(siSoftwareIssuer)
                << StringHelper::uint8ToHexString(siSoftwareVersion)
                << StringHelper::uint8ToHexString(siSoftwareRevision);

    apduResponse = std::make_shared<ApduResponse>(
                       ByteArrayUtil::fromHex("6F 24 84 0A " +
                                              dfName +
                                              " A5 16 BF0C 13 C7 08 " +
                                              appSerialNumber +
                                              " 53 07 " +
                                              startupInfo.str() +
                                              "9000"),
                       nullptr);

    GetDataFciRespPars parser2(apduResponse, nullptr);

    /* DF not invalidated */
    ASSERT_FALSE(parser2.isDfInvalidated());
    /* Expected dfName */
    ASSERT_EQ(parser2.getDfName(), ByteArrayUtil::fromHex(dfName));
    /* Expected Application Serial Number */
    ASSERT_EQ(parser2.getApplicationSerialNumber(),
              ByteArrayUtil::fromHex(appSerialNumber));
    /* Expected StartupInfo */
    ASSERT_EQ(parser2.getDiscretionaryData(),
              ByteArrayUtil::fromHex("10AA55AA55AA55"));
}

TEST(GetDataFciRespParsTest, testFCIparser_FCI_Invalidated)
{
    const std::string dfName = "00112233445566778899";
    const std::string appSerialNumber = "AABBCCDDEEFF0011";

    const uint8_t siBufferSizeIndicator = 11;
    const uint8_t siPlatform = 0x55;
    const uint8_t siApplicationType = 0xAA;
    const uint8_t siApplicationSubtype = 0x55;
    const uint8_t siSoftwareIssuer = 0xAA;
    const uint8_t siSoftwareVersion = 0x55;
    const uint8_t siSoftwareRevision = 0xAA;

    std::stringstream startupInfo;
    startupInfo << StringHelper::uint8ToHexString(siBufferSizeIndicator)
                << StringHelper::uint8ToHexString(siPlatform)
                << StringHelper::uint8ToHexString(siApplicationType)
                << StringHelper::uint8ToHexString(siApplicationSubtype)
                << StringHelper::uint8ToHexString(siSoftwareIssuer)
                << StringHelper::uint8ToHexString(siSoftwareVersion)
                << StringHelper::uint8ToHexString(siSoftwareRevision);

    std::shared_ptr<ApduResponse> apduResponse =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex("6F 24 84 0A " +
                                   dfName +
                                   " A5 16 BF0C 13 C7 08 " +
                                   appSerialNumber +
                                   " 53 07 " +
                                   startupInfo.str() +
                                   "6283"),
            nullptr);

    GetDataFciRespPars parser(apduResponse, nullptr);

    ASSERT_TRUE(parser.isDfInvalidated());
}

TEST(GetDataFciRespParsTest, testFCIparser_FCI_BadTags)
{
    const std::string dfName = "00112233445566778899";
    const std::string appSerialNumber = "AABBCCDDEEFF0011";

    const uint8_t siBufferSizeIndicator = 11;
    const uint8_t siPlatform = 0x55;
    const uint8_t siApplicationType = 0xAA;
    const uint8_t siApplicationSubtype = 0x55;
    const uint8_t siSoftwareIssuer = 0xAA;
    const uint8_t siSoftwareVersion = 0x55;
    const uint8_t siSoftwareRevision = 0xAA;

    std::stringstream startupInfo;
    startupInfo << StringHelper::uint8ToHexString(siBufferSizeIndicator)
                << StringHelper::uint8ToHexString(siPlatform)
                << StringHelper::uint8ToHexString(siApplicationType)
                << StringHelper::uint8ToHexString(siApplicationSubtype)
                << StringHelper::uint8ToHexString(siSoftwareIssuer)
                << StringHelper::uint8ToHexString(siSoftwareVersion)
                << StringHelper::uint8ToHexString(siSoftwareRevision);

    /* Bad tag FCI Template (not constructed) */
    std::shared_ptr<ApduResponse> apduResponse =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex("4F 24 84 0A " +
                                   dfName +
                                   " A5 16 BF0C 13 C7 08 " +
                                   appSerialNumber +
                                   " 53 07 " +
                                   startupInfo.str() +
                                   "9000"),
            nullptr);

    GetDataFciRespPars parser1(apduResponse, nullptr);

    /* Invalid Calypso FCI */
    ASSERT_FALSE(parser1.isValidCalypsoFCI());

    /* Bad tag DF Name (constructed) */
    apduResponse = std::make_shared<ApduResponse>(
                       ByteArrayUtil::fromHex("6F 24 C4 0A " +
                                              dfName +
                                              " A5 16 BF0C 13 C7 08 " +
                                              appSerialNumber +
                                              " 53 07 " +
                                              startupInfo.str() +
                                              "9000"),
                        nullptr);

    GetDataFciRespPars parser2(apduResponse, nullptr);

    /* Invalid Calypso FCI */
    ASSERT_FALSE(parser2.isValidCalypsoFCI());

    /* Bad tag FCI Proprietary Template (not constructed) */
    apduResponse = std::make_shared<ApduResponse>(
                       ByteArrayUtil::fromHex("6F 24 84 0A " +
                                              dfName +
                                              " 85 16 BF0C 13 C7 08 " +
                                              appSerialNumber +
                                              " 53 07 " +
                                              startupInfo.str() +
                                              "9000"),
                       nullptr);

    GetDataFciRespPars parser3(apduResponse, nullptr);

    /* Invalid Calypso FCI */
    ASSERT_FALSE(parser3.isValidCalypsoFCI());

    /* Bad tag FCI Issuer Discretionary */
    apduResponse = std::make_shared<ApduResponse>(
                       ByteArrayUtil::fromHex("6F 24 84 0A " +
                                              dfName +
                                              " A5 16 BF0D 13 C7 08 " +
                                              appSerialNumber +
                                              " 53 07 " +
                                              startupInfo.str() +
                                              "9000"),
                       nullptr);

    GetDataFciRespPars parser4(apduResponse, nullptr);

    /* Invalid Calypso FCI */
    ASSERT_FALSE(parser4.isValidCalypsoFCI());

    /* Bad tag Application Serial Number */
    apduResponse = std::make_shared<ApduResponse>(
                       ByteArrayUtil::fromHex("6F 24 84 0A " +
                                              dfName +
                                              " A5 16 BF0C 13 87 08 " +
                                              appSerialNumber +
                                              " 53 07 " +
                                              startupInfo.str() +
                                              "9000"),
            nullptr);

    GetDataFciRespPars parser5(apduResponse, nullptr);

    /* Invalid Calypso FCI */
    ASSERT_FALSE(parser5.isValidCalypsoFCI());

    /* Bad tag Discretionary Data */
    apduResponse = std::make_shared<ApduResponse>(
                       ByteArrayUtil::fromHex("6F 24 84 0A " +
                                              dfName +
                                              " A5 16 BF0C 13 C7 08 " +
                                              appSerialNumber +
                                              " 43 07 " +
                                              startupInfo.str() +
                                              "9000"),
                       nullptr);

    GetDataFciRespPars parser6(apduResponse, nullptr);

    /*Iinvalid Calypso FCI */
    ASSERT_FALSE(parser6.isValidCalypsoFCI());
}

static void check_CLR_1(const std::string& dfName,
                        const std::string& appSerialNumber,
                        const std::string& startupInformation)
{

    const std::string dfNameLength =
        StringHelper::uint8ToHexString(dfName.length() / 2);
    const std::string snLength =
        StringHelper::uint8ToHexString(appSerialNumber.length() / 2);
    const std::string siLength =
        StringHelper::uint8ToHexString(startupInformation.length() / 2);
    const std::string totalLength =
        StringHelper::uint8ToHexString(12 +
                                       (dfName.length() +
                                        appSerialNumber.length() +
                                        startupInformation.length()) / 2);

    std::shared_ptr<ApduResponse> apduResponse =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex("6F " +
                                   totalLength +
                                   " 84" +
                                   dfNameLength +
                                   dfName +
                                   " A5 16 BF0C 13 C7 " +
                                   snLength +
                                   appSerialNumber +
                                   " 53" +
                                   siLength +
                                   startupInformation +
                                   "9000"),
            nullptr);

    GetDataFciRespPars parser(apduResponse, nullptr);

    /* DF not invalidated */
    ASSERT_FALSE(parser.isDfInvalidated());
    /* Expected dfName */
    ASSERT_EQ(parser.getDfName(), ByteArrayUtil::fromHex(dfName));
    /* Expected Application Serial Number */
    ASSERT_EQ(parser.getApplicationSerialNumber(),
              ByteArrayUtil::fromHex(appSerialNumber));
    /* Expected StartupInfo */
    ASSERT_EQ(parser.getDiscretionaryData(),
              ByteArrayUtil::fromHex(startupInformation));
}

/**
 * CL-SEL-TLVSTRUC.1<br>
 * Calypso Layer shall allow the answer to the Select Application command to be
 * variable in length and shall analyse the TLV structure to retrieve its data.
 */
TEST(GetDataFciRespParsTest, test_CRL_1)
{
    /* DF Name 5 const uint8_ts - SN 8 const uint8_ts - SI 7 const uint8_ts */
    check_CLR_1("0011223344", "AABBCCDDEEFF0011", "0B55AA55AA55AA");

    /* DF Name 16 const uint8_ts - SN 8 const uint8_ts - SI 7 const uint8_ts */
    check_CLR_1("00112233445566778899AABBCCDDEEFF",
                "AABBCCDDEEFF0011",
                "0B55AA55AA55AA");

    /* DF Name 16 const uint8_ts - SN 8 const uint8_ts - SI 9 const uint8_ts */
    check_CLR_1("00112233445566778899AABBCCDDEEFF",
                "AABBCCDDEEFF0011",
                "0B55AA55AA55AABBCC");

    /* DF Name 16 const uint8_ts - SN 10 const uint8_ts - SI 9 const uint8_ts */
    check_CLR_1("00112233445566778899AABBCCDDEEFF",
                "AABBCCDDEEFF00112233",
                "0B55AA55AA55AABBCC");
}
