#include "ResponseUtilsTest.h"
#include "../../../../../../../main/java/org/eclipse/keyple/calypso/command/po/parser/GetDataFciRespPars.h"
#include "../../../../../../../main/java/org/eclipse/keyple/calypso/command/po/parser/session/AbstractOpenSessionRespPars.h"
#include "../../../../../../../main/java/org/eclipse/keyple/calypso/command/po/parser/session/OpenSession32RespPars.h"
#include "../../../../../../../main/java/org/eclipse/keyple/calypso/command/po/parser/session/OpenSession24RespPars.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace util {
                    using GetDataFciRespPars = org::eclipse::keyple::calypso::command::po::parser::GetDataFciRespPars;
                    using AbstractOpenSessionRespPars = org::eclipse::keyple::calypso::command::po::parser::session::AbstractOpenSessionRespPars;
                    using OpenSession24RespPars = org::eclipse::keyple::calypso::command::po::parser::session::OpenSession24RespPars;
                    using OpenSession32RespPars = org::eclipse::keyple::calypso::command::po::parser::session::OpenSession32RespPars;
                    using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                    using org::junit::Assert;
                    using org::junit::Test;
                    using org::junit::runner::RunWith;
                    using org::mockito::junit::MockitoJUnitRunner;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void TestToFCI()
                    void ResponseUtilsTest::TestToFCI() {

                        // Case if
                        std::vector<char> apduResponse = {static_cast<char>(0x6F), 0x22, static_cast<char>(0x84), 0x08, 0x33, 0x4D, 0x54, 0x52, 0x2E, 0x49, 0x43, 0x41, static_cast<char>(0xA5), 0x16, static_cast<char>(0xBF), 0x0C, 0x13, static_cast<char>(0xC7), 0x08, 0x00, 0x00, 0x00, 0x00, 0x27, 0x4A, static_cast<char>(0x9A), static_cast<char>(0xB7), 0x53, 0x07, 0x0A, 0x3C, 0x11, 0x32, 0x14, 0x10, 0x01};

                        std::vector<char> aid = {0x33, 0x4D, 0x54, 0x52, 0x2E, 0x49, 0x43, 0x41};
                        // AID aidExpected = new AID(aid);
                        std::vector<char> fciProprietaryTemplate = {static_cast<char>(0xBF), 0x0C, 0x13, static_cast<char>(0xC7), 0x08, 0x00, 0x00, 0x00, 0x00, 0x27, 0x4A, static_cast<char>(0x9A), static_cast<char>(0xB7), 0x53, 0x07, 0x0A, 0x3C, 0x11, 0x32, 0x14, 0x10, 0x01};
                        std::vector<char> fciIssuerDiscretionaryData = {static_cast<char>(0xC7), 0x08, 0x00, 0x00, 0x00, 0x00, 0x27, 0x4A, static_cast<char>(0x9A), static_cast<char>(0xB7), 0x53, 0x07, 0x0A, 0x3C, 0x11, 0x32, 0x14, 0x10, 0x01};
                        std::vector<char> applicationSN = {0x00, 0x00, 0x00, 0x00, 0x27, 0x4A, static_cast<char>(0x9A), static_cast<char>(0xB7)};
                        std::shared_ptr<GetDataFciRespPars::StartupInformation> startupInfoExpected = std::make_shared<GetDataFciRespPars::StartupInformation>(static_cast<char>(0x0A), static_cast<char>(0x3C), static_cast<char>(0x11), static_cast<char>(0x32), static_cast<char>(0x14), static_cast<char>(0x10), static_cast<char>(0x01));

                        std::shared_ptr<GetDataFciRespPars::FCI> fciExpected = std::make_shared<GetDataFciRespPars::FCI>(aid, fciProprietaryTemplate, fciIssuerDiscretionaryData, applicationSN, startupInfoExpected);
                        std::shared_ptr<GetDataFciRespPars::FCI> fciTested = GetDataFciRespPars::toFCI(apduResponse);

                        Assert::assertArrayEquals(fciExpected->getApplicationSN(), fciTested->getApplicationSN());
                        Assert::assertArrayEquals(fciExpected->getFciIssuerDiscretionaryData(), fciTested->getFciIssuerDiscretionaryData());
                        Assert::assertArrayEquals(fciExpected->getFciProprietaryTemplate(), fciTested->getFciProprietaryTemplate());
                        Assert::assertEquals(fciExpected->getStartupInformation()->getApplicationSubtype(), fciTested->getStartupInformation()->getApplicationSubtype());
                        Assert::assertEquals(fciExpected->getStartupInformation()->getApplicationType(), fciTested->getStartupInformation()->getApplicationType());
                        Assert::assertEquals(fciExpected->getStartupInformation()->getBufferSizeIndicator(), fciTested->getStartupInformation()->getBufferSizeIndicator());
                        Assert::assertEquals(fciExpected->getStartupInformation()->getPlatform(), fciTested->getStartupInformation()->getPlatform());
                        Assert::assertEquals(fciExpected->getStartupInformation()->getSoftwareIssuer(), fciTested->getStartupInformation()->getSoftwareIssuer());
                        Assert::assertEquals(fciExpected->getStartupInformation()->getSoftwareRevision(), fciTested->getStartupInformation()->getSoftwareRevision());
                        Assert::assertEquals(fciExpected->getStartupInformation()->getSoftwareVersion(), fciTested->getStartupInformation()->getSoftwareVersion());

                        // Case else
                        std::vector<char> wrongApduResponse = {static_cast<char>(0x5F), 0x22, static_cast<char>(0x84), 0x08, 0x33, 0x4D, 0x54, 0x52, 0x2E, 0x49, 0x43, 0x41, static_cast<char>(0xA5), 0x16, static_cast<char>(0xBF), 0x0C, 0x13, static_cast<char>(0xC7), 0x08, 0x00, 0x00, 0x00, 0x00, 0x27, 0x4A, static_cast<char>(0x9A), static_cast<char>(0xB7), 0x53, 0x07, 0x0A, 0x3C, 0x11, 0x32, 0x14, 0x10, 0x01};

                        fciTested = GetDataFciRespPars::toFCI(wrongApduResponse);

                        Assert::assertNull(fciTested->getFciProprietaryTemplate());
                        Assert::assertNull(fciTested->getFciIssuerDiscretionaryData());
                        Assert::assertNull(fciTested->getApplicationSN());
                        // Assert.assertNull(fciTested.getStartupInformation());

                        // Case if else
                        wrongApduResponse = std::vector<char> {static_cast<char>(0x6F), 0x22, static_cast<char>(0x83), 0x08, 0x33, 0x4D, 0x54, 0x52, 0x2E, 0x49, 0x43, 0x41, static_cast<char>(0xA5), 0x16, static_cast<char>(0xBF), 0x0C, 0x13, static_cast<char>(0xC7), 0x08, 0x00, 0x00, 0x00, 0x00, 0x27, 0x4A, static_cast<char>(0x9A), static_cast<char>(0xB7), 0x53, 0x07, 0x0A, 0x3C, 0x11, 0x32, 0x14, 0x10, 0x01};

                        fciTested = GetDataFciRespPars::toFCI(wrongApduResponse);

                        Assert::assertNotNull(fciTested->getFciProprietaryTemplate());
                        Assert::assertNotNull(fciTested->getFciIssuerDiscretionaryData());
                        Assert::assertNotNull(fciTested->getApplicationSN());
                        Assert::assertNotNull(fciTested->getStartupInformation());

                        // Case if else 2
                        wrongApduResponse = std::vector<char> {static_cast<char>(0x6F), 0x22, static_cast<char>(0x84), 0x08, 0x33, 0x4D, 0x54, 0x52, 0x2E, 0x49, 0x43, 0x41, static_cast<char>(0xA6), 0x16, static_cast<char>(0xBF), 0x0C, 0x13, static_cast<char>(0xC7), 0x08, 0x00, 0x00, 0x00, 0x00, 0x27, 0x4A, static_cast<char>(0x9A), static_cast<char>(0xB7), 0x53, 0x07, 0x0A, 0x3C, 0x11, 0x32, 0x14, 0x10, 0x01};

                        fciTested = GetDataFciRespPars::toFCI(wrongApduResponse);

                        Assert::assertNull(fciTested->getFciProprietaryTemplate());
                        Assert::assertNotNull(fciTested->getFciIssuerDiscretionaryData());
                        Assert::assertNotNull(fciTested->getApplicationSN());
                        Assert::assertNotNull(fciTested->getStartupInformation());

                        // Case if else 3
                        wrongApduResponse = std::vector<char> {static_cast<char>(0x6F), 0x22, static_cast<char>(0x84), 0x08, 0x33, 0x4D, 0x54, 0x52, 0x2E, 0x49, 0x43, 0x41, static_cast<char>(0xA5), 0x16, static_cast<char>(0xAF), 0x0C, 0x13, static_cast<char>(0xC7), 0x08, 0x00, 0x00, 0x00, 0x00, 0x27, 0x4A, static_cast<char>(0x9A), static_cast<char>(0xB7), 0x53, 0x07, 0x0A, 0x3C, 0x11, 0x32, 0x14, 0x10, 0x01};

                        fciTested = GetDataFciRespPars::toFCI(wrongApduResponse);

                        Assert::assertNotNull(fciTested->getFciProprietaryTemplate());
                        Assert::assertNull(fciTested->getFciIssuerDiscretionaryData());
                        Assert::assertNotNull(fciTested->getApplicationSN());
                        Assert::assertNotNull(fciTested->getStartupInformation());

                        // Case if else 3bis
                        wrongApduResponse = std::vector<char> {static_cast<char>(0x6F), 0x22, static_cast<char>(0x84), 0x08, 0x33, 0x4D, 0x54, 0x52, 0x2E, 0x49, 0x43, 0x41, static_cast<char>(0xA5), 0x16, static_cast<char>(0xBF), 0x0D, 0x13, static_cast<char>(0xC7), 0x08, 0x00, 0x00, 0x00, 0x00, 0x27, 0x4A, static_cast<char>(0x9A), static_cast<char>(0xB7), 0x53, 0x07, 0x0A, 0x3C, 0x11, 0x32, 0x14, 0x10, 0x01};

                        fciTested = GetDataFciRespPars::toFCI(wrongApduResponse);

                        Assert::assertNotNull(fciTested->getFciProprietaryTemplate());
                        Assert::assertNull(fciTested->getFciIssuerDiscretionaryData());
                        Assert::assertNotNull(fciTested->getApplicationSN());
                        Assert::assertNotNull(fciTested->getStartupInformation());

                        // Case if else 4
                        wrongApduResponse = std::vector<char> {static_cast<char>(0x6F), 0x22, static_cast<char>(0x84), 0x08, 0x33, 0x4D, 0x54, 0x52, 0x2E, 0x49, 0x43, 0x41, static_cast<char>(0xA5), 0x16, static_cast<char>(0xBF), 0x0C, 0x13, static_cast<char>(0xC8), 0x08, 0x00, 0x00, 0x00, 0x00, 0x27, 0x4A, static_cast<char>(0x9A), static_cast<char>(0xB7), 0x53, 0x07, 0x0A, 0x3C, 0x11, 0x32, 0x14, 0x10, 0x01};

                        fciTested = GetDataFciRespPars::toFCI(wrongApduResponse);

                        Assert::assertNotNull(fciTested->getFciProprietaryTemplate());
                        Assert::assertNotNull(fciTested->getFciIssuerDiscretionaryData());
                        Assert::assertNull(fciTested->getApplicationSN());
                        Assert::assertNotNull(fciTested->getStartupInformation());

                        // Case if else 4
                        wrongApduResponse = std::vector<char> {static_cast<char>(0x6F), 0x22, static_cast<char>(0x84), 0x08, 0x33, 0x4D, 0x54, 0x52, 0x2E, 0x49, 0x43, 0x41, static_cast<char>(0xA5), 0x16, static_cast<char>(0xBF), 0x0C, 0x13, static_cast<char>(0xC7), 0x08, 0x00, 0x00, 0x00, 0x00, 0x27, 0x4A, static_cast<char>(0x9A), static_cast<char>(0xB7), 0x54, 0x07, 0x0A, 0x3C, 0x11, 0x32, 0x14, 0x10, 0x01};

                        fciTested = GetDataFciRespPars::toFCI(wrongApduResponse);

                        Assert::assertNotNull(fciTested->getFciProprietaryTemplate());
                        Assert::assertNotNull(fciTested->getFciIssuerDiscretionaryData());
                        Assert::assertNotNull(fciTested->getApplicationSN());
                        // Assert.assertNull(fciTested.getStartupInformation());

                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void TestToSecureSession()
                    void ResponseUtilsTest::TestToSecureSession() {
                        std::vector<char> apduResponse = {static_cast<char>(0x8F), 0x05, 0x75, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x7E, static_cast<char>(0x1D), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

                        std::vector<char> transactionCounter = {static_cast<char>(0x8F), 0x05, 0x75};
                        std::vector<char> randomNumber = {0x1A, 0x00, 0x00, 0x00, 0x00};
                        char kif = 0x00;
                        char kvc = static_cast<char>(0x00);

                        bool isPreviousSessionRatifiedExpected = true;
                        bool isManageSecureSessionAuthorizedExpected = false;
                        std::vector<char> originalData(0);

                        std::shared_ptr<AbstractOpenSessionRespPars::SecureSession> SecureSessionExpected = std::make_shared<AbstractOpenSessionRespPars::SecureSession>(transactionCounter, randomNumber, isPreviousSessionRatifiedExpected, isManageSecureSessionAuthorizedExpected, kif, kvc, originalData, apduResponse);
                        std::shared_ptr<AbstractOpenSessionRespPars::SecureSession> SecureSessionTested = OpenSession32RespPars::createSecureSession(apduResponse);

                        Assert::assertArrayEquals(SecureSessionExpected->getOriginalData(), SecureSessionTested->getOriginalData());
                        Assert::assertArrayEquals(SecureSessionExpected->getSecureSessionData(), SecureSessionTested->getSecureSessionData());
                        Assert::assertEquals(SecureSessionExpected->getKIF(), SecureSessionTested->getKIF());
                        Assert::assertEquals(SecureSessionExpected->getKVC(), SecureSessionTested->getKVC());
                        Assert::assertArrayEquals(SecureSessionExpected->getChallengeRandomNumber(), SecureSessionTested->getChallengeRandomNumber());
                        Assert::assertArrayEquals(SecureSessionExpected->getChallengeTransactionCounter(), SecureSessionTested->getChallengeTransactionCounter());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void TestToSecureSessionRev2()
                    void ResponseUtilsTest::TestToSecureSessionRev2() {

                        // Case Else
                        std::vector<char> apduResponse = {static_cast<char>(0x7E), static_cast<char>(0x03), static_cast<char>(0x0D), static_cast<char>(0x14), static_cast<char>(0x53)};

                        std::vector<char> transactionCounter = {static_cast<char>(0x03), static_cast<char>(0x0D), static_cast<char>(0x14)};
                        std::vector<char> randomNumber = {static_cast<char>(0x53)};
                        char kvc = static_cast<char>(0x7E);

                        bool isPreviousSessionRatifiedExpected = false;
                        bool isManageSecureSessionAuthorizedExpected = false;
                        std::vector<char> originalData;

                        std::shared_ptr<AbstractOpenSessionRespPars::SecureSession> SecureSessionExpected = std::make_shared<AbstractOpenSessionRespPars::SecureSession>(transactionCounter, randomNumber, isPreviousSessionRatifiedExpected, isManageSecureSessionAuthorizedExpected, kvc, originalData, apduResponse);
                        std::shared_ptr<AbstractOpenSessionRespPars::SecureSession> SecureSessionTested = OpenSession24RespPars::createSecureSession(apduResponse);

                        Assert::assertEquals(SecureSessionExpected->getSecureSessionData(), SecureSessionTested->getSecureSessionData());
                        Assert::assertEquals(SecureSessionExpected->getKVC(), SecureSessionTested->getKVC());
                        Assert::assertArrayEquals(SecureSessionExpected->getChallengeRandomNumber(), SecureSessionTested->getChallengeRandomNumber());
                        Assert::assertArrayEquals(SecureSessionExpected->getChallengeTransactionCounter(), SecureSessionTested->getChallengeTransactionCounter());

                        // Case If Else
                        // byte[] apduResponseCaseTwo = new byte[] {(byte) 0x7E, (byte) 0x03, (byte) 0x0D, (byte)
                        // 0x14,
                        // (byte) 0x53, (byte) 0x30, 0x00, 0x04, 0x01, 0x02, 0x03, 0x04};
                        // byte[] originalDataCaseTwo = new byte[] {(byte) 0x7E, (byte) 0x03, (byte) 0x0D, (byte)
                        // 0x14,
                        // (byte) 0x53, (byte) 0xFF, 0x00, 0x04, 0x01, 0x02, 0x03, 0x04};
                        std::vector<char> apduResponseCaseTwo = ByteArrayUtils::fromHex("7E 030D1453 9999 00112233445566778899AABBCCDDEEFF 00112233445566778899AABBCC");
                        std::vector<char> originalDataCaseTwo = ByteArrayUtils::fromHex("7E 030D1453 9999 00112233445566778899AABBCCDDEEFF 00112233445566778899AABBCC");

                        std::shared_ptr<AbstractOpenSessionRespPars::SecureSession> SecureSessionExpectedCaseTwo = std::make_shared<AbstractOpenSessionRespPars::SecureSession>(transactionCounter, randomNumber, isPreviousSessionRatifiedExpected, isManageSecureSessionAuthorizedExpected, kvc, originalDataCaseTwo, apduResponseCaseTwo);
                        std::shared_ptr<AbstractOpenSessionRespPars::SecureSession> SecureSessionTestedCaseTwo = OpenSession24RespPars::createSecureSession(apduResponseCaseTwo);

                        Assert::assertEquals(SecureSessionExpectedCaseTwo->getSecureSessionData(), SecureSessionTestedCaseTwo->getSecureSessionData());
                        Assert::assertEquals(SecureSessionExpectedCaseTwo->getKVC(), SecureSessionTestedCaseTwo->getKVC());
                        Assert::assertArrayEquals(SecureSessionExpectedCaseTwo->getChallengeRandomNumber(), SecureSessionTestedCaseTwo->getChallengeRandomNumber());
                        Assert::assertArrayEquals(SecureSessionExpectedCaseTwo->getChallengeTransactionCounter(), SecureSessionTestedCaseTwo->getChallengeTransactionCounter());

                        // Case If If
                        // byte[] apduResponseCaseThree = new byte[] {(byte) 0x7E, (byte) 0x03, (byte) 0x0D,
                        // (byte) 0x14, (byte) 0x53, (byte) 0xFF, 0x00, 0x04, 0x01, 0x02, 0x03, 0x04};
                        // byte[] originalDataCaseThree = new byte[] {(byte) 0x7E, (byte) 0x03, (byte) 0x0D,
                        // (byte) 0x14, (byte) 0x53, (byte) 0xFF, 0x00, 0x04, 0x01, 0x02, 0x03, 0x04};

                        std::vector<char> apduResponseCaseThree = ByteArrayUtils::fromHex("7E 030D1453 9999 00112233445566778899AABBCCDDEEFF 00112233445566778899AABBCC");
                        std::vector<char> originalDataCaseThree = ByteArrayUtils::fromHex("7E 030D1453 9999 00112233445566778899AABBCCDDEEFF 00112233445566778899AABBCC");

                        std::shared_ptr<AbstractOpenSessionRespPars::SecureSession> SecureSessionExpectedCaseThree = std::make_shared<AbstractOpenSessionRespPars::SecureSession>(transactionCounter, randomNumber, isPreviousSessionRatifiedExpected, isManageSecureSessionAuthorizedExpected, kvc, originalDataCaseThree, apduResponseCaseThree);
                        std::shared_ptr<AbstractOpenSessionRespPars::SecureSession> SecureSessionTestedCaseThree = OpenSession24RespPars::createSecureSession(apduResponseCaseThree);

                        Assert::assertEquals(SecureSessionExpectedCaseThree->getSecureSessionData(), SecureSessionTestedCaseThree->getSecureSessionData());
                        Assert::assertEquals(SecureSessionExpectedCaseThree->getKVC(), SecureSessionTestedCaseThree->getKVC());
                        Assert::assertArrayEquals(SecureSessionExpectedCaseThree->getChallengeRandomNumber(), SecureSessionTestedCaseThree->getChallengeRandomNumber());
                        Assert::assertArrayEquals(SecureSessionExpectedCaseThree->getChallengeTransactionCounter(), SecureSessionTestedCaseThree->getChallengeTransactionCounter());
                    }
                }
            }
        }
    }
}
