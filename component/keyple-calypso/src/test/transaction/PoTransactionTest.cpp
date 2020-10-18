/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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

#include "PoTransaction.h"

/* Calypso */
#include "CalypsoAtomicTransactionException.h"
#include "CalypsoAuthenticationNotVerifiedException.h"
#include "CalypsoPo.h"
#include "CalypsoPoTransactionIllegalStateException.h"
#include "CalypsoSam.h"
#include "CalypsoUnauthorizedKvcException.h"
#include "CalypsoPoCloseSecureSessionException.h"
#include "CalypsoSessionAuthenticationException.h"
#include "PoSecuritySettings.h"

/* Core */
#include "ByteArrayUtil.h"
#include "KeypleReaderIOException.h"
#include "ProxyReader.h"
#include "TransmissionMode.h"

using namespace testing;

using namespace keyple::calypso;
using namespace keyple::calypso::transaction;
using namespace keyple::calypso::transaction::exception;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;

using ModificationMode = PoTransaction::SessionSetting::ModificationMode;
using PoSecuritySettingsBuilder = PoSecuritySettings::PoSecuritySettingsBuilder;

/* The default KIF values for personalization, loading and debiting */
static const uint8_t DEFAULT_KIF_PERSO = 0x21;
static const uint8_t DEFAULT_KIF_LOAD  = 0x27;
static const uint8_t DEFAULT_KIF_DEBIT = 0x30;

/*
 * The default key record number values for personalization, loading and
 * debiting. The actual value should be adjusted.
 */
static const uint8_t DEFAULT_KEY_RECORD_NUMBER_PERSO = 0x01;
static const uint8_t DEFAULT_KEY_RECORD_NUMBER_LOAD  = 0x02;
static const uint8_t DEFAULT_KEY_RECORD_NUMBER_DEBIT = 0x03;

static std::shared_ptr<SeReader> poReader;
static std::shared_ptr<PoTransaction> poTransaction;
static std::shared_ptr<SeResource<CalypsoSam>> samResource;

static std::map<std::string, std::string> poCommandsTestSet;
static std::map<std::string, std::string> samCommandsTestSet;

static const std::string FCI_REV10 = "6F228408315449432E494341A516BF0C13C708 " \
                                     "  0000000011223344 5307060A010320031190" \
                                     "00";
static const std::string FCI_REV24 = "6F2A8410A000000404012509010100000000000" \
                                     "0A516BF0C13C708 0000000011223344 53070A" \
                                     "2E11420001019000";
static const std::string FCI_REV31 = "6F238409315449432E49434131A516BF0C13C70" \
                                     "8 0000000011223344 53070A3C231214100190" \
                                     "00";

static const std::string ATR1 = "3B3F9600805A0080C120000012345678829000";

static const uint8_t FILE7  = 0x07;
static const uint8_t FILE8  = 0x08;
static const uint8_t FILE9  = 0x09;
static const uint8_t FILE10 = 0x10;
static const uint8_t FILE11 = 0x11;

static const std::string SW1SW2_OK      = "9000";
static const std::string SAM_CHALLENGE  = "C1C2C3C4";
static const std::string PO_DIVERSIFIER = "0000000011223344";
static const std::string SAM_SIGNATURE  = "12345678";
static const std::string PO_SIGNATURE   = "9ABCDEF0";

static const std::string FILE7_REC1_29B = "7111111111111111111111111111111111" \
                                          "111111111111111111111111";
static const std::string FILE7_REC2_29B = "7222222222222222222222222222222222" \
                                          "222222222222222222222222";
static const std::string FILE7_REC3_29B = "7333333333333333333333333333333333" \
                                          "333333333333333333333333";
static const std::string FILE7_REC4_29B = "7444444444444444444444444444444444" \
                                          "444444444444444444444444";
static const std::string FILE7_REC1_4B  = "00112233";
static const std::string FILE8_REC1_29B = "8111111111111111111111111111111111" \
                                          "111111111111111111111111";
static const std::string FILE8_REC1_5B  = "8122334455";
static const std::string FILE8_REC1_4B  = "84332211";
static const std::string FILE9_REC1_4B  = "8899AABB";

static const std::string FILE10_REC1_COUNTER = "00112200000000000000000000000" \
                                               "00000000000000000000000000000" \
                                               "0000000000";
static const std::string FILE11_REC1_COUNTER = "00221100000000000000000000000" \
                                               "00000000000000000000000000000" \
                                               "0000000000";
static const std::string FILE7_REC1_COUNTER1 = "A55AA5";
static const std::string FILE7_REC1_COUNTER2 = "5AA55A";

static const std::vector<uint8_t> FILE7_REC1_29B_BYTES =
    ByteArrayUtil::fromHex(FILE7_REC1_29B);
static const std::vector<uint8_t> FILE7_REC2_29B_BYTES =
    ByteArrayUtil::fromHex(FILE7_REC2_29B);
static const std::vector<uint8_t> FILE7_REC3_29B_BYTES =
    ByteArrayUtil::fromHex(FILE7_REC3_29B);
static const std::vector<uint8_t> FILE7_REC4_29B_BYTES =
    ByteArrayUtil::fromHex(FILE7_REC4_29B);
static const std::vector<uint8_t> FILE8_REC1_29B_BYTES =
    ByteArrayUtil::fromHex(FILE8_REC1_29B);
static const std::vector<uint8_t> FILE8_REC1_5B_BYTES =
    ByteArrayUtil::fromHex(FILE8_REC1_5B);
static const std::vector<uint8_t> FILE8_REC1_4B_BYTES =
    ByteArrayUtil::fromHex(FILE8_REC1_4B);

static const uint16_t LID_3F00 = 0x3F00;
static const uint16_t LID_0002 = 0x0002;
static const uint16_t LID_0003 = 0x0003;

static const std::string LID_3F00_STR           = "3F00";
static const std::string LID_0002_STR           = "0002";
static const std::string LID_0003_STR           = "0003";
static const std::string ACCESS_CONDITIONS_3F00 = "10100000";
static const std::string KEY_INDEXES_3F00       = "01030101";
static const std::string ACCESS_CONDITIONS_0002 = "1F000000";
static const std::string KEY_INDEXES_0002       = "01010101";
static const std::string ACCESS_CONDITIONS_0003 = "01100000";
static const std::string KEY_INDEXES_0003       = "01020101";

static const std::string SW1SW2_OK_RSP = SW1SW2_OK;
static const std::string PO_OPEN_SECURE_SESSION_SFI7_REC1_CMD =
     "008A0B3904" + SAM_CHALLENGE + "00";
static const std::string PO_OPEN_SECURE_SESSION_SFI7_REC1_RSP =
    "030490980030791D" + FILE7_REC1_29B + SW1SW2_OK;
static const std::string PO_OPEN_SECURE_SESSION_SFI7_REC1_NOT_RATIFIED_RSP =
    "030490980130791D" + FILE7_REC1_29B + SW1SW2_OK;
static const std::string PO_OPEN_SECURE_SESSION_CMD =
    "008A030104" + SAM_CHALLENGE + "00";
static const std::string PO_OPEN_SECURE_SESSION_RSP =
    "0304909800307900" + SW1SW2_OK;
static const std::string PO_OPEN_SECURE_SESSION_KVC_78_CMD =
    "0304909800307800" + SW1SW2_OK;
static const std::string PO_OPEN_SECURE_SESSION_SFI7_REC1_2_4_CMD =
    "948A8B3804C1C2C3C400";
static const std::string PO_OPEN_SECURE_SESSION_SFI7_REC1_2_4_RSP =
    "79030D307124B928480805CBABAE30001240800000000000000000000000000000009000";
static const std::string PO_CLOSE_SECURE_SESSION_CMD =
    "008E800004" + SAM_SIGNATURE + "00";
static const std::string PO_CLOSE_SECURE_SESSION_NOT_RATIFIED_CMD =
    "008E000004" + SAM_SIGNATURE + "00";
static const std::string PO_CLOSE_SECURE_SESSION_RSP =
    PO_SIGNATURE + SW1SW2_OK;
static const std::string PO_CLOSE_SECURE_SESSION_FAILED_RSP =
    "6988";
static const std::string PO_ABORT_SECURE_SESSION_CMD =
    "008E000000";
static const std::string PO_RATIFICATION_CMD =
    "00B2000000";
static const std::string PO_RATIFICATION_RSP =
    "6B00";

static const std::string PO_READ_REC_SFI7_REC1_CMD =
    "00B2013C00";
static const std::string PO_READ_REC_SFI7_REC1_RSP =
    FILE7_REC1_29B + SW1SW2_OK;
static const std::string PO_READ_REC_SFI7_REC1_6B_COUNTER_CMD =
    "00B2013C06";
static const std::string PO_READ_REC_SFI7_REC1_6B_COUNTER_RSP =
    FILE7_REC1_COUNTER1 + FILE7_REC1_COUNTER2 + SW1SW2_OK;
static const std::string PO_READ_REC_SFI8_REC1_CMD =
    "00B2014400";
static const std::string PO_READ_REC_SFI8_REC1_RSP =
    FILE8_REC1_29B + SW1SW2_OK;
static const std::string PO_READ_REC_SFI7_REC3_4_CMD =
    "00B2033D3E";
static const std::string PO_READ_REC_SFI7_REC3_4_RSP =
    "031D" + FILE7_REC3_29B + "041D" + FILE7_REC4_29B + SW1SW2_OK;
static const std::string PO_READ_REC_SFI10_REC1_CMD =
    "00B2018400";
static const std::string PO_READ_REC_SFI10_REC1_RSP =
    FILE10_REC1_COUNTER + SW1SW2_OK;
static const std::string PO_READ_REC_SFI11_REC1_CMD =
    "00B2018C00";
static const std::string PO_READ_REC_SFI11_REC1_RSP =
    FILE11_REC1_COUNTER + SW1SW2_OK;
static const std::string PO_UPDATE_REC_SFI7_REC1_4B_CMD =
    "00DC013C0400112233";
static const std::string PO_UPDATE_REC_SFI8_REC1_29B_CMD =
    "00DC01441D" + FILE8_REC1_29B;
static const std::string PO_UPDATE_REC_SFI8_REC1_5B_CMD =
    "00DC014405" + FILE8_REC1_5B;
static const std::string PO_UPDATE_REC_SFI8_REC1_4B_CMD =
     "00DC014404" + FILE8_REC1_4B;
static const std::string PO_UPDATE_REC_SFI8_REC1_29B_2_4_CMD =
    "94DC01441D" + FILE8_REC1_29B;
static const std::string PO_WRITE_REC_SFI8_REC1_4B_CMD =
    "00D2014404" + FILE8_REC1_4B;
static const std::string PO_APPEND_REC_SFI9_REC1_4B_CMD =
    "00E2004804" + FILE9_REC1_4B;
static const std::string PO_DECREASE_SFI10_REC1_100U_CMD =
    "003001800300006400";
static const std::string PO_DECREASE_SFI10_REC1_100U_RSP =
    "0010BE9000";
static const std::string PO_DECREASE_SFI11_REC1_100U_CMD =
    "003201880300006400";
static const std::string PO_DECREASE_SFI11_REC1_100U_RSP =
    "0022759000";

static const std::string PO_SELECT_FILE_CURRENT_CMD = "00A4090002000000";
static const std::string PO_SELECT_FILE_FIRST_CMD = "00A4020002000000";
static const std::string PO_SELECT_FILE_NEXT_CMD = "00A4020202000000";
static const std::string PO_SELECT_FILE_3F00_CMD = "00A40900023F0000";
static const std::string PO_SELECT_FILE_0002_CMD = "00A4090002000200";
static const std::string PO_SELECT_FILE_0003_CMD = "00A4090002000300";
static const std::string PO_SELECT_FILE_3F00_RSP = "85170001000000" +
                                                   ACCESS_CONDITIONS_3F00 +
                                                   KEY_INDEXES_3F00 +
                                                   "00777879616770003F009000";
static const std::string PO_SELECT_FILE_0002_RSP = "85170204021D01" +
                                                   ACCESS_CONDITIONS_0002 +
                                                   KEY_INDEXES_0002 +
                                                   "003F02000000000000029000";
static const std::string PO_SELECT_FILE_0003_RSP = "85170304021D01" +
                                                   ACCESS_CONDITIONS_0003 +
                                                   KEY_INDEXES_0003 +
                                                   "003F03000000000000039000";


static const std::string SAM_SELECT_DIVERSIFIER_CMD =
    "8014000008" + PO_DIVERSIFIER;
static const std::string SAM_GET_CHALLENGE_CMD =
    "8084000004";
static const std::string SAM_GET_CHALLENGE_RSP =
    SAM_CHALLENGE + SW1SW2_OK;
static const std::string SAM_DIGEST_INIT_OPEN_SECURE_SESSION_SFI7_REC1_CMD =
    "808A00FF273079030490980030791D" + FILE7_REC1_29B;
static const std::string SAM_DIGEST_INIT_OPEN_SECURE_SESSION_CMD =
    "808A00FF0A30790304909800307900";
static const std::string SAM_DIGEST_UPDATE_READ_REC_SFI7_REC1_CMD =
    "808C00000500B2013C00";
static const std::string SAM_DIGEST_UPDATE_READ_REC_SFI7_REC1_RSP_CMD =
    "808C00001F\" + FILE7_REC1_29B+ \"9000";
static const std::string SAM_DIGEST_UPDATE_READ_REC_SFI8_REC1_RSP_CMD =
    "808C00001F" + FILE8_REC1_29B + "9000";
static const std::string SAM_DIGEST_UPDATE_READ_REC_SFI7_REC1_RSP =
    "808C00001F" + FILE7_REC1_29B + SW1SW2_OK;
static const std::string SAM_DIGEST_UPDATE_READ_REC_SFI8_REC1_CMD =
    "808C00000500B2014400";
static const std::string SAM_DIGEST_UPDATE_READ_REC_SFI10_REC1_CMD =
    "808C00000500B2018C00";
static const std::string SAM_DIGEST_UPDATE_READ_REC_SFI10_REC1_RSP_CMD =
    "808C00002400112200000000000000000000000000000000000000000000000000000000" \
    "0000009000";
static const std::string SAM_DIGEST_UPDATE_READ_REC_SFI11_REC1_CMD =
    "808C00000500B2018400";
static const std::string SAM_DIGEST_UPDATE_READ_REC_SFI11_REC1_RSP_CMD =
    "808C00002400221100000000000000000000000000000000000000000000000000000000" \
    "0000009000";
static const std::string SAM_DIGEST_UPDATE_RSP_OK_CMD =
    "808C0000029000";
static const std::string SAM_DIGEST_UPDATE_UPDATE_REC_SFI8_REC1_29B_CMD =
    "808C00002200DC01441D" + FILE8_REC1_29B;
static const std::string SAM_DIGEST_UPDATE_UPDATE_REC_SFI8_REC1_5B_CMD =
    "808C00000A00DC0144058122334455";
static const std::string SAM_DIGEST_UPDATE_UPDATE_REC_SFI8_REC1_4B_CMD =
    "808C00000900DC014404" + FILE8_REC1_4B;
static const std::string SAM_DIGEST_UPDATE_UPDATE_REC_SFI7_REC1_4B_CMD =
    "808C00000900DC013C04" + FILE7_REC1_4B;
static const std::string SAM_DIGEST_UPDATE_DECREASE_SFI10_CMD =
    "808C0000080030018003000064";
static const std::string SAM_DIGEST_UPDATE_DECREASE_SFI10_RESP =
    "808C0000050010BE9000";
static const std::string SAM_DIGEST_UPDATE_INCREASE_SFI11_CMD =
    "808C0000080032018803000064";
static const std::string SAM_DIGEST_UPDATE_INCREASE_SFI11_RESP =
    "808C0000050022759000";
static const std::string SAM_DIGEST_UPDATE_WRITE_REC_SFI8_REC1_4B_CMD =
    "808C00000900D2014404" + FILE8_REC1_4B;
static const std::string SAM_DIGEST_UPDATE_APPEND_REC_SFI9_REC1_4B_CMD =
    "808C00000900E2004804" + FILE9_REC1_4B;
static const std::string SAM_DIGEST_CLOSE_CMD =
    "808E000004";
static const std::string SAM_DIGEST_CLOSE_RSP =
    SAM_SIGNATURE + SW1SW2_OK;
static const std::string SAM_DIGEST_AUTHENTICATE =
    "8082000004" + PO_SIGNATURE;
static const std::string SAM_DIGEST_AUTHENTICATE_FAILED =
    "6988";

class PT_ProxyReaderMock : public ProxyReader {
public:
    ~PT_ProxyReaderMock() = default;

    MOCK_METHOD((const std::string&),
                getName,
                (),
                (const, override));

    MOCK_METHOD(bool,
                isSePresent,
                (),
                (override));

    MOCK_METHOD((std::vector<std::shared_ptr<SeResponse>>),
                transmitSeRequests,
                (const std::vector<std::shared_ptr<SeRequest>>& seRequests,
                 const MultiSeRequestProcessing& multiSeRequestProcessing,
                 const ChannelControl& channelControl),
                (override));

    MOCK_METHOD((const std::map<const std::string, const std::string>&),
                getParameters,
                (),
                (const, override));

    MOCK_METHOD(void,
                setParameter,
                (const std::string& key, const std::string&),
                (override));

    MOCK_METHOD(void,
                setParameters,
                ((const std::map<const std::string, const std::string>&)),
                (override));

    MOCK_METHOD(void,
                addSeProtocolSetting,
                (std::shared_ptr<SeProtocol>, const std::string&),
                (override));

    MOCK_METHOD(void,
                setSeProtocolSetting,
                ((const std::map<std::shared_ptr<SeProtocol>, std::string>&)),
                (override));

    MOCK_METHOD((const TransmissionMode&),
                getTransmissionMode,
                (),
                (const, override));

    MOCK_METHOD((std::shared_ptr<SeResponse>),
                transmitSeRequest,
                (std::shared_ptr<SeRequest>, ChannelControl),
                (override));
};

static std::shared_ptr<CalypsoPo> createCalypsoPo(const std::string& FCI)
{
    auto selectionData =
        std::make_shared<SeResponse>(
            true,
            false,
            std::make_shared<SelectionStatus>(
                nullptr,
                std::make_shared<ApduResponse>(ByteArrayUtil::fromHex(FCI),
                                               nullptr),
                true),
            std::vector<std::shared_ptr<ApduResponse>>{});

    return std::make_shared<CalypsoPo>(selectionData,
                                       TransmissionMode::CONTACTLESS);
}

static std::shared_ptr<CalypsoSam> createCalypsoSam()
{
    auto selectionStatus =
        std::make_shared<SelectionStatus>(
            std::make_shared<AnswerToReset>(ByteArrayUtil::fromHex(ATR1)),
            nullptr,
            true);

    return std::make_shared<CalypsoSam>(
               std::make_shared<SeResponse>(
                   true,
                   true,
                   selectionStatus,
                   std::vector<std::shared_ptr<ApduResponse>>{}),
               TransmissionMode::CONTACTS);
}


static std::shared_ptr<ApduResponse> getResponses(
    const std::string& name,
    std::map<std::string, std::string>* cmdRespMap,
    const std::shared_ptr<ApduRequest> apduRequest)
{
    (void)name;

    const std::string apdu_c = ByteArrayUtil::toHex(apduRequest->getBytes());

    const auto it = cmdRespMap->find(apdu_c);

    /* Return matching hexa response if found */
    if (it != cmdRespMap->end())
        return std::make_shared<ApduResponse>(
                   ByteArrayUtil::fromHex(it->second), nullptr);

    /* Throw a KeypleReaderIOException if not found */
    throw KeypleReaderIOException("No response available for this request.");
}

static std::shared_ptr<ProxyReader> createMockReader(
    const std::string& name,
    TransmissionMode transmissionMode,
    std::map<std::string, std::string>* commandTestSet)
{
    /* Configure mock native reader */
    auto mockReader = std::make_shared<PT_ProxyReaderMock>();
    ON_CALL(*mockReader, getName())
        .WillByDefault(ReturnRef(name));
    ON_CALL(*mockReader, getTransmissionMode())
        .WillByDefault(ReturnRef(transmissionMode));
    EXPECT_CALL(*mockReader, transmitSeRequest(_,_))
        .WillRepeatedly(
            Invoke(
                [&, name, commandTestSet]
                (std::shared_ptr<SeRequest> seRequest,
                 ChannelControl channelControl) {

                    (void)channelControl;

                    std::vector<std::shared_ptr<ApduRequest>> apduRequests =
                        seRequest->getApduRequests();
                    std::vector<std::shared_ptr<ApduResponse>> apduResponses;

                    try {
                        for (const auto& apduRequest : apduRequests) {
                            std::shared_ptr<ApduResponse> apduResponse =
                                getResponses(name, commandTestSet, apduRequest);
                            apduResponses.push_back(apduResponse);
                        }
                    } catch (KeypleReaderIOException& ex) {
                        ex.setSeResponse(
                            std::make_shared<SeResponse>(true,
                                                         true,
                                                         nullptr,
                                                         apduResponses));
                        throw ex;
                    }

                    return std::make_shared<SeResponse>(
                            true,
                            true,
                            nullptr,
                            apduResponses);
                }
            ));

    return mockReader;
}

static void setUp()
{
    poCommandsTestSet.clear();
    samCommandsTestSet.clear();

    poReader = createMockReader("PO",
                                TransmissionMode::CONTACTLESS,
                                &poCommandsTestSet);

    std::shared_ptr<SeReader> samReader =
        createMockReader("SAM",
                         TransmissionMode::CONTACTS,
                         &samCommandsTestSet);

    std::shared_ptr<CalypsoSam> calypsoSam = createCalypsoSam();

    samResource = std::make_shared<SeResource<CalypsoSam>>(samReader,
                                                           calypsoSam);
}

static void tearDown()
{
    EXPECT_TRUE(Mock::VerifyAndClearExpectations(poReader.get()));
}

/* Standard opening with two records read */
TEST(PoTransactionTest, testProcessOpening_noSamResource)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    /* PoTransaction without PoSecuritySettings */
    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(
                            poReader, calypsoPoRev31));

    /* should raise an exception */
    EXPECT_THROW(poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT),
                 CalypsoPoTransactionIllegalStateException);

    tearDown();
}

/* Standard opening with two records read */
TEST(PoTransactionTest, testProcessOpening_readReopen)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);
    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev31),
                        poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP});
    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_SFI7_REC1_CMD,
                              PO_OPEN_SECURE_SESSION_SFI7_REC1_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI8_REC1_CMD,
                              PO_READ_REC_SFI8_REC1_RSP});

    poTransaction->prepareReadRecordFile(FILE7, 1);
    poTransaction->prepareReadRecordFile(FILE8, 1);
    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE7)->getData()->getContent(),
              ByteArrayUtil::fromHex(FILE7_REC1_29B));
    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE8)->getData()->getContent(),
              ByteArrayUtil::fromHex(FILE8_REC1_29B));
    ASSERT_TRUE(calypsoPoRev31->isDfRatified());

    /* Expected exception: session is already open */
    EXPECT_THROW(poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT),
                 CalypsoPoTransactionIllegalStateException);

    tearDown();
}

/* Standard opening, DF not previously ratified */
TEST(PoTransactionTest, testProcessOpening_dfNotRatified)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);
    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev31),
                        poSecuritySettings);


    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP});

    /*
     * addCommandPo(OPEN_SECURE_SESSION_SFI7_REC1,
     *              SAM_CHALLENGE_4,
     *              OPEN_SECURE_SESSION_RESP +
     *              std::string.format("%02X", FILE_7_REC_1.length() / 2) +
     *              FILE_7_REC_1, SW1SW2_OK);
     */
    poCommandsTestSet.insert({
        PO_OPEN_SECURE_SESSION_SFI7_REC1_CMD,
        PO_OPEN_SECURE_SESSION_SFI7_REC1_NOT_RATIFIED_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI8_REC1_CMD,
                              PO_READ_REC_SFI8_REC1_RSP});

    poTransaction->prepareReadRecordFile(FILE7, 1);
    poTransaction->prepareReadRecordFile(FILE8, 1);
    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE7)->getData()->getContent(),
              ByteArrayUtil::fromHex(FILE7_REC1_29B));
    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE8)->getData()->getContent(),
              ByteArrayUtil::fromHex(FILE8_REC1_29B));
    ASSERT_FALSE(calypsoPoRev31->isDfRatified());

    tearDown();
}

/* Standard opening with 1 multiple records read */
TEST(PoTransactionTest, testProcessOpening_readMultipleRecords)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);
    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev31),
                        poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP});
    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_CMD,
                              PO_OPEN_SECURE_SESSION_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI7_REC3_4_CMD,
                              PO_READ_REC_SFI7_REC3_4_RSP});

    poTransaction->prepareReadRecordFile(FILE7, 3, 2, 29);
    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE7)->getData()->getContent(3),
              ByteArrayUtil::fromHex(FILE7_REC3_29B));
    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE7)->getData()->getContent(4),
              ByteArrayUtil::fromHex(FILE7_REC4_29B));

    tearDown();
}

/* Standard opening but KVC is not present authorized list */
TEST(PoTransactionTest, testProcessOpening_kvcNotAuthorized)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::vector<uint8_t> authorizedKvc;
    authorizedKvc.push_back(0x79);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .sessionAuthorizedKvcList(authorizedKvc)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev31),
                        poSecuritySettings);
    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP});
    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_SFI7_REC1_CMD,
                              PO_OPEN_SECURE_SESSION_KVC_78_CMD});

    poTransaction->prepareReadRecordFile(FILE7, 1);

    /* An exception is expected */
    EXPECT_THROW(poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT),
                 CalypsoUnauthorizedKvcException);

    tearDown();
}

/*
 * Buffer overflow limit in atomic mode (counter in bytes): session buffer size
 * = 430 b, consumed size 430 b
 */
TEST(PoTransactionTest, testProcessOpening_sessionBuffer_limit)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev31),
                        poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP});
    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_SFI7_REC1_CMD,
                              PO_OPEN_SECURE_SESSION_SFI7_REC1_RSP});
    poCommandsTestSet.insert({PO_UPDATE_REC_SFI8_REC1_29B_CMD,
                              SW1SW2_OK_RSP});
    poCommandsTestSet.insert({PO_UPDATE_REC_SFI8_REC1_4B_CMD,
                              SW1SW2_OK_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI7_REC1_CMD,
                              PO_READ_REC_SFI7_REC1_RSP});

    /*
     * Add additional non modifying commands (should not affect the session
     * buffer)
     */
    for (int i = 0; i < 4; i++)
        poTransaction->prepareReadRecordFile(FILE7, 1);

    /*
     * 12 x update (29 b) = 12 x (29 + 6) = 420 consumed in the session buffer
     */
    for (int i = 0; i < 12; i++)
        poTransaction->prepareUpdateRecord(FILE8, 1, FILE8_REC1_29B_BYTES);

    /*
     * Insert additional non modifying commands (should not affect the session
     * buffer)
     */
    for (int i = 0; i < 4; i++)
        poTransaction->prepareReadRecordFile(FILE7, 1);

    /* 4 additional bytes (10 b consumed) */
    poTransaction->prepareUpdateRecord(FILE8, 1, FILE8_REC1_4B_BYTES);

    /* ATOMIC transaction should be ok (430 / 430 bytes consumed) */
    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

    tearDown();
}

/*
 * Buffer overflowed in atomic mode (counter in bytes): session buffer size = 430 b, consumed
 * size 431 b
 */
TEST(PoTransactionTest, testProcessOpening_sessionBuffer_overflowBytesCounter)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev31),
                        poSecuritySettings);

    poTransaction->prepareReadRecordFile(FILE7, 1);

    /*
     * 12 x update (29 b) = 12 x (29 + 6) = 420 consumed in the session buffer
     */
    for (int i = 0; i < 12; i++)
        poTransaction->prepareUpdateRecord(FILE8, 1, FILE8_REC1_29B_BYTES);

    /* 5 additional bytes (11 b consumed) */
    poTransaction->prepareUpdateRecord(FILE8, 1, FILE8_REC1_5B_BYTES);

    /* Expected exception: session buffer overflow */
    EXPECT_THROW(poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT),
                 CalypsoAtomicTransactionException);

    tearDown();
}

/*
 * Buffer overflow limit in atomic mode (counter in operations): session buffer size = 6 op,
 * consumed 6 op
 */
TEST(PoTransactionTest, testProcessOpening_sessionBuffer_limitOperationsCounter)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev24 = createCalypsoPo(FCI_REV24);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev24),
                        poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP});
    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_SFI7_REC1_2_4_CMD,
                              PO_OPEN_SECURE_SESSION_SFI7_REC1_2_4_RSP});
    poCommandsTestSet.insert({PO_UPDATE_REC_SFI8_REC1_29B_2_4_CMD,
                              SW1SW2_OK_RSP});

    poTransaction->prepareReadRecordFile(FILE7, 1);

    /* 6 x update (29 b) = 6 operations consumed in the session buffer */
    for (int i = 0; i < 6; i++)
        poTransaction->prepareUpdateRecord(FILE8, 1, FILE8_REC1_29B_BYTES);

    /* ATOMIC transaction should be ok (6 / 6 operations consumed) */
    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

    tearDown();
}

/*
 * Buffer overflow limit in atomic mode (counter in operations): session buffer
 * size = 6 op, consumed 7 op
 */
TEST(PoTransactionTest,
     testProcessOpening_sessionBuffer_overflowOperationsCounter)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev24 = createCalypsoPo(FCI_REV24);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev24),
                        poSecuritySettings);

    poTransaction->prepareReadRecordFile(FILE7, 1);

    /* 7 x update (29 b) = 7 operations consumed in the session buffer */
    for (int i = 0; i < 7; i++)
        poTransaction->prepareUpdateRecord(FILE8, 1, FILE8_REC1_29B_BYTES);

    /*
     * ATOMIC transaction should be ko (7 / 6 operations consumed)
     * expected exception: session buffer overflow
     */
    EXPECT_THROW(poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT),
                 CalypsoAtomicTransactionException);

    tearDown();
}

/*
 * Buffer overflowed in multiple mode (counter in bytes): session buffer size =
 * 430 b, consumed size 431 b
 */
TEST(PoTransactionTest,
     testProcessOpening_sessionBuffer_overflowBytesCounter_MulitpleMode)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .sessionModificationMode(ModificationMode::MULTIPLE)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev31),
                        poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP});
    samCommandsTestSet.insert(
        {SAM_DIGEST_INIT_OPEN_SECURE_SESSION_SFI7_REC1_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_UPDATE_REC_SFI8_REC1_29B_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_RSP_OK_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_CLOSE_CMD, SAM_DIGEST_CLOSE_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_AUTHENTICATE, SW1SW2_OK_RSP});

    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_SFI7_REC1_CMD,
                              PO_OPEN_SECURE_SESSION_SFI7_REC1_RSP});
    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_CMD,
                              PO_OPEN_SECURE_SESSION_RSP});
    poCommandsTestSet.insert({PO_UPDATE_REC_SFI8_REC1_29B_CMD, SW1SW2_OK_RSP});
    poCommandsTestSet.insert({PO_UPDATE_REC_SFI8_REC1_5B_CMD, SW1SW2_OK_RSP});
    poCommandsTestSet.insert({PO_CLOSE_SECURE_SESSION_CMD,
                              PO_CLOSE_SECURE_SESSION_RSP});

    poTransaction->prepareReadRecordFile(FILE7, 1);

    /*
     * 12 x update (29 b) = 12 x (29 + 6) = 420 consumed in the session buffer
     */
    for (int i = 0; i < 12; i++)
        poTransaction->prepareUpdateRecord(FILE8, 1, FILE8_REC1_29B_BYTES);

    /* 5 additional bytes (11 b consumed) */
    poTransaction->prepareUpdateRecord(FILE8, 1, FILE8_REC1_5B_BYTES);

    /* ATOMIC transaction should be ok (430 / 431 bytes consumed) */
    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

    tearDown();
}

/* standard process Po commands */
TEST(PoTransactionTest, testProcessPoCommands_nominalCase)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(
                            poReader, calypsoPoRev31));

    poCommandsTestSet.insert({PO_READ_REC_SFI7_REC1_CMD,
                              PO_READ_REC_SFI7_REC1_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI8_REC1_CMD,
                              PO_READ_REC_SFI8_REC1_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI7_REC3_4_CMD,
                              PO_READ_REC_SFI7_REC3_4_RSP});

    poTransaction->prepareReadRecordFile(FILE7, 1);
    poTransaction->prepareReadRecordFile(FILE8, 1);
    poTransaction->prepareReadRecordFile(FILE7, 3, 2, 29);
    poTransaction->processPoCommands(ChannelControl::KEEP_OPEN);

    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE8)->getData()->getContent(1),
              FILE8_REC1_29B_BYTES);
    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE7)->getData()->getContent(1),
              FILE7_REC1_29B_BYTES);
    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE7)->getData()->getContent(3),
              FILE7_REC3_29B_BYTES);
    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE7)->getData()->getContent(4),
              FILE7_REC4_29B_BYTES);

    tearDown();
}

/* standard process Po commands: session open before */
TEST(PoTransactionTest, testProcessPoCommands_sessionOpen)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .sessionModificationMode(ModificationMode::MULTIPLE)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev31),
                        poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP});
    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_SFI7_REC1_CMD,
                              PO_OPEN_SECURE_SESSION_SFI7_REC1_RSP});

    poTransaction->prepareReadRecordFile(FILE7, 1);
    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);
    poTransaction->prepareReadRecordFile(FILE8, 1);

    /*
     * PoTransaction while a session is open
     * expected exception: a session is open
     */
    EXPECT_THROW(poTransaction->processPoCommands(ChannelControl::KEEP_OPEN),
                 CalypsoPoTransactionIllegalStateException);

    tearDown();
}

/* No session open */
TEST(PoTransactionTest, testProcessPoCommandsInSession_noSessionOpen)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(
                            poReader, calypsoPoRev31));

    poTransaction->prepareReadRecordFile(FILE8, 1);

    /* Expected exception: no session is open */
    EXPECT_THROW(poTransaction->processPoCommandsInSession(),
                 CalypsoPoTransactionIllegalStateException);

    tearDown();
}

/* Standard processPoCommandsInSession */
TEST(PoTransactionTest, testProcessPoCommandsInSession_nominalCase)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev31),
                        poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP});
    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_SFI7_REC1_CMD,
                              PO_OPEN_SECURE_SESSION_SFI7_REC1_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI8_REC1_CMD,
                              PO_READ_REC_SFI8_REC1_RSP});

    poTransaction->prepareReadRecordFile(FILE7, 1);
    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);
    poTransaction->prepareReadRecordFile(FILE8, 1);
    /* PoTransaction after a session is open */
    poTransaction->processPoCommandsInSession();

    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE7)->getData()->getContent(1),
              FILE7_REC1_29B_BYTES);
    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE8)->getData()->getContent(1),
              FILE8_REC1_29B_BYTES);

    tearDown();
}

/* processClosing no session open */
TEST(PoTransactionTest, testProcessClosing_noSessionOpen)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(
                            poReader, calypsoPoRev31));

    poTransaction->prepareReadRecordFile(FILE8, 1);

    /* Expected exception: no session is open */
    EXPECT_THROW(poTransaction->processClosing(ChannelControl::CLOSE_AFTER),
                 CalypsoPoTransactionIllegalStateException);

    tearDown();
}

/* Standard processClosing - default ratification */
TEST(PoTransactionTest, testProcessClosing_nominalCase)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev31),
                        poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP});
    samCommandsTestSet.insert(
        {SAM_DIGEST_INIT_OPEN_SECURE_SESSION_SFI7_REC1_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI8_REC1_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_RSP_OK_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI7_REC1_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI10_REC1_RSP_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI10_REC1_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI11_REC1_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI11_REC1_RSP_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_INIT_OPEN_SECURE_SESSION_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_DECREASE_SFI10_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_DECREASE_SFI10_RESP,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_INCREASE_SFI11_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_INCREASE_SFI11_RESP,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_UPDATE_REC_SFI7_REC1_4B_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_UPDATE_REC_SFI8_REC1_4B_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_WRITE_REC_SFI8_REC1_4B_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_APPEND_REC_SFI9_REC1_4B_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI11_REC1_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_CLOSE_CMD, SAM_DIGEST_CLOSE_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_AUTHENTICATE, SW1SW2_OK_RSP});

    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_SFI7_REC1_CMD,
                              PO_OPEN_SECURE_SESSION_SFI7_REC1_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI10_REC1_CMD,
                              PO_READ_REC_SFI10_REC1_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI11_REC1_CMD,
                              PO_READ_REC_SFI11_REC1_RSP});
    poCommandsTestSet.insert({PO_DECREASE_SFI10_REC1_100U_CMD,
                              PO_DECREASE_SFI10_REC1_100U_RSP});
    poCommandsTestSet.insert({PO_DECREASE_SFI11_REC1_100U_CMD,
                              PO_DECREASE_SFI11_REC1_100U_RSP});
    poCommandsTestSet.insert({PO_UPDATE_REC_SFI7_REC1_4B_CMD, SW1SW2_OK_RSP});
    poCommandsTestSet.insert({PO_WRITE_REC_SFI8_REC1_4B_CMD, SW1SW2_OK_RSP});
    poCommandsTestSet.insert({PO_APPEND_REC_SFI9_REC1_4B_CMD, SW1SW2_OK_RSP});
    poCommandsTestSet.insert({PO_CLOSE_SECURE_SESSION_CMD,
                              PO_CLOSE_SECURE_SESSION_RSP});
    poCommandsTestSet.insert({PO_RATIFICATION_CMD, PO_RATIFICATION_RSP});

    poTransaction->prepareReadRecordFile(FILE7, 1);
    poTransaction->prepareReadRecordFile(FILE10, 1);
    poTransaction->prepareReadRecordFile(FILE11, 1);
    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

    poTransaction->prepareDecreaseCounter(FILE10, 1, 100);
    poTransaction->prepareIncreaseCounter(FILE11, 1, 100);
    poTransaction->prepareUpdateRecord(FILE7,
                                       1,
                                       ByteArrayUtil::fromHex(FILE7_REC1_4B));
    poTransaction->prepareWriteRecord(FILE8,
                                      1,
                                      ByteArrayUtil::fromHex(FILE8_REC1_4B));
    poTransaction->prepareAppendRecord(FILE9,
                                       ByteArrayUtil::fromHex(FILE9_REC1_4B));

    /* PoTransaction after a session is open */
    poTransaction->processClosing(ChannelControl::CLOSE_AFTER);

    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE10)->getData()
                  ->getContentAsCounterValue(1),
              0x1122 - 100);
    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE11)->getData()
                  ->getContentAsCounterValue(1),
              0x2211 + 100);
    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE7)->getData()->getContent(1),
              ByteArrayUtil::fromHex(FILE7_REC1_4B));
    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE8)->getData()->getContent(1),
              ByteArrayUtil::fromHex(FILE8_REC1_4B));
    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE9)->getData()->getContent(1),
              ByteArrayUtil::fromHex(FILE9_REC1_4B));

    tearDown();
}

/* processClosing - PO fail on closing */
TEST(PoTransactionTest, testProcessClosing_poCloseFail)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev31),
                        poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_INIT_OPEN_SECURE_SESSION_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI7_REC1_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_RSP_OK_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_CLOSE_CMD, SAM_DIGEST_CLOSE_RSP});

    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_CMD,
                              PO_OPEN_SECURE_SESSION_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI7_REC1_CMD,
                              PO_READ_REC_SFI7_REC1_RSP});
    poCommandsTestSet.insert({PO_CLOSE_SECURE_SESSION_CMD,
                              PO_CLOSE_SECURE_SESSION_FAILED_RSP});

    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);
    poTransaction->prepareReadRecordFile(FILE7, 1);

    /*
     * PoTransaction after a session is open
     * should raise a CalypsoPoCloseSecureSessionException
     */
    EXPECT_THROW(poTransaction->processClosing(ChannelControl::CLOSE_AFTER),
                 CalypsoPoCloseSecureSessionException);

    tearDown();
}

/* processClosing - SAM authentication fail on closing */
TEST(PoTransactionTest, testProcessClosing_samAuthenticateFail)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev31),
                        poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_GET_CHALLENGE_CMD,
                               SAM_GET_CHALLENGE_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_INIT_OPEN_SECURE_SESSION_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI7_REC1_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_RSP_OK_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_CLOSE_CMD,
                               SAM_DIGEST_CLOSE_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_AUTHENTICATE,
                               SAM_DIGEST_AUTHENTICATE_FAILED});

    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_CMD,
                              PO_OPEN_SECURE_SESSION_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI7_REC1_CMD,
                              PO_READ_REC_SFI7_REC1_RSP});
    poCommandsTestSet.insert({PO_CLOSE_SECURE_SESSION_CMD,
                              PO_CLOSE_SECURE_SESSION_RSP});

    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);
    poTransaction->prepareReadRecordFile(FILE7, 1);

    /*
     * PoTransaction after a session is open
     * Should raise a CalypsoSessionAuthenticationException
     */
    EXPECT_THROW(poTransaction->processClosing(ChannelControl::CLOSE_AFTER),
                 CalypsoSessionAuthenticationException);

    tearDown();
}


/* processClosing - SAM IO error while authenticating */
TEST(PoTransactionTest, testProcessClosing_samIoErrorAuthenticating)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev31),
                        poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_INIT_OPEN_SECURE_SESSION_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI7_REC1_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_RSP_OK_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_CLOSE_CMD, SAM_DIGEST_CLOSE_RSP});

    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_CMD,
                              PO_OPEN_SECURE_SESSION_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI7_REC1_CMD,
                              PO_READ_REC_SFI7_REC1_RSP});
    poCommandsTestSet.insert({PO_CLOSE_SECURE_SESSION_CMD,
                              PO_CLOSE_SECURE_SESSION_RSP});

    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);
    poTransaction->prepareReadRecordFile(FILE7, 1);

    /*
     * PoTransaction after a session is open
     * should raise a CalypsoAuthenticationNotVerifiedException
     */
    EXPECT_THROW(poTransaction->processClosing(ChannelControl::CLOSE_AFTER),
                 CalypsoAuthenticationNotVerifiedException);

    tearDown();
}

/*
 * Buffer overflow limit in atomic mode (counter in bytes): session buffer size = 430 b,
 * consumed size 430 b
 */
TEST(PoTransactionTest, testProcessClosing_sessionBuffer_limit)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev31),
                        poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_INIT_OPEN_SECURE_SESSION_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI8_REC1_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_RSP_OK_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI7_REC1_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_UPDATE_REC_SFI8_REC1_29B_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_UPDATE_REC_SFI8_REC1_4B_CMD,
                               SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_CLOSE_CMD, SAM_DIGEST_CLOSE_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_AUTHENTICATE, SW1SW2_OK_RSP});

    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_CMD,
                              PO_OPEN_SECURE_SESSION_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI7_REC1_CMD,
                              PO_READ_REC_SFI7_REC1_RSP});
    poCommandsTestSet.insert({PO_UPDATE_REC_SFI8_REC1_29B_CMD,
                              SW1SW2_OK_RSP});
    poCommandsTestSet.insert({PO_UPDATE_REC_SFI8_REC1_4B_CMD,
                              SW1SW2_OK_RSP});
    poCommandsTestSet.insert({PO_CLOSE_SECURE_SESSION_CMD,
                              PO_CLOSE_SECURE_SESSION_RSP});

    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

    /*
     * Add additional non modifying commands (should not affect the session
     * buffer)
     */
    for (int i = 0; i < 4; i++)
        poTransaction->prepareReadRecordFile(FILE7, 1);

    /*
     * 12 x update (29 b) = 12 x (29 + 6) = 420 consumed in the session buffer
     */
    for (int i = 0; i < 12; i++)
        poTransaction->prepareUpdateRecord(FILE8, 1, FILE8_REC1_29B_BYTES);

    /*
     * Insert additional non modifying commands (should not affect the session
     * buffer)
     */
    for (int i = 0; i < 4; i++)
        poTransaction->prepareReadRecordFile(FILE7, 1);

    /* 4 additional bytes (10 b consumed) */
    poTransaction->prepareUpdateRecord(FILE8, 1, FILE8_REC1_4B_BYTES);

    /* PoTransaction after a session is open */
    poTransaction->processClosing(ChannelControl::CLOSE_AFTER);

    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE8)->getData()->getContent(1),
              FILE8_REC1_4B_BYTES);

    tearDown();
}

// /*
//     * Buffer overflowed in atomic mode (counter in bytes): session buffer size = 430 b, consumed
//     * size 431 b
//     */
// @Test
// public void testProcessClosing_sessionBuffer_overflowed() {
//     CalypsoPo calypsoPoRev31 = createCalypsoPo(FCI_REV31);
//     PoSecuritySettings poSecuritySettings =
//             new PoSecuritySettings.PoSecuritySettingsBuilder(samResource) //
//                     .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT) //
//                     .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
//                             DEFAULT_KEY_RECORD_NUMBER_DEBIT)
//                     .build();

//     poTransaction =
//             new PoTransaction(new SeResource(poReader, calypsoPoRev31), poSecuritySettings);

//     samCommandsTestSet.insert(SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP);
//     samCommandsTestSet.insert(SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP);

//     poCommandsTestSet.insert(PO_OPEN_SECURE_SESSION_CMD, PO_OPEN_SECURE_SESSION_RSP);

//     poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

//     // add additional non modifying commands (should not affect the session buffer)
//     for (int i = 0; i < 4; i++) {
//         poTransaction->prepareReadRecordFile(FILE7, 1);
//     }
//     // 12 x update (29 b) = 12 x (29 + 6) = 420 consumed in the session buffer
//     for (int i = 0; i < 12; i++) {
//         poTransaction->prepareUpdateRecord(FILE8, static const uint8_t1, ByteArrayUtil
//                 .fromHex("8111111111111111111111111111111111111111111111111111111111"));
//     }
//     // insert additional non modifying commands (should not affect the session buffer)
//     for (int i = 0; i < 4; i++) {
//         poTransaction->prepareReadRecordFile(FILE7, 1);
//     }
//     // 4 additional bytes (10 b consumed)
//     poTransaction->prepareUpdateRecord(FILE8, static const uint8_t1, FILE8_REC1_5B_BYTES);

//     try {
//         // PoTransaction after a session is open
//         poTransaction->processClosing(ChannelControl::CLOSE_AFTER);
//     } catch (CalypsoAtomicTransactionException ex) {
//         // expected exception: session buffer overflow
//         return;
//     }
//     fail("Unexpected behaviour");
// }

// /*
//     * Buffer overflowed in multiple mode (counter in bytes): session buffer size = 430 b, consumed
//     * size 431 b
//     */
// @Test
// public void testProcessClosing_sessionBuffer_overflowMultipleMode() {
//     CalypsoPo calypsoPoRev31 = createCalypsoPo(FCI_REV31);
//     PoSecuritySettings poSecuritySettings =
//             new PoSecuritySettings.PoSecuritySettingsBuilder(samResource) //
//                     .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT) //
//                     .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
//                             DEFAULT_KEY_RECORD_NUMBER_DEBIT)//
//                     .sessionModificationMode(
//                             ModificationMode::MULTIPLE)//
//                     .build();

//     poTransaction =
//             new PoTransaction(new SeResource(poReader, calypsoPoRev31), poSecuritySettings);

//     samCommandsTestSet.insert(SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP);
//     samCommandsTestSet.insert(SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP);
//     samCommandsTestSet.insert(SAM_DIGEST_INIT_OPEN_SECURE_SESSION_CMD, SW1SW2_OK_RSP);
//     samCommandsTestSet.insert(SAM_DIGEST_UPDATE_READ_REC_SFI8_REC1_CMD, SW1SW2_OK_RSP);
//     samCommandsTestSet.insert(SAM_DIGEST_UPDATE_RSP_OK_CMD, SW1SW2_OK_RSP);
//     samCommandsTestSet.insert(SAM_DIGEST_UPDATE_READ_REC_SFI7_REC1_CMD, SW1SW2_OK_RSP);
//     samCommandsTestSet.insert(SAM_DIGEST_UPDATE_UPDATE_REC_SFI8_REC1_29B_CMD, SW1SW2_OK_RSP);
//     samCommandsTestSet.insert(SAM_DIGEST_UPDATE_UPDATE_REC_SFI8_REC1_4B_CMD, SW1SW2_OK_RSP);
//     samCommandsTestSet.insert(SAM_DIGEST_UPDATE_READ_REC_SFI7_REC1_RSP, SW1SW2_OK_RSP);
//     samCommandsTestSet.insert(SAM_DIGEST_UPDATE_UPDATE_REC_SFI7_REC1_4B_CMD, SW1SW2_OK_RSP);

//     samCommandsTestSet.insert(SAM_DIGEST_CLOSE_CMD, SAM_DIGEST_CLOSE_RSP);
//     samCommandsTestSet.insert(SAM_DIGEST_AUTHENTICATE, SW1SW2_OK_RSP);

//     poCommandsTestSet.insert(PO_OPEN_SECURE_SESSION_CMD, PO_OPEN_SECURE_SESSION_RSP);
//     poCommandsTestSet.insert(PO_READ_REC_SFI7_REC1_CMD, PO_READ_REC_SFI7_REC1_RSP);
//     poCommandsTestSet.insert(PO_UPDATE_REC_SFI8_REC1_29B_CMD, SW1SW2_OK_RSP);
//     poCommandsTestSet.insert(PO_UPDATE_REC_SFI8_REC1_4B_CMD, SW1SW2_OK_RSP);
//     poCommandsTestSet.insert(PO_CLOSE_SECURE_SESSION_CMD, PO_CLOSE_SECURE_SESSION_RSP);

//     poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

//     // add additional non modifying commands (should not affect the session buffer)
//     for (int i = 0; i < 4; i++) {
//         poTransaction->prepareReadRecordFile(FILE7, 1);
//     }
//     // 12 x update (29 b) = 12 x (29 + 6) = 420 consumed in the session buffer
//     for (int i = 0; i < 12; i++) {
//         poTransaction->prepareUpdateRecord(FILE8, static const uint8_t1, FILE8_REC1_29B_BYTES);
//     }
//     // insert additional non modifying commands (should not affect the session buffer)
//     for (int i = 0; i < 4; i++) {
//         poTransaction->prepareReadRecordFile(FILE7, 1);
//     }
//     // 4 additional bytes (10 b consumed)
//     poTransaction->prepareUpdateRecord(FILE8, static const uint8_t1, FILE8_REC1_4B_BYTES);

//     // PoTransaction after a session is open
//     poTransaction->processClosing(ChannelControl::CLOSE_AFTER);

//     assertThat(true).isTrue();
// }

// /* Standard processClosing - close not ratified */
// @Test
// public void testProcessClosing_nominalCase_closeNotRatified() {
//     CalypsoPo calypsoPoRev31 = createCalypsoPo(FCI_REV31);
//     PoSecuritySettings poSecuritySettings =
//             new PoSecuritySettings.PoSecuritySettingsBuilder(samResource) //
//                     .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT) //
//                     .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
//                             DEFAULT_KEY_RECORD_NUMBER_DEBIT)//
//                     .ratificationMode(
//                             PoTransaction.SessionSetting.RatificationMode.CLOSE_NOT_RATIFIED)//
//                     .build();

//     poTransaction =
//             new PoTransaction(new SeResource(poReader, calypsoPoRev31), poSecuritySettings);

//     samCommandsTestSet.insert(SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP);
//     samCommandsTestSet.insert(SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP);
//     samCommandsTestSet.insert(SAM_DIGEST_INIT_OPEN_SECURE_SESSION_SFI7_REC1_CMD, SW1SW2_OK_RSP);
//     samCommandsTestSet.insert(SAM_DIGEST_CLOSE_CMD, SAM_DIGEST_CLOSE_RSP);
//     samCommandsTestSet.insert(SAM_DIGEST_AUTHENTICATE, SW1SW2_OK_RSP);

//     poCommandsTestSet.insert(PO_OPEN_SECURE_SESSION_SFI7_REC1_CMD,
//             PO_OPEN_SECURE_SESSION_SFI7_REC1_RSP);
//     poCommandsTestSet.insert(PO_CLOSE_SECURE_SESSION_NOT_RATIFIED_CMD,
//             PO_CLOSE_SECURE_SESSION_RSP);

//     poTransaction->prepareReadRecordFile(FILE7, 1);
//     poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

//     poTransaction->processClosing(ChannelControl::CLOSE_AFTER);

//     assertThat(true).isTrue();
// }

// /* Session buffer overflow in atomic mode: the overflow happens at closing */
// @Test
// public void testTransaction_sessionBuffer_overflowAtomic() {
//     CalypsoPo calypsoPoRev31 = createCalypsoPo(FCI_REV31);
//     PoSecuritySettings poSecuritySettings =
//             new PoSecuritySettings.PoSecuritySettingsBuilder(samResource) //
//                     .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT) //
//                     .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
//                             DEFAULT_KEY_RECORD_NUMBER_DEBIT)//
//                     .build();

//     poTransaction =
//             new PoTransaction(new SeResource(poReader, calypsoPoRev31), poSecuritySettings);

//     samCommandsTestSet.insert(SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP);
//     samCommandsTestSet.insert(SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP);

//     poCommandsTestSet.insert(PO_OPEN_SECURE_SESSION_CMD, PO_OPEN_SECURE_SESSION_RSP);
//     poCommandsTestSet.insert(PO_READ_REC_SFI8_REC1_CMD, PO_READ_REC_SFI8_REC1_RSP);
//     poCommandsTestSet.insert(PO_UPDATE_REC_SFI8_REC1_29B_CMD, SW1SW2_OK_RSP);
//     poCommandsTestSet.insert(PO_CLOSE_SECURE_SESSION_CMD, PO_CLOSE_SECURE_SESSION_RSP);

//     // 4 x update (29 b) = 4 x (29 + 6) = 140 consumed in the session buffer
//     for (int i = 0; i < 4; i++) {
//         poTransaction->prepareUpdateRecord(FILE8, static const uint8_t1, FILE8_REC1_29B_BYTES);
//     }
//     poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

//     // add additional non modifying commands (should not affect the session buffer)
//     for (int i = 0; i < 4; i++) {
//         poTransaction->prepareReadRecordFile(FILE8, 1);
//     }
//     // 4 x update (29 b) = 4 x (29 + 6) = 140 consumed in the session buffer
//     for (int i = 0; i < 4; i++) {
//         poTransaction->prepareUpdateRecord(FILE8, static const uint8_t1, FILE8_REC1_29B_BYTES);
//     }
//     // insert additional non modifying commands (should not affect the session buffer)
//     for (int i = 0; i < 4; i++) {
//         poTransaction->prepareReadRecordFile(FILE8, 1);
//     }
//     poTransaction->processPoCommandsInSession();

//     // 5 x update (29 b) = 5 x (29 + 6) = 140 consumed in the session buffer
//     for (int i = 0; i < 4; i++) {
//         poTransaction->prepareUpdateRecord(FILE8, static const uint8_t1, FILE8_REC1_29B_BYTES);
//     }
//     // 4 additional bytes (10 b consumed)
//     poTransaction->prepareUpdateRecord(FILE8, static const uint8_t1, FILE8_REC1_5B_BYTES);

//     try {
//         // PoTransaction after a session is open
//         poTransaction->processClosing(ChannelControl::CLOSE_AFTER);
//     } catch (CalypsoAtomicTransactionException ex) {
//         // expected exception: buffer overflow
//         return;
//     }
//     fail("Unexpected behaviour");
// }

// /* Session buffer overflow in multiple mode: the overflow happens and is handled at closing */
// @Test
// public void testTransaction_sessionBuffer_overflowMultiple() {
//     CalypsoPo calypsoPoRev31 = createCalypsoPo(FCI_REV31);
//     PoSecuritySettings poSecuritySettings =
//             new PoSecuritySettings.PoSecuritySettingsBuilder(samResource) //
//                     .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT) //
//                     .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
//                             DEFAULT_KEY_RECORD_NUMBER_DEBIT)//
//                     .sessionModificationMode(
//                             ModificationMode::MULTIPLE)//
//                     .build();

//     poTransaction =
//             new PoTransaction(new SeResource(poReader, calypsoPoRev31), poSecuritySettings);

//     samCommandsTestSet.insert(SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP);
//     samCommandsTestSet.insert(SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP);
//     samCommandsTestSet.insert(SAM_DIGEST_INIT_OPEN_SECURE_SESSION_CMD, SW1SW2_OK_RSP);
//     samCommandsTestSet.insert(SAM_DIGEST_UPDATE_READ_REC_SFI8_REC1_CMD, SW1SW2_OK_RSP);
//     samCommandsTestSet.insert(SAM_DIGEST_UPDATE_RSP_OK_CMD, SW1SW2_OK_RSP);
//     samCommandsTestSet.insert(SAM_DIGEST_UPDATE_UPDATE_REC_SFI8_REC1_29B_CMD, SW1SW2_OK_RSP);
//     samCommandsTestSet.insert(SAM_DIGEST_UPDATE_READ_REC_SFI7_REC1_RSP_CMD, SW1SW2_OK_RSP);
//     samCommandsTestSet.insert(SAM_DIGEST_UPDATE_READ_REC_SFI8_REC1_RSP_CMD, SW1SW2_OK_RSP);
//     samCommandsTestSet.insert(SAM_DIGEST_UPDATE_UPDATE_REC_SFI8_REC1_5B_CMD, SW1SW2_OK_RSP);
//     samCommandsTestSet.insert(SAM_DIGEST_CLOSE_CMD, SAM_DIGEST_CLOSE_RSP);
//     samCommandsTestSet.insert(SAM_DIGEST_AUTHENTICATE, SW1SW2_OK_RSP);

//     poCommandsTestSet.insert(PO_OPEN_SECURE_SESSION_CMD, PO_OPEN_SECURE_SESSION_RSP);
//     poCommandsTestSet.insert(PO_READ_REC_SFI7_REC1_CMD, PO_READ_REC_SFI7_REC1_RSP);
//     poCommandsTestSet.insert(PO_READ_REC_SFI8_REC1_CMD, PO_READ_REC_SFI8_REC1_RSP);
//     poCommandsTestSet.insert(PO_UPDATE_REC_SFI8_REC1_29B_CMD, SW1SW2_OK_RSP);
//     poCommandsTestSet.insert(PO_UPDATE_REC_SFI8_REC1_5B_CMD, SW1SW2_OK_RSP);
//     poCommandsTestSet.insert(PO_CLOSE_SECURE_SESSION_CMD, PO_CLOSE_SECURE_SESSION_RSP);

//     // 4 x update (29 b) = 4 x (29 + 6) = 140 consumed in the session buffer
//     for (int i = 0; i < 4; i++) {
//         poTransaction->prepareUpdateRecord(FILE8, static const uint8_t1, FILE8_REC1_29B_BYTES);
//     }
//     poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

//     // add additional non modifying commands (should not affect the session buffer)
//     for (int i = 0; i < 4; i++) {
//         poTransaction->prepareReadRecordFile(FILE8, 1);
//     }
//     // 24 x update (29 b) = 24 x (29 + 6) = 840 consumed in the session buffer
//     // force multiple cycles
//     for (int i = 0; i < 24; i++) {
//         poTransaction->prepareUpdateRecord(FILE8, static const uint8_t1, FILE8_REC1_29B_BYTES);
//     }
//     // insert additional non modifying commands (should not affect the session buffer)
//     for (int i = 0; i < 4; i++) {
//         poTransaction->prepareReadRecordFile(FILE8, 1);
//     }
//     poTransaction->processPoCommandsInSession();

//     // 24 x update (29 b) = 24 x (29 + 6) = 840 consumed in the session buffer
//     // force multiple cycles
//     for (int i = 0; i < 24; i++) {
//         poTransaction->prepareUpdateRecord(FILE8, static const uint8_t1, FILE8_REC1_29B_BYTES);
//     }
//     // 4 additional bytes (10 b consumed)
//     poTransaction->prepareUpdateRecord(FILE8, static const uint8_t1, FILE8_REC1_5B_BYTES);

//     // PoTransaction after a session is open
//     poTransaction->processClosing(ChannelControl::CLOSE_AFTER);

//     assertThat(true).isTrue();
// }

// /* open, cancel and reopen */
// @Test
// public void testProcessCancel_open_cancelOpen() {
//     CalypsoPo calypsoPoRev31 = createCalypsoPo(FCI_REV31);
//     PoSecuritySettings poSecuritySettings =
//             new PoSecuritySettings.PoSecuritySettingsBuilder(samResource)//
//                     .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT)//
//                     .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
//                             DEFAULT_KEY_RECORD_NUMBER_DEBIT)
//                     .build();

//     poTransaction =
//             new PoTransaction(new SeResource(poReader, calypsoPoRev31), poSecuritySettings);
//     samCommandsTestSet.insert(SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP);
//     samCommandsTestSet.insert(SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP);

//     poCommandsTestSet.insert(PO_OPEN_SECURE_SESSION_SFI7_REC1_CMD,
//             PO_OPEN_SECURE_SESSION_SFI7_REC1_RSP);
//     poCommandsTestSet.insert(PO_OPEN_SECURE_SESSION_CMD, PO_OPEN_SECURE_SESSION_RSP);
//     poCommandsTestSet.insert(PO_READ_REC_SFI8_REC1_CMD, PO_READ_REC_SFI8_REC1_RSP);
//     // Abort session
//     poCommandsTestSet.insert(PO_ABORT_SECURE_SESSION_CMD, SW1SW2_OK_RSP);

//     poTransaction->prepareReadRecordFile(FILE7, 1);
//     poTransaction->prepareReadRecordFile(FILE8, 1);
//     poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);
//     assertThat(calypsoPoRev31->getFileBySfi(FILE7)->getData()->getContent(),
//             .isEqualTo(FILE7_REC1_29B_BYTES);
//     assertThat(calypsoPoRev31->getFileBySfi(FILE8)->getData()->getContent(),
//             .isEqualTo(FILE8_REC1_29B_BYTES);
//     assertThat(calypsoPoRev31->isDfRatified()).isTrue();
//     poTransaction->processCancel(ChannelControl::KEEP_OPEN);
//     poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);
// }

// @Test
// public void testPrepareSelectFile_selectControl() {
//     CalypsoPo calypsoPoRev31 = createCalypsoPo(FCI_REV31);
//     poTransaction = new PoTransaction(new SeResource(poReader, calypsoPoRev31));

//     poCommandsTestSet.insert(PO_SELECT_FILE_CURRENT_CMD, PO_SELECT_FILE_3F00_RSP);
//     poCommandsTestSet.insert(PO_SELECT_FILE_FIRST_CMD, PO_SELECT_FILE_0002_RSP);
//     poCommandsTestSet.insert(PO_SELECT_FILE_NEXT_CMD, PO_SELECT_FILE_0003_RSP);

//     poTransaction->prepareSelectFile(SelectFileControl.CURRENT_DF);
//     poTransaction->prepareSelectFile(SelectFileControl.FIRST_EF);
//     poTransaction->prepareSelectFile(SelectFileControl.NEXT_EF);
//     poTransaction->processPoCommands(ChannelControl::KEEP_OPEN);
//     DirectoryHeader directoryHeader = calypsoPoRev31->getDirectoryHeader();
//     FileHeader fileHeader1 = calypsoPoRev31->getFileByLid((short) 0x02).getHeader();
//     FileHeader fileHeader2 = calypsoPoRev31->getFileByLid((short) 0x03).getHeader();
//     System.out.println(directoryHeader);
//     System.out.println(fileHeader1);
//     System.out.println(fileHeader2);

//     assertThat(directoryHeader.getLid()).isEqualTo(LID_3F00);
//     assertThat(directoryHeader.getAccessConditions())
//             .isEqualTo(ByteArrayUtil::fromHex(ACCESS_CONDITIONS_3F00));
//     assertThat(directoryHeader.getKeyIndexes())
//             .isEqualTo(ByteArrayUtil::fromHex(KEY_INDEXES_3F00));
//     assertThat(directoryHeader.getDfStatus()).isEqualTo(0x00);
//     assertThat(directoryHeader.getKif(AccessLevel::SESSION_LVL_PERSO)).isEqualTo(0x61);
//     assertThat(directoryHeader.getKif(AccessLevel::SESSION_LVL_LOAD)).isEqualTo(0x67);
//     assertThat(directoryHeader.getKif(AccessLevel::SESSION_LVL_DEBIT)).isEqualTo(0x70);
//     assertThat(directoryHeader.getKvc(AccessLevel::SESSION_LVL_PERSO)).isEqualTo(0x77);
//     assertThat(directoryHeader.getKvc(AccessLevel::SESSION_LVL_LOAD)).isEqualTo(0x78);
//     assertThat(directoryHeader.getKvc(AccessLevel::SESSION_LVL_DEBIT)).isEqualTo(0x79);

//     assertThat(fileHeader1.getLid()).isEqualTo(LID_0002);
//     assertThat(fileHeader1.getRecordsNumber()).isEqualTo(1);
//     assertThat(fileHeader1.getRecordSize()).isEqualTo(29);
//     assertThat(fileHeader1.getType()).isEqualTo(FileHeader.FileType.LINEAR);
//     assertThat(fileHeader1.getAccessConditions())
//             .isEqualTo(ByteArrayUtil::fromHex(ACCESS_CONDITIONS_0002));
//     assertThat(fileHeader1.getKeyIndexes()).isEqualTo(ByteArrayUtil::fromHex(KEY_INDEXES_0002));
//     assertThat(fileHeader1.getDfStatus()).isEqualTo(0x00);
//     assertThat(fileHeader1.getSharedReference()).isEqualTo(Short.valueOf((short) 0x3F02));

//     assertThat(fileHeader2.getLid()).isEqualTo(LID_0003);
//     assertThat(fileHeader2.getRecordsNumber()).isEqualTo(1);
//     assertThat(fileHeader2.getRecordSize()).isEqualTo(29);
//     assertThat(fileHeader2.getType()).isEqualTo(FileHeader.FileType.LINEAR);
//     assertThat(fileHeader2.getAccessConditions())
//             .isEqualTo(ByteArrayUtil::fromHex(ACCESS_CONDITIONS_0003));
//     assertThat(fileHeader2.getKeyIndexes()).isEqualTo(ByteArrayUtil::fromHex(KEY_INDEXES_0003));
//     assertThat(fileHeader2.getDfStatus()).isEqualTo(0x00);
//     assertThat(fileHeader2.getSharedReference()).isEqualTo(Short.valueOf((short) 0x3F03));
// }

// @Test
// public void testPrepareSelectFile_lid() {
//     CalypsoPo calypsoPoRev31 = createCalypsoPo(FCI_REV31);
//     poTransaction = new PoTransaction(new SeResource(poReader, calypsoPoRev31));

//     poCommandsTestSet.insert(PO_SELECT_FILE_3F00_CMD, PO_SELECT_FILE_3F00_RSP);
//     poCommandsTestSet.insert(PO_SELECT_FILE_0002_CMD, PO_SELECT_FILE_0002_RSP);
//     poCommandsTestSet.insert(PO_SELECT_FILE_0003_CMD, PO_SELECT_FILE_0003_RSP);

//     poTransaction->prepareSelectFile(ByteArrayUtil::fromHex(LID_3F00_STR));
//     poTransaction->prepareSelectFile(ByteArrayUtil::fromHex(LID_0002_STR));
//     poTransaction->prepareSelectFile(ByteArrayUtil::fromHex(LID_0003_STR));
//     poTransaction->processPoCommands(ChannelControl::KEEP_OPEN);

//     DirectoryHeader directoryHeader = calypsoPoRev31->getDirectoryHeader();
//     ElementaryFile file1 = calypsoPoRev31->getFileByLid((short) 0x02);
//     ElementaryFile file2 = calypsoPoRev31->getFileByLid((short) 0x03);
//     byte sfi1 = file1.getSfi();
//     byte sfi2 = file2.getSfi();
//     System.out.println(directoryHeader);
//     System.out.println(file1);
//     System.out.println(file2);

//     assertThat(calypsoPoRev31->getFileBySfi(sfi1)).isEqualTo(file1);
//     assertThat(calypsoPoRev31->getFileBySfi(sfi2)).isEqualTo(file2);

//     assertThat(directoryHeader.getLid()).isEqualTo(LID_3F00);
//     assertThat(directoryHeader.getAccessConditions())
//             .isEqualTo(ByteArrayUtil::fromHex(ACCESS_CONDITIONS_3F00));
//     assertThat(directoryHeader.getKeyIndexes())
//             .isEqualTo(ByteArrayUtil::fromHex(KEY_INDEXES_3F00));
//     assertThat(directoryHeader.getDfStatus()).isEqualTo(0x00);
//     assertThat(directoryHeader.getKif(AccessLevel::SESSION_LVL_PERSO)).isEqualTo(0x61);
//     assertThat(directoryHeader.getKif(AccessLevel::SESSION_LVL_LOAD)).isEqualTo(0x67);
//     assertThat(directoryHeader.getKif(AccessLevel::SESSION_LVL_DEBIT)).isEqualTo(0x70);
//     assertThat(directoryHeader.getKvc(AccessLevel::SESSION_LVL_PERSO)).isEqualTo(0x77);
//     assertThat(directoryHeader.getKvc(AccessLevel::SESSION_LVL_LOAD)).isEqualTo(0x78);
//     assertThat(directoryHeader.getKvc(AccessLevel::SESSION_LVL_DEBIT)).isEqualTo(0x79);

//     FileHeader fileHeader1 = file1.getHeader();
//     assertThat(fileHeader1.getLid()).isEqualTo(LID_0002);
//     assertThat(fileHeader1.getRecordsNumber()).isEqualTo(1);
//     assertThat(fileHeader1.getRecordSize()).isEqualTo(29);
//     assertThat(fileHeader1.getType()).isEqualTo(FileHeader.FileType.LINEAR);
//     assertThat(fileHeader1.getAccessConditions())
//             .isEqualTo(ByteArrayUtil::fromHex(ACCESS_CONDITIONS_0002));
//     assertThat(fileHeader1.getKeyIndexes()).isEqualTo(ByteArrayUtil::fromHex(KEY_INDEXES_0002));
//     assertThat(fileHeader1.getDfStatus()).isEqualTo(0x00);
//     assertThat(fileHeader1.getSharedReference()).isEqualTo(Short.valueOf((short) 0x3F02));

//     FileHeader fileHeader2 = file2.getHeader();
//     assertThat(fileHeader2.getLid()).isEqualTo(LID_0003);
//     assertThat(fileHeader2.getRecordsNumber()).isEqualTo(1);
//     assertThat(fileHeader2.getRecordSize()).isEqualTo(29);
//     assertThat(fileHeader2.getType()).isEqualTo(FileHeader.FileType.LINEAR);
//     assertThat(fileHeader2.getAccessConditions())
//             .isEqualTo(ByteArrayUtil::fromHex(ACCESS_CONDITIONS_0003));
//     assertThat(fileHeader2.getKeyIndexes()).isEqualTo(ByteArrayUtil::fromHex(KEY_INDEXES_0003));
//     assertThat(fileHeader2.getDfStatus()).isEqualTo(0x00);
//     assertThat(fileHeader2.getSharedReference()).isEqualTo(Short.valueOf((short) 0x3F03));
// }

// @Test
// public void testPrepareReadCounterFile() {
//     CalypsoPo calypsoPoRev31 = createCalypsoPo(FCI_REV31);
//     poTransaction = new PoTransaction(new SeResource(poReader, calypsoPoRev31));

//     poCommandsTestSet.insert(PO_READ_REC_SFI7_REC1_6B_COUNTER_CMD,
//             PO_READ_REC_SFI7_REC1_6B_COUNTER_RSP);

//     poTransaction->prepareReadCounterFile(FILE7, 2);
//     poTransaction->processPoCommands(ChannelControl::KEEP_OPEN);
//     assertThat(calypsoPoRev31->getFileBySfi(FILE7)->getData()->getContentAsCounterValue(1))
//             .isEqualTo(ByteArrayUtil::threeBytesToInt(ByteArrayUtil::fromHex(FILE7_REC1_COUNTER1),
//                     0));
//     assertThat(calypsoPoRev31->getFileBySfi(FILE7)->getData()->getContentAsCounterValue(2))
//             .isEqualTo(ByteArrayUtil::threeBytesToInt(ByteArrayUtil::fromHex(FILE7_REC1_COUNTER2),
//                     0));
// }

// @Test(expected = CalypsoPoIOException.class)
// public void testPoIoException() {
//     CalypsoPo calypsoPoRev31 = createCalypsoPo(FCI_REV31);
//     poTransaction = new PoTransaction(new SeResource(poReader, calypsoPoRev31));
//     poTransaction->prepareReadRecordFile(FILE7, 1);
//     poTransaction->processPoCommands(ChannelControl::KEEP_OPEN);
// }

// @Test
// public void testAccessLevel() {
//     assertThat(AccessLevel::SESSION_LVL_PERSO.getName()).isEqualTo("perso");
//     assertThat(AccessLevel::SESSION_LVL_LOAD.getName()).isEqualTo("load");
//     assertThat(AccessLevel::SESSION_LVL_DEBIT.getName()).isEqualTo("debit");
//     assertThat(AccessLevel::SESSION_LVL_PERSO.getSessionKey()).isEqualTo(0x01);
//     assertThat(AccessLevel::SESSION_LVL_LOAD.getSessionKey()).isEqualTo(0x02);
//     assertThat(AccessLevel::SESSION_LVL_DEBIT.getSessionKey()).isEqualTo(0x03);
// }
