#include "GetDataRespParsTest.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduResponse.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../main/java/org/eclipse/keyple/calypso/command/po/parser/GetDataFciRespPars.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace parser {
                            using ApduResponse = org::eclipse::keyple::seproxy::message::ApduResponse;
                            using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                            using org::junit::Assert;
                            using org::junit::Test;
                            using org::junit::runner::RunWith;
                            using org::mockito::junit::MockitoJUnitRunner;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFCIparser_FCI_OK()
                            void GetDataRespParsTest::testFCIparser_FCI_OK() {
                                std::string dfName = "00112233445566778899";
                                std::string appSerialNumber = "AABBCCDDEEFF0011";
                                char siBufferSizeIndicator = 11;
                                char siPlatform = static_cast<char>(0x55);
                                char siApplicationType = static_cast<char>(0xAA);
                                char siApplicationSubtype = static_cast<char>(0x55);
                                char siSoftwareIssuer = static_cast<char>(0xAA);
                                char siSoftwareVersion = static_cast<char>(0x55);
                                char siSoftwareRevision = static_cast<char>(0xAA);
                                std::string startupInformation = std::string::format("%02X%02X%02X%02X%02X%02X%02X", siBufferSizeIndicator, siPlatform, siApplicationType, siApplicationSubtype, siSoftwareIssuer, siSoftwareVersion, siSoftwareRevision);
                                std::shared_ptr<ApduResponse> apduResponse = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("6F 24 84 0A " + dfName + " A5 16 BF0C 13 C7 08 " + appSerialNumber + " 53 07 " + startupInformation + "9000"), nullptr);

                                std::shared_ptr<GetDataFciRespPars> parser = std::make_shared<GetDataFciRespPars>(apduResponse);

                                /* valid Calypso FCI */
                                Assert::assertTrue(parser->isValidCalypsoFCI());
                                /* DF not invalidated */
                                Assert::assertFalse(parser->isDfInvalidated());
                                /* expected dfName */
                                Assert::assertArrayEquals(ByteArrayUtils::fromHex(dfName), parser->getDfName());
                                /* expected Application Serial Number */
                                Assert::assertArrayEquals(ByteArrayUtils::fromHex(appSerialNumber), parser->getApplicationSerialNumber());
                                /* Buffer size indicator and value */
                                Assert::assertEquals(siBufferSizeIndicator, parser->getBufferSizeIndicator());
                                Assert::assertEquals(512, parser->getBufferSizeValue());
                                /* Platform */
                                Assert::assertEquals(siPlatform, parser->getPlatformByte());
                                /* ApplicationType */
                                Assert::assertEquals(siApplicationType, parser->getApplicationTypeByte());
                                Assert::assertTrue(parser->isRev3_2ModeAvailable());
                                Assert::assertTrue(parser->isRatificationCommandRequired());
                                Assert::assertTrue(parser->hasCalypsoPin());
                                Assert::assertFalse(parser->hasCalypsoStoredValue());
                                /* ApplicationSubType */
                                Assert::assertEquals(siApplicationSubtype, parser->getApplicationSubtypeByte());
                                /* SoftwareIssuer */
                                Assert::assertEquals(siSoftwareIssuer, parser->getSoftwareIssuerByte());
                                /* SoftwareVersion */
                                Assert::assertEquals(siSoftwareVersion, parser->getSoftwareVersionByte());
                                /* SoftwareRevision */
                                Assert::assertEquals(siSoftwareRevision, parser->getSoftwareRevisionByte());

                                /* Change startup information */
                                siBufferSizeIndicator = 16;
                                siPlatform = static_cast<char>(0xAA);
                                siApplicationType = static_cast<char>(0x55);
                                siApplicationSubtype = static_cast<char>(0xAA);
                                siSoftwareIssuer = static_cast<char>(0x55);
                                siSoftwareVersion = static_cast<char>(0xAA);
                                siSoftwareRevision = static_cast<char>(0x55);
                                startupInformation = std::string::format("%02X%02X%02X%02X%02X%02X%02X", siBufferSizeIndicator, siPlatform, siApplicationType, siApplicationSubtype, siSoftwareIssuer, siSoftwareVersion, siSoftwareRevision);
                                apduResponse = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("6F 24 84 0A " + dfName + " A5 16 BF0C 13 C7 08 " + appSerialNumber + " 53 07 " + startupInformation + "9000"), nullptr);

                                parser = std::make_shared<GetDataFciRespPars>(apduResponse);

                                /* valid Calypso FCI */
                                Assert::assertTrue(parser->isValidCalypsoFCI());
                                /* DF not invalidated */
                                Assert::assertFalse(parser->isDfInvalidated());
                                /* expected dfName */
                                Assert::assertArrayEquals(ByteArrayUtils::fromHex(dfName), parser->getDfName());
                                /* expected Application Serial Number */
                                Assert::assertArrayEquals(ByteArrayUtils::fromHex(appSerialNumber), parser->getApplicationSerialNumber());
                                /* Buffer size indicator and value */
                                Assert::assertEquals(siBufferSizeIndicator, parser->getBufferSizeIndicator());
                                Assert::assertEquals(1217, parser->getBufferSizeValue());
                                /* Platform */
                                Assert::assertEquals(siPlatform, parser->getPlatformByte());
                                /* ApplicationType */
                                Assert::assertEquals(siApplicationType, parser->getApplicationTypeByte());
                                Assert::assertFalse(parser->isRev3_2ModeAvailable());
                                Assert::assertFalse(parser->isRatificationCommandRequired());
                                Assert::assertFalse(parser->hasCalypsoPin());
                                Assert::assertTrue(parser->hasCalypsoStoredValue());
                                /* ApplicationSubType */
                                Assert::assertEquals(siApplicationSubtype, parser->getApplicationSubtypeByte());
                                /* SoftwareIssuer */
                                Assert::assertEquals(siSoftwareIssuer, parser->getSoftwareIssuerByte());
                                /* SoftwareVersion */
                                Assert::assertEquals(siSoftwareVersion, parser->getSoftwareVersionByte());
                                /* SoftwareRevision */
                                Assert::assertEquals(siSoftwareRevision, parser->getSoftwareRevisionByte());
                            }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFCIparser_FCI_Invalidated()
                            void GetDataRespParsTest::testFCIparser_FCI_Invalidated() {
                                std::string dfName = "00112233445566778899";
                                std::string appSerialNumber = "AABBCCDDEEFF0011";
                                char siBufferSizeIndicator = 11;
                                char siPlatform = static_cast<char>(0x55);
                                char siApplicationType = static_cast<char>(0xAA);
                                char siApplicationSubtype = static_cast<char>(0x55);
                                char siSoftwareIssuer = static_cast<char>(0xAA);
                                char siSoftwareVersion = static_cast<char>(0x55);
                                char siSoftwareRevision = static_cast<char>(0xAA);
                                std::string startupInformation = std::string::format("%02X%02X%02X%02X%02X%02X%02X", siBufferSizeIndicator, siPlatform, siApplicationType, siApplicationSubtype, siSoftwareIssuer, siSoftwareVersion, siSoftwareRevision);
                                std::shared_ptr<ApduResponse> apduResponse = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("6F 24 84 0A " + dfName + " A5 16 BF0C 13 C7 08 " + appSerialNumber + " 53 07 " + startupInformation + "6283"), nullptr);

                                std::shared_ptr<GetDataFciRespPars> parser = std::make_shared<GetDataFciRespPars>(apduResponse);

                                /* valid Calypso FCI */
                                Assert::assertTrue(parser->isValidCalypsoFCI());
                                Assert::assertTrue(parser->isDfInvalidated());
                            }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFCIparser_FCI_BadTags()
                            void GetDataRespParsTest::testFCIparser_FCI_BadTags() {
                                std::string dfName = "00112233445566778899";
                                std::string appSerialNumber = "AABBCCDDEEFF0011";
                                char siBufferSizeIndicator = 11;
                                char siPlatform = static_cast<char>(0x55);
                                char siApplicationType = static_cast<char>(0xAA);
                                char siApplicationSubtype = static_cast<char>(0x55);
                                char siSoftwareIssuer = static_cast<char>(0xAA);
                                char siSoftwareVersion = static_cast<char>(0x55);
                                char siSoftwareRevision = static_cast<char>(0xAA);
                                std::string startupInformation = std::string::format("%02X%02X%02X%02X%02X%02X%02X", siBufferSizeIndicator, siPlatform, siApplicationType, siApplicationSubtype, siSoftwareIssuer, siSoftwareVersion, siSoftwareRevision);

                                /* bad tag FCI Template (not constructed) */
                                std::shared_ptr<ApduResponse> apduResponse = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("4F 24 84 0A " + dfName + " A5 16 BF0C 13 C7 08 " + appSerialNumber + " 53 07 " + startupInformation + "9000"), nullptr);

                                std::shared_ptr<GetDataFciRespPars> parser = std::make_shared<GetDataFciRespPars>(apduResponse);

                                /* invalid Calypso FCI */
                                Assert::assertFalse(parser->isValidCalypsoFCI());

                                /* bad tag DF Name (constructed) */
                                apduResponse = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("6F 24 C4 0A " + dfName + " A5 16 BF0C 13 C7 08 " + appSerialNumber + " 53 07 " + startupInformation + "9000"), nullptr);

                                parser = std::make_shared<GetDataFciRespPars>(apduResponse);

                                /* invalid Calypso FCI */
                                Assert::assertFalse(parser->isValidCalypsoFCI());

                                /* bad tag FCI Proprietary Template (not constructed) */
                                apduResponse = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("6F 24 84 0A " + dfName + " 85 16 BF0C 13 C7 08 " + appSerialNumber + " 53 07 " + startupInformation + "9000"), nullptr);

                                parser = std::make_shared<GetDataFciRespPars>(apduResponse);

                                /* invalid Calypso FCI */
                                Assert::assertFalse(parser->isValidCalypsoFCI());

                                /* bad tag FCI Issuer Discretionary */
                                apduResponse = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("6F 24 84 0A " + dfName + " A5 16 BF0D 13 C7 08 " + appSerialNumber + " 53 07 " + startupInformation + "9000"), nullptr);

                                parser = std::make_shared<GetDataFciRespPars>(apduResponse);

                                /* invalid Calypso FCI */
                                Assert::assertFalse(parser->isValidCalypsoFCI());

                                /* bad tag Application Serial Number */
                                apduResponse = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("6F 24 84 0A " + dfName + " A5 16 BF0C 13 87 08 " + appSerialNumber + " 53 07 " + startupInformation + "9000"), nullptr);

                                parser = std::make_shared<GetDataFciRespPars>(apduResponse);

                                /* invalid Calypso FCI */
                                Assert::assertFalse(parser->isValidCalypsoFCI());

                                /* bad tag Discretionary Data */
                                apduResponse = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("6F 24 84 0A " + dfName + " A5 16 BF0C 13 C7 08 " + appSerialNumber + " 43 07 " + startupInformation + "9000"), nullptr);

                                parser = std::make_shared<GetDataFciRespPars>(apduResponse);

                                /* invalid Calypso FCI */
                                Assert::assertFalse(parser->isValidCalypsoFCI());
                            }
                        }
                    }
                }
            }
        }
    }
}
