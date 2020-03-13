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

#include "GetDataRespParsTest.h"
#include "ApduResponse.h"
#include "ByteArrayUtil.h"
#include "GetDataFciRespPars.h"

using namespace keyple::calypso::command::po::parser;

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
using ApduResponse   = keyple::core::seproxy::message::ApduResponse;
using ByteArrayUtils = keyple::core::util::ByteArrayUtil;

void GetDataRespParsTest::testFCIparser_FCI_OK()
{
    std::string dfName                  = "00112233445566778899";
    std::string appSerialNumber         = "AABBCCDDEEFF0011";
    uint8_t siBufferSizeIndicator = 11;
    uint8_t siPlatform            = 0x55;
    uint8_t siApplicationType     = 0xAA;
    uint8_t siApplicationSubtype  = 0x55;
    uint8_t siSoftwareIssuer      = 0xAA;
    uint8_t siSoftwareVersion     = 0x55;
    uint8_t siSoftwareRevision    = 0xAA;
    char buff[100];
    snprintf(buff, sizeof(buff), "%02X%02X%02X%02X%02X%02X%02X",
             siBufferSizeIndicator, siPlatform, siApplicationType,
             siApplicationSubtype, siSoftwareIssuer, siSoftwareVersion,
             siSoftwareRevision);
    std::string startupInformation = buff;
    std::vector<uint8_t> resp         = ByteArrayUtils::fromHex(
        "6F 24 84 0A " + dfName + " A5 16 BF0C 13 C7 08 " + appSerialNumber +
        " 53 07 " + startupInformation + "9000");
    std::shared_ptr<ApduResponse> apduResponse =
        std::make_shared<ApduResponse>(resp, nullptr);
    std::shared_ptr<GetDataFciRespPars> parser =
        std::make_shared<GetDataFciRespPars>(apduResponse);

    /* valid Calypso FCI */
    ASSERT_TRUE(parser->isValidCalypsoFCI());
    /* DF not invalidated */
    ASSERT_FALSE(parser->isDfInvalidated());
    /* expected dfName */
    ASSERT_EQ(dfName, ByteArrayUtils::toHex(parser->getDfName()));
    /* expected Application Serial Number */
    ASSERT_EQ(appSerialNumber,
              ByteArrayUtils::toHex(parser->getApplicationSerialNumber()));
    /* Buffer size indicator and value */
    ASSERT_EQ(siBufferSizeIndicator, parser->getBufferSizeIndicator());
    ASSERT_EQ(512, parser->getBufferSizeValue());
    /* Platform */
    ASSERT_EQ(siPlatform, parser->getPlatformByte());
    /* ApplicationType */
    ASSERT_EQ(siApplicationType, parser->getApplicationTypeByte());
    ASSERT_TRUE(parser->isRev3_2ModeAvailable());
    ASSERT_TRUE(parser->isRatificationCommandRequired());
    ASSERT_TRUE(parser->hasCalypsoPin());
    ASSERT_FALSE(parser->hasCalypsoStoredValue());
    /* ApplicationSubType */
    ASSERT_EQ(siApplicationSubtype, parser->getApplicationSubtypeByte());
    /* SoftwareIssuer */
    ASSERT_EQ(siSoftwareIssuer, parser->getSoftwareIssuerByte());
    /* SoftwareVersion */
    ASSERT_EQ(siSoftwareVersion, parser->getSoftwareVersionByte());
    /* SoftwareRevision */
    ASSERT_EQ(siSoftwareRevision, parser->getSoftwareRevisionByte());

    /* Change startup information */
    siBufferSizeIndicator = 16;
    siPlatform            = 0xAA;
    siApplicationType     = 0x55;
    siApplicationSubtype  = 0xAA;
    siSoftwareIssuer      = 0x55;
    siSoftwareVersion     = 0xAA;
    siSoftwareRevision    = 0x55;

    snprintf(buff, sizeof(buff), "%02X%02X%02X%02X%02X%02X%02X",
             siBufferSizeIndicator, siPlatform, siApplicationType,
             siApplicationSubtype, siSoftwareIssuer, siSoftwareVersion,
             siSoftwareRevision);
    startupInformation      = buff;
    std::vector<uint8_t> resp2 = ByteArrayUtils::fromHex(
        "6F 24 84 0A " + dfName + " A5 16 BF0C 13 C7 08 " + appSerialNumber +
        " 53 07 " + startupInformation + "9000");
    apduResponse = std::make_shared<ApduResponse>(resp2, nullptr);
    parser       = std::make_shared<GetDataFciRespPars>(apduResponse);

    /* valid Calypso FCI */
    ASSERT_TRUE(parser->isValidCalypsoFCI());
    /* DF not invalidated */
    ASSERT_FALSE(parser->isDfInvalidated());
    /* expected dfName */
    ASSERT_EQ(dfName, ByteArrayUtils::toHex(parser->getDfName()));
    /* expected Application Serial Number */
    ASSERT_EQ(appSerialNumber,
              ByteArrayUtils::toHex(parser->getApplicationSerialNumber()));
    /* Buffer size indicator and value */
    ASSERT_EQ(siBufferSizeIndicator, parser->getBufferSizeIndicator());
    ASSERT_EQ(1217, parser->getBufferSizeValue());
    /* Platform */
    ASSERT_EQ(siPlatform, parser->getPlatformByte());
    /* ApplicationType */
    ASSERT_EQ(siApplicationType, parser->getApplicationTypeByte());
    ASSERT_FALSE(parser->isRev3_2ModeAvailable());
    ASSERT_FALSE(parser->isRatificationCommandRequired());
    ASSERT_FALSE(parser->hasCalypsoPin());
    ASSERT_TRUE(parser->hasCalypsoStoredValue());
    /* ApplicationSubType */
    ASSERT_EQ(siApplicationSubtype, parser->getApplicationSubtypeByte());
    /* SoftwareIssuer */
    ASSERT_EQ(siSoftwareIssuer, parser->getSoftwareIssuerByte());
    /* SoftwareVersion */
    ASSERT_EQ(siSoftwareVersion, parser->getSoftwareVersionByte());
    /* SoftwareRevision */
    ASSERT_EQ(siSoftwareRevision, parser->getSoftwareRevisionByte());
}

void GetDataRespParsTest::testFCIparser_FCI_Invalidated()
{
    std::string dfName                  = "00112233445566778899";
    std::string appSerialNumber         = "AABBCCDDEEFF0011";
    uint8_t siBufferSizeIndicator = 11;
    uint8_t siPlatform            = 0x55;
    uint8_t siApplicationType     = 0xAA;
    uint8_t siApplicationSubtype  = 0x55;
    uint8_t siSoftwareIssuer      = 0xAA;
    uint8_t siSoftwareVersion     = 0x55;
    uint8_t siSoftwareRevision    = 0xAA;

    char buff[100];
    snprintf(buff, sizeof(buff), "%02X%02X%02X%02X%02X%02X%02X",
             siBufferSizeIndicator, siPlatform, siApplicationType,
             siApplicationSubtype, siSoftwareIssuer, siSoftwareVersion,
             siSoftwareRevision);
    std::string startupInformation = buff;

    std::vector<uint8_t> resp = ByteArrayUtils::fromHex(
        "6F 24 84 0A " + dfName + " A5 16 BF0C 13 C7 08 " + appSerialNumber +
        " 53 07 " + startupInformation + "6283");
    std::shared_ptr<ApduResponse> apduResponse =
        std::make_shared<ApduResponse>(resp, nullptr);
    std::shared_ptr<GetDataFciRespPars> parser =
        std::make_shared<GetDataFciRespPars>(apduResponse);

    /* valid Calypso FCI */
    ASSERT_TRUE(parser->isValidCalypsoFCI());
    ASSERT_TRUE(parser->isDfInvalidated());
}

void GetDataRespParsTest::testFCIparser_FCI_BadTags()
{
    std::string dfName                  = "00112233445566778899";
    std::string appSerialNumber         = "AABBCCDDEEFF0011";
    uint8_t siBufferSizeIndicator = 11;
    uint8_t siPlatform            = 0x55;
    uint8_t siApplicationType     = 0xAA;
    uint8_t siApplicationSubtype  = 0x55;
    uint8_t siSoftwareIssuer      = 0xAA;
    uint8_t siSoftwareVersion     = 0x55;
    uint8_t siSoftwareRevision    = 0xAA;

    char buff[100];
    snprintf(buff, sizeof(buff), "%02X%02X%02X%02X%02X%02X%02X",
             siBufferSizeIndicator, siPlatform, siApplicationType,
             siApplicationSubtype, siSoftwareIssuer, siSoftwareVersion,
             siSoftwareRevision);
    std::string startupInformation = buff;

    /* bad tag FCI Template (not constructed) */
    std::vector<uint8_t> resp = ByteArrayUtils::fromHex(
        "4F 24 84 0A " + dfName + " A5 16 BF0C 13 C7 08 " + appSerialNumber +
        " 53 07 " + startupInformation + "9000");
    std::shared_ptr<ApduResponse> apduResponse =
        std::make_shared<ApduResponse>(resp, nullptr);
    std::shared_ptr<GetDataFciRespPars> parser =
        std::make_shared<GetDataFciRespPars>(apduResponse);

    /* invalid Calypso FCI */
    ASSERT_FALSE(parser->isValidCalypsoFCI());

    /* bad tag DF Name (constructed) */
    std::vector<uint8_t> resp2 = ByteArrayUtils::fromHex(
        "6F 24 C4 0A " + dfName + " A5 16 BF0C 13 C7 08 " + appSerialNumber +
        " 53 07 " + startupInformation + "9000");
    apduResponse = std::make_shared<ApduResponse>(resp2, nullptr);
    parser       = std::make_shared<GetDataFciRespPars>(apduResponse);

    /* invalid Calypso FCI */
    ASSERT_FALSE(parser->isValidCalypsoFCI());

    /* bad tag FCI Proprietary Template (not constructed) */
    std::vector<uint8_t> resp3 = ByteArrayUtils::fromHex(
        "6F 24 84 0A " + dfName + " 85 16 BF0C 13 C7 08 " + appSerialNumber +
        " 53 07 " + startupInformation + "9000");
    apduResponse = std::make_shared<ApduResponse>(resp3, nullptr);
    parser       = std::make_shared<GetDataFciRespPars>(apduResponse);

    /* invalid Calypso FCI */
    ASSERT_FALSE(parser->isValidCalypsoFCI());

    /* bad tag FCI Issuer Discretionary */
    std::vector<uint8_t> resp4 = ByteArrayUtils::fromHex(
        "6F 24 84 0A " + dfName + " A5 16 BF0D 13 C7 08 " + appSerialNumber +
        " 53 07 " + startupInformation + "9000");
    apduResponse = std::make_shared<ApduResponse>(resp4, nullptr);
    parser       = std::make_shared<GetDataFciRespPars>(apduResponse);

    /* invalid Calypso FCI */
    ASSERT_FALSE(parser->isValidCalypsoFCI());

    /* bad tag Application Serial Number */
    std::vector<uint8_t> resp5 = ByteArrayUtils::fromHex(
        "6F 24 84 0A " + dfName + " A5 16 BF0C 13 87 08 " + appSerialNumber +
        " 53 07 " + startupInformation + "9000");
    apduResponse = std::make_shared<ApduResponse>(resp5, nullptr);
    parser       = std::make_shared<GetDataFciRespPars>(apduResponse);

    /* invalid Calypso FCI */
    ASSERT_FALSE(parser->isValidCalypsoFCI());

    /* bad tag Discretionary Data */
    std::vector<uint8_t> resp6 = ByteArrayUtils::fromHex(
        "6F 24 84 0A " + dfName + " A5 16 BF0C 13 C7 08 " + appSerialNumber +
        " 43 07 " + startupInformation + "9000");
    apduResponse = std::make_shared<ApduResponse>(resp6, nullptr);
    parser       = std::make_shared<GetDataFciRespPars>(apduResponse);

    /* invalid Calypso FCI */
    ASSERT_FALSE(parser->isValidCalypsoFCI());
}
}
}
}
}
}

TEST(GetDataRespParsTest, testA)
{
    std::shared_ptr<GetDataRespParsTest> LocalTest =
        std::make_shared<GetDataRespParsTest>();
    LocalTest->testFCIparser_FCI_OK();
}

TEST(GetDataRespParsTest, testB)
{
    std::shared_ptr<GetDataRespParsTest> LocalTest =
        std::make_shared<GetDataRespParsTest>();
    LocalTest->testFCIparser_FCI_Invalidated();
}

TEST(GetDataRespParsTest, testC)
{
    std::shared_ptr<GetDataRespParsTest> LocalTest =
        std::make_shared<GetDataRespParsTest>();
    LocalTest->testFCIparser_FCI_BadTags();
}
