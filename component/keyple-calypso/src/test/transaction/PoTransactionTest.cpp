/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "PoTransaction.h"

/* Calypso */
#include "CalypsoAtomicTransactionException.h"
#include "CalypsoAuthenticationNotVerifiedException.h"
#include "CalypsoPo.h"
#include "CalypsoPoIOException.h"
#include "CalypsoPoTransactionIllegalStateException.h"
#include "CalypsoSam.h"
#include "CalypsoUnauthorizedKvcException.h"
#include "CalypsoPoCloseSecureSessionException.h"
#include "CalypsoSessionAuthenticationException.h"
#include "KeypleSeCommandException.h"
#include "PoSecuritySettings.h"
#include "PoTransaction.h"

/* Core */
#include "ByteArrayUtil.h"
#include "KeypleReaderIOException.h"
#include "ProxyReader.h"
#include "TransmissionMode.h"

/* Common */
#include "IllegalStateException.h"
#include "NoSuchElementException.h"

using namespace testing;

using namespace keyple::calypso;
using namespace keyple::calypso::transaction;
using namespace keyple::calypso::transaction::exception;
using namespace keyple::common::exception;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;

using LogRead = PoTransaction::SvSettings::LogRead;
using ModificationMode = PoTransaction::SessionSetting::ModificationMode;
using Operation = PoTransaction::SvSettings::Operation;
using PoSecuritySettingsBuilder = PoSecuritySettings::PoSecuritySettingsBuilder;

/* The default KIF values for personalization, loading and debiting */
//static const uint8_t DEFAULT_KIF_PERSO = 0x21;
//static const uint8_t DEFAULT_KIF_LOAD  = 0x27;
static const uint8_t DEFAULT_KIF_DEBIT = 0x30;

/*
 * The default key record number values for personalization, loading and
 * debiting. The actual value should be adjusted.
 */
//static const uint8_t DEFAULT_KEY_RECORD_NUMBER_PERSO = 0x01;
//static const uint8_t DEFAULT_KEY_RECORD_NUMBER_LOAD  = 0x02;
static const uint8_t DEFAULT_KEY_RECORD_NUMBER_DEBIT = 0x03;

static std::shared_ptr<SeReader> poReader;
static std::shared_ptr<PoTransaction> poTransaction;
static std::shared_ptr<SeResource<CalypsoSam>> samResource;

static std::map<std::string, std::string> poCommandsTestSet;
static std::map<std::string, std::string> samCommandsTestSet;

static const std::string FCI_REV10 =
   "6F228408315449432E494341A516BF0C13C708   0000000011223344 5307060A01032003119000";
static const std::string FCI_REV24 =
   "6F2A8410A0000004040125090101000000000000A516BF0C13C708 0000000011223344 53070A2E11420001019000";
static const std::string FCI_REV31 =
   "6F238409315449432E49434131A516BF0C13C708 0000000011223344 53070A3C23121410019000";
static const std::string FCI_STORED_VALUE_REV31 =
   "6F238409315449432E49434131A516BF0C13C708 0000000011223344 53070A3C23201410019000";
static const std::string FCI_REV31_INVALIDATED =
   "6F238409315449432E49434131A516BF0C13C708 0000000011223344 53070A3C23121410016283";


static const std::string ATR1 = "3B3F9600805A0080C120000012345678829000";

static const std::string PIN_OK = "0000";
static const std::string CIPHER_PIN_OK = "1122334455667788";
static const std::string PIN_KO = "0000";

static const uint8_t FILE7  = 0x07;
static const uint8_t FILE8  = 0x08;
static const uint8_t FILE9  = 0x09;
static const uint8_t FILE10 = 0x10;
static const uint8_t FILE11 = 0x11;

static const std::string SW1SW2_OK      = "9000";
static const std::string SW1SW2_KO = "6700";
static const std::string SAM_CHALLENGE  = "C1C2C3C4";
static const std::string PO_CHALLENGE = "C1C2C3C4C5C6C7C8";
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


static const std::string PO_VERIFY_PIN_PLAIN_OK_CMD =
    "0020000004" + ByteArrayUtil::toHex(std::vector<uint8_t>{0x00, 0x00, 0x00, 0x00});
static const std::string PO_VERIFY_PIN_ENCRYPTED_OK_CMD = "0020000008" + CIPHER_PIN_OK;
static const std::string PO_CHECK_PIN_CMD = "0020000000";
static const std::string PO_VERIFY_PIN_OK_RSP = "9000";
static const std::string PO_VERIFY_PIN_KO_RSP = "63C2";

static const int SV_BALANCE = 0x123456;
static const std::string SV_BALANCE_STR = "123456";
static const std::string PO_SV_GET_DEBIT_CMD = "007C000900";
static const std::string PO_SV_GET_DEBIT_RSP = "790073A54BC97DFA" + SV_BALANCE_STR
    + "FFFE0000000079123456780000DD0000160072" + SW1SW2_OK;
static const std::string PO_SV_GET_RELOAD_CMD = "007C000700";
static const std::string PO_SV_GET_RELOAD_RSP = "79007221D35F0E36" + SV_BALANCE_STR
    + "000000790000001A0000020000123456780000DB0070" + SW1SW2_OK;
static const std::string PO_SV_RELOAD_CMD =
    "00B89591171600000079000000020000123456780000DE2C8CB3D280";
static const std::string PO_SV_RELOAD_RSP = "A54BC9" + SW1SW2_OK;
static const std::string PO_SV_DEBIT_CMD =
    "00BACD001434FFFE0000000079123456780000DF0C9437AABB";
static const std::string PO_SV_DEBIT_RSP = "A54BC9" + SW1SW2_OK;
static const std::string PO_SV_UNDEBIT_CMD =
    "00BCCD00143400020000000079123456780000DF0C9437AABB";
static const std::string PO_SV_UNDEBIT_RSP = "A54BC9" + SW1SW2_OK;
static const std::string PO_READ_SV_LOAD_LOG_FILE_CMD = "00B201A400";
static const std::string PO_READ_SV_LOAD_LOG_FILE_RSP =
    "000000780000001A0000020000AABBCCDD0000DB007000000000000000" + SW1SW2_OK;
static const std::string PO_READ_SV_DEBIT_LOG_FILE_CMD = "00B201AD5D";
static const std::string PO_READ_SV_DEBIT_LOG_FILE_RSP =
    std::string("011DFFFE0000000079AABBCC010000DA000018006F00000000000000000000") +
    "021DFFFE0000000079AABBCC020000DA000018006F00000000000000000000" +
    "031DFFFE0000000079AABBCC030000DA000018006F00000000000000000000" +
    SW1SW2_OK;

static const std::string PO_INVALIDATE_CMD = "0004000000";
static const std::string PO_REHABILITATE_CMD = "0044000000";

static const std::string PO_GET_CHALLENGE_CMD = "0084000008";
static const std::string PO_GET_CHALLENGE_RSP = PO_CHALLENGE + SW1SW2_OK;

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
    "808C000024001122000000000000000000000000000000000000000000000000000000000000009000";
static const std::string SAM_DIGEST_UPDATE_READ_REC_SFI11_REC1_CMD =
    "808C00000500B2018400";
static const std::string SAM_DIGEST_UPDATE_READ_REC_SFI11_REC1_RSP_CMD =
    "808C000024002211000000000000000000000000000000000000000000000000000000000000009000";
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

static const std::string SAM_CARD_CIPHER_PIN_CMD =
    "801280FF060000" + ByteArrayUtil::toHex(std::vector<uint8_t>{0x00, 0x00, 0x00, 0x00});
static const std::string SAM_CARD_CIPHER_PIN_RSP = CIPHER_PIN_OK + SW1SW2_OK;
static const std::string SAM_GIVE_RANDOM_CMD = "8086000008" + PO_CHALLENGE;
static const std::string SAM_GIVE_RANDOM_RSP = SW1SW2_OK;
static const std::string SAM_PREPARE_LOAD_CMD = "805601FF367C00070079007221D35F0E36"
    + SV_BALANCE_STR
    + "000000790000001A0000020000123456780000DB00709000B80000170000000079000000020000";
static const std::string SAM_PREPARE_LOAD_RSP = "9591160000DE2C8CB3D280" + SW1SW2_OK;
static const std::string SAM_PREPARE_DEBIT_CMD = "805401FF307C000900790073A54BC97DFA"
    + SV_BALANCE_STR + "FFFE0000000079123456780000DD00001600729000BA00001400FFFE0000000079";
static const std::string SAM_PREPARE_DEBIT_RSP = "CD00340000DF0C9437AABB" + SW1SW2_OK;
static const std::string SAM_PREPARE_UNDEBIT_CMD = "805C01FF307C000900790073A54BC97DFA"
    + SV_BALANCE_STR + "FFFE0000000079123456780000DD00001600729000BC0000140000020000000079";
static const std::string SAM_PREPARE_UNDEBIT_RSP = "CD00340000DF0C9437AABB" + SW1SW2_OK;
static const std::string SAM_SV_CHECK_CMD = "8058000003A54BC9";


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
                (std::shared_ptr<SeRequest>, const ChannelControl&),
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
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT,
                               DEFAULT_KIF_DEBIT)
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
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT,
                               DEFAULT_KIF_DEBIT)
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
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT,
                               DEFAULT_KIF_DEBIT)
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
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT,
                               DEFAULT_KIF_DEBIT)
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
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT,
                               DEFAULT_KIF_DEBIT)
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
 * Buffer overflowed in atomic mode (counter in bytes): session buffer size =
 * 430 b, consumed size 431 b
 */
TEST(PoTransactionTest, testProcessOpening_sessionBuffer_overflowBytesCounter)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT,
                               DEFAULT_KIF_DEBIT)
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
 * Buffer overflow limit in atomic mode (counter in operations): session buffer
 * size = 6 op, consumed 6 op
 */
TEST(PoTransactionTest, testProcessOpening_sessionBuffer_limitOperationsCounter)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev24 = createCalypsoPo(FCI_REV24);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT,
                               DEFAULT_KIF_DEBIT)
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
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT,
                               DEFAULT_KIF_DEBIT)
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
 * Buffer overflowed in multiple mode (counter in bytes): session buffer size = 430 b, consumed size
 * 431 b
 */
TEST(PoTransactionTest, testProcessOpening_sessionBuffer_overflowBytesCounter_MulitpleMode)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT,
                               DEFAULT_KIF_DEBIT)
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

    poCommandsTestSet.insert({PO_READ_REC_SFI7_REC1_CMD, PO_READ_REC_SFI7_REC1_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI8_REC1_CMD, PO_READ_REC_SFI8_REC1_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI7_REC3_4_CMD, PO_READ_REC_SFI7_REC3_4_RSP});

    poTransaction->prepareReadRecordFile(FILE7, 1);
    poTransaction->prepareReadRecordFile(FILE8, 1);
    poTransaction->prepareReadRecordFile(FILE7, 3, 2, 29);
    poTransaction->processPoCommands();

    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE8)->getData()->getContent(1), FILE8_REC1_29B_BYTES);
    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE7)->getData()->getContent(1), FILE7_REC1_29B_BYTES);
    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE7)->getData()->getContent(3), FILE7_REC3_29B_BYTES);
    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE7)->getData()->getContent(4), FILE7_REC4_29B_BYTES);

    tearDown();
}

/* standard processPoCommands */
TEST(PoTransactionTest, testprocessPoCommands_nominalCase)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT,
                               DEFAULT_KIF_DEBIT)
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

    /* PoTransaction while a session is open */
    poTransaction->processPoCommands();

    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE7)->getData()->getContent(1), FILE7_REC1_29B_BYTES);
    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE8)->getData()->getContent(1), FILE8_REC1_29B_BYTES);

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
    poTransaction->prepareReleasePoChannel();

    /* Expected exception: no session is open */
    EXPECT_THROW(poTransaction->processClosing(), CalypsoPoTransactionIllegalStateException);

    tearDown();
}

/* Standard processClosing - default ratification */
TEST(PoTransactionTest, testProcessClosing_nominalCase)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT,
                               DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev31),
                        poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_INIT_OPEN_SECURE_SESSION_SFI7_REC1_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI8_REC1_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_RSP_OK_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI7_REC1_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI10_REC1_RSP_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI10_REC1_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI11_REC1_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI11_REC1_RSP_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_INIT_OPEN_SECURE_SESSION_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_DECREASE_SFI10_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_DECREASE_SFI10_RESP, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_INCREASE_SFI11_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_INCREASE_SFI11_RESP, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_UPDATE_REC_SFI7_REC1_4B_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_UPDATE_REC_SFI8_REC1_4B_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_WRITE_REC_SFI8_REC1_4B_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_APPEND_REC_SFI9_REC1_4B_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI11_REC1_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_CLOSE_CMD, SAM_DIGEST_CLOSE_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_AUTHENTICATE, SW1SW2_OK_RSP});

    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_SFI7_REC1_CMD,
                              PO_OPEN_SECURE_SESSION_SFI7_REC1_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI10_REC1_CMD, PO_READ_REC_SFI10_REC1_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI11_REC1_CMD, PO_READ_REC_SFI11_REC1_RSP});
    poCommandsTestSet.insert({PO_DECREASE_SFI10_REC1_100U_CMD, PO_DECREASE_SFI10_REC1_100U_RSP});
    poCommandsTestSet.insert({PO_DECREASE_SFI11_REC1_100U_CMD, PO_DECREASE_SFI11_REC1_100U_RSP});
    poCommandsTestSet.insert({PO_UPDATE_REC_SFI7_REC1_4B_CMD, SW1SW2_OK_RSP});
    poCommandsTestSet.insert({PO_WRITE_REC_SFI8_REC1_4B_CMD, SW1SW2_OK_RSP});
    poCommandsTestSet.insert({PO_APPEND_REC_SFI9_REC1_4B_CMD, SW1SW2_OK_RSP});
    poCommandsTestSet.insert({PO_CLOSE_SECURE_SESSION_CMD, PO_CLOSE_SECURE_SESSION_RSP});
    poCommandsTestSet.insert({PO_RATIFICATION_CMD, PO_RATIFICATION_RSP});

    poTransaction->prepareReadRecordFile(FILE7, 1);
    poTransaction->prepareReadRecordFile(FILE10, 1);
    poTransaction->prepareReadRecordFile(FILE11, 1);
    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

    poTransaction->prepareDecreaseCounter(FILE10, 1, 100);
    poTransaction->prepareIncreaseCounter(FILE11, 1, 100);
    poTransaction->prepareUpdateRecord(FILE7, 1, ByteArrayUtil::fromHex(FILE7_REC1_4B));
    poTransaction->prepareWriteRecord(FILE8, 1, ByteArrayUtil::fromHex(FILE8_REC1_4B));
    poTransaction->prepareAppendRecord(FILE9, ByteArrayUtil::fromHex(FILE9_REC1_4B));

    /* PoTransaction after a session is open */
    poTransaction->prepareReleasePoChannel();
    poTransaction->processClosing();

    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE10)->getData()->getContentAsCounterValue(1),
              0x1122 - 100);
    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE11)->getData() ->getContentAsCounterValue(1),
              0x2211 + 100);
    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE7)->getData()->getContent(1),
              ByteArrayUtil::fromHex(FILE7_REC1_4B));
    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE8)->getData()->getContent(1),
              ByteArrayUtil::fromHex(FILE8_REC1_4B));
    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE9)->getData()->getContent(1),
              ByteArrayUtil::fromHex(FILE9_REC1_4B));

    tearDown();
}

/* processClosing - PO fail on closing #1 Close Session is failing */
TEST(PoTransactionTest, testProcessClosing_poCloseFail)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT,
                               DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev31),
                        poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_INIT_OPEN_SECURE_SESSION_CMD, SW1SW2_OK_RSP});

    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI7_REC1_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_RSP_OK_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_APPEND_REC_SFI9_REC1_4B_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_CLOSE_CMD, SAM_DIGEST_CLOSE_RSP});

    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_CMD, PO_OPEN_SECURE_SESSION_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI7_REC1_CMD, PO_READ_REC_SFI7_REC1_RSP});
    poCommandsTestSet.insert({PO_APPEND_REC_SFI9_REC1_4B_CMD, SW1SW2_OK_RSP});
    poCommandsTestSet.insert({PO_CLOSE_SECURE_SESSION_CMD, PO_CLOSE_SECURE_SESSION_FAILED_RSP});

    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);
    poTransaction->prepareReadRecordFile(FILE7, 1);

    /*
     * PoTransaction after a session is open
     * should raise a CalypsoPoCloseSecureSessionException due to the Close Session failure
     */
    poTransaction->prepareReleasePoChannel();
    poTransaction->prepareAppendRecord(FILE9, ByteArrayUtil::fromHex(FILE9_REC1_4B));

    EXPECT_THROW(poTransaction->processClosing(), KeypleSeCommandException);

    tearDown();
}

/* processClosing - PO fail on closing #2 Command is failing */
TEST(PoTransactionTest, testProcessClosing_poCommandFail)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettings::PoSecuritySettingsBuilder(samResource)
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
    samCommandsTestSet.insert({SAM_DIGEST_INIT_OPEN_SECURE_SESSION_CMD, SW1SW2_OK_RSP});

    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI7_REC1_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_RSP_OK_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_APPEND_REC_SFI9_REC1_4B_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_CLOSE_CMD, SAM_DIGEST_CLOSE_RSP});

    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_CMD, PO_OPEN_SECURE_SESSION_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI7_REC1_CMD, PO_READ_REC_SFI7_REC1_RSP});
    poCommandsTestSet.insert({PO_APPEND_REC_SFI9_REC1_4B_CMD, SW1SW2_KO});
    poCommandsTestSet.insert({PO_CLOSE_SECURE_SESSION_CMD, PO_CLOSE_SECURE_SESSION_RSP});

    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

    poTransaction->prepareReadRecordFile(FILE7, 1);

    /*
     * PoTransaction after a session is open
     * should raise a CalypsoPoCommandException due to the append record failure
     */
    poTransaction->prepareReleasePoChannel();
    poTransaction->prepareAppendRecord(FILE9, ByteArrayUtil::fromHex(FILE9_REC1_4B));

    EXPECT_THROW(poTransaction->processClosing(), CalypsoPoCommandException);

    tearDown();
}

/* processClosing - SAM authentication fail on closing */
TEST(PoTransactionTest, testProcessClosing_samAuthenticateFail)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT,
                               DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev31),
                        poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_INIT_OPEN_SECURE_SESSION_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI7_REC1_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_RSP_OK_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_CLOSE_CMD, SAM_DIGEST_CLOSE_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_AUTHENTICATE, SAM_DIGEST_AUTHENTICATE_FAILED});

    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_CMD, PO_OPEN_SECURE_SESSION_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI7_REC1_CMD, PO_READ_REC_SFI7_REC1_RSP});
    poCommandsTestSet.insert({PO_CLOSE_SECURE_SESSION_CMD, PO_CLOSE_SECURE_SESSION_RSP});

    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);
    poTransaction->prepareReadRecordFile(FILE7, 1);

    /*
     * PoTransaction after a session is open
     * Should raise a CalypsoSessionAuthenticationException
     */
    poTransaction->prepareReleasePoChannel();

    EXPECT_THROW(poTransaction->processClosing(), KeypleSeCommandException);

    tearDown();
}


/* processClosing - SAM IO error while authenticating */
TEST(PoTransactionTest, testProcessClosing_samIoErrorAuthenticating)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT,
                               DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev31),
                        poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_INIT_OPEN_SECURE_SESSION_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI7_REC1_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_RSP_OK_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_CLOSE_CMD, SAM_DIGEST_CLOSE_RSP});

    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_CMD, PO_OPEN_SECURE_SESSION_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI7_REC1_CMD, PO_READ_REC_SFI7_REC1_RSP});
    poCommandsTestSet.insert({PO_CLOSE_SECURE_SESSION_CMD, PO_CLOSE_SECURE_SESSION_RSP});

    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);
    poTransaction->prepareReadRecordFile(FILE7, 1);

    /*
     * PoTransaction after a session is open
     * should raise a CalypsoAuthenticationNotVerifiedException
     */
    poTransaction->prepareReleasePoChannel();

    EXPECT_THROW(poTransaction->processClosing(), CalypsoAuthenticationNotVerifiedException);

    tearDown();
}

/*
 * Buffer overflow limit in atomic mode (counter in bytes): session buffer size
 * = 430 b, consumed size 430 b
 */
TEST(PoTransactionTest, testProcessClosing_sessionBuffer_limit)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT,
                               DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev31),
                        poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_INIT_OPEN_SECURE_SESSION_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI8_REC1_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_RSP_OK_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI7_REC1_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_UPDATE_REC_SFI8_REC1_29B_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_UPDATE_REC_SFI8_REC1_4B_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_CLOSE_CMD, SAM_DIGEST_CLOSE_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_AUTHENTICATE, SW1SW2_OK_RSP});

    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_CMD, PO_OPEN_SECURE_SESSION_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI7_REC1_CMD, PO_READ_REC_SFI7_REC1_RSP});
    poCommandsTestSet.insert({PO_UPDATE_REC_SFI8_REC1_29B_CMD, SW1SW2_OK_RSP});
    poCommandsTestSet.insert({PO_UPDATE_REC_SFI8_REC1_4B_CMD, SW1SW2_OK_RSP});
    poCommandsTestSet.insert({PO_CLOSE_SECURE_SESSION_CMD, PO_CLOSE_SECURE_SESSION_RSP});

    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

    /* Add additional non modifying commands (should not affect the session buffer) */
    for (int i = 0; i < 4; i++)
        poTransaction->prepareReadRecordFile(FILE7, 1);

    /* 12 x update (29 b) = 12 x (29 + 6) = 420 consumed in the session buffer */
    for (int i = 0; i < 12; i++)
        poTransaction->prepareUpdateRecord(FILE8, 1, FILE8_REC1_29B_BYTES);

    /* Insert additional non modifying commands (should not affect the session buffer) */
    for (int i = 0; i < 4; i++)
        poTransaction->prepareReadRecordFile(FILE7, 1);

    /* 4 additional bytes (10 b consumed) */
    poTransaction->prepareUpdateRecord(FILE8, 1, FILE8_REC1_4B_BYTES);

    /* PoTransaction after a session is open */
    poTransaction->prepareReleasePoChannel();
    poTransaction->processClosing();

    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE8)->getData()->getContent(1),
              FILE8_REC1_4B_BYTES);

    tearDown();
}

/*
 * Buffer overflowed in atomic mode (counter in bytes): session buffer size =
 * 430 b, consumed size 431 b
 */
TEST(PoTransactionTest, testProcessClosing_sessionBuffer_overflowed)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT,
                               DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev31),
                        poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP});

    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_CMD, PO_OPEN_SECURE_SESSION_RSP});

    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

    /* Add additional non modifying commands (should not affect the session buffer) */
    for (int i = 0; i < 4; i++)
        poTransaction->prepareReadRecordFile(FILE7, 1);

    /* 12 x update (29 b) = 12 x (29 + 6) = 420 consumed in the session buffer */
    for (int i = 0; i < 12; i++)
        poTransaction->prepareUpdateRecord(
            FILE8,
            1,
            ByteArrayUtil::fromHex("81111111111111111111111111111111111111111" \
                                   "11111111111111111"));

    /* Insert additional non modifying commands (should not affect the session  buffer) */
    for (int i = 0; i < 4; i++)
        poTransaction->prepareReadRecordFile(FILE7, 1);

    /* 4 additional bytes (10 b consumed) */
    poTransaction->prepareUpdateRecord(FILE8, 1, FILE8_REC1_5B_BYTES);

    /* PoTransaction after a session is open */
    poTransaction->prepareReleasePoChannel();
    EXPECT_THROW(poTransaction->processClosing(), CalypsoAtomicTransactionException);

    tearDown();
}

/*
 * Buffer overflowed in multiple mode (counter in bytes): session buffer size =
 * 430 b, consumed size 431 b
 */
TEST(PoTransactionTest, testProcessClosing_sessionBuffer_overflowMultipleMode)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT,
                               DEFAULT_KIF_DEBIT)
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
    samCommandsTestSet.insert({SAM_DIGEST_INIT_OPEN_SECURE_SESSION_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI8_REC1_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_RSP_OK_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI7_REC1_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_UPDATE_REC_SFI8_REC1_29B_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_UPDATE_REC_SFI8_REC1_4B_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI7_REC1_RSP, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_UPDATE_REC_SFI7_REC1_4B_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_CLOSE_CMD, SAM_DIGEST_CLOSE_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_AUTHENTICATE, SW1SW2_OK_RSP});

    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_CMD, PO_OPEN_SECURE_SESSION_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI7_REC1_CMD, PO_READ_REC_SFI7_REC1_RSP});
    poCommandsTestSet.insert({PO_UPDATE_REC_SFI8_REC1_29B_CMD, SW1SW2_OK_RSP});
    poCommandsTestSet.insert({PO_UPDATE_REC_SFI8_REC1_4B_CMD, SW1SW2_OK_RSP});
    poCommandsTestSet.insert({PO_CLOSE_SECURE_SESSION_CMD, PO_CLOSE_SECURE_SESSION_RSP});

    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

    /* Add additional non modifying commands (should not affect the session buffer) */
    for (int i = 0; i < 4; i++)
        poTransaction->prepareReadRecordFile(FILE7, 1);

    /* 12 x update (29 b) = 12 x (29 + 6) = 420 consumed in the session buffer */
    for (int i = 0; i < 12; i++)
        poTransaction->prepareUpdateRecord(FILE8, 1, FILE8_REC1_29B_BYTES);

    /* Insert additional non modifying commands (should not affect the session buffer) */
    for (int i = 0; i < 4; i++)
        poTransaction->prepareReadRecordFile(FILE7, 1);

    /* 4 additional bytes (10 b consumed) */
    poTransaction->prepareUpdateRecord(FILE8, 1, FILE8_REC1_4B_BYTES);

    /* PoTransaction after a session is open */
    poTransaction->prepareReleasePoChannel();
    poTransaction->processClosing();

    tearDown();
}

/* Standard processClosing - close not ratified */
TEST(PoTransactionTest, testProcessClosing_nominalCase_closeNotRatified)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT,
                               DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .ratificationMode(RatificationMode::CLOSE_NOT_RATIFIED)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev31),
                        poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_INIT_OPEN_SECURE_SESSION_SFI7_REC1_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_CLOSE_CMD, SAM_DIGEST_CLOSE_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_AUTHENTICATE, SW1SW2_OK_RSP});

    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_SFI7_REC1_CMD,
                              PO_OPEN_SECURE_SESSION_SFI7_REC1_RSP});
    poCommandsTestSet.insert({PO_CLOSE_SECURE_SESSION_NOT_RATIFIED_CMD,
                              PO_CLOSE_SECURE_SESSION_RSP});

    poTransaction->prepareReadRecordFile(FILE7, 1);
    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

    poTransaction->prepareReleasePoChannel();
    poTransaction->processClosing();

    tearDown();
}

/* Session buffer overflow in atomic mode: the overflow happens at closing */
TEST(PoTransactionTest, testTransaction_sessionBuffer_overflowAtomic)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT,
                               DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .build();

    poTransaction = std::make_shared<PoTransaction>(
                        std::make_shared<SeResource<CalypsoPo>>(poReader,
                                                                calypsoPoRev31),
                        poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_GET_CHALLENGE_CMD, SAM_GET_CHALLENGE_RSP});

    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_CMD, PO_OPEN_SECURE_SESSION_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI8_REC1_CMD, PO_READ_REC_SFI8_REC1_RSP});
    poCommandsTestSet.insert({PO_UPDATE_REC_SFI8_REC1_29B_CMD, SW1SW2_OK_RSP});
    poCommandsTestSet.insert({PO_CLOSE_SECURE_SESSION_CMD, PO_CLOSE_SECURE_SESSION_RSP});

    /* 4 x update (29 b) = 4 x (29 + 6) = 140 consumed in the session buffer */
    for (int i = 0; i < 4; i++)
        poTransaction->prepareUpdateRecord(FILE8, 1, FILE8_REC1_29B_BYTES);

    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

    /* Add additional non modifying commands (should not affect the session buffer) */
    for (int i = 0; i < 4; i++)
        poTransaction->prepareReadRecordFile(FILE8, 1);

    /* 4 x update (29 b) = 4 x (29 + 6) = 140 consumed in the session buffer */
    for (int i = 0; i < 4; i++)
        poTransaction->prepareUpdateRecord(FILE8, 1, FILE8_REC1_29B_BYTES);

    /* Insert additional non modifying commands (should not affect the session buffer) */
    for (int i = 0; i < 4; i++)
        poTransaction->prepareReadRecordFile(FILE8, 1);

    poTransaction->processPoCommands();

    /* 5 x update (29 b) = 5 x (29 + 6) = 140 consumed in the session buffer */
    for (int i = 0; i < 4; i++)
        poTransaction->prepareUpdateRecord(FILE8, 1, FILE8_REC1_29B_BYTES);

    /* 4 additional bytes (10 b consumed) */
    poTransaction->prepareUpdateRecord(FILE8, 1, FILE8_REC1_5B_BYTES);


    /* PoTransaction after a session is open */
    poTransaction->prepareReleasePoChannel();
    EXPECT_THROW(poTransaction->processClosing(), CalypsoAtomicTransactionException);

    tearDown();
}

/*
 * Session buffer overflow in multiple mode: the overflow happens and is handled
 * at closing
 */
TEST(PoTransactionTest, testTransaction_sessionBuffer_overflowMultiple)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT,
                               DEFAULT_KIF_DEBIT)
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
    samCommandsTestSet.insert({SAM_DIGEST_INIT_OPEN_SECURE_SESSION_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI8_REC1_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_RSP_OK_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_UPDATE_REC_SFI8_REC1_29B_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI7_REC1_RSP_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_READ_REC_SFI8_REC1_RSP_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_UPDATE_UPDATE_REC_SFI8_REC1_5B_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_CLOSE_CMD, SAM_DIGEST_CLOSE_RSP});
    samCommandsTestSet.insert({SAM_DIGEST_AUTHENTICATE, SW1SW2_OK_RSP});

    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_CMD, PO_OPEN_SECURE_SESSION_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI7_REC1_CMD, PO_READ_REC_SFI7_REC1_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI8_REC1_CMD, PO_READ_REC_SFI8_REC1_RSP});
    poCommandsTestSet.insert({PO_UPDATE_REC_SFI8_REC1_29B_CMD, SW1SW2_OK_RSP});
    poCommandsTestSet.insert({PO_UPDATE_REC_SFI8_REC1_5B_CMD, SW1SW2_OK_RSP});
    poCommandsTestSet.insert({PO_CLOSE_SECURE_SESSION_CMD, PO_CLOSE_SECURE_SESSION_RSP});

    /* 4 x update (29 b) = 4 x (29 + 6) = 140 consumed in the session buffer */
    for (int i = 0; i < 4; i++)
        poTransaction->prepareUpdateRecord(FILE8, 1, FILE8_REC1_29B_BYTES);

    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

    /* Add additional non modifying commands (should not affect the session buffer) */
    for (int i = 0; i < 4; i++)
        poTransaction->prepareReadRecordFile(FILE8, 1);

    /*
     * 24 x update (29 b) = 24 x (29 + 6) = 840 consumed in the session buffer force multiple cycles
     */
    for (int i = 0; i < 24; i++)
        poTransaction->prepareUpdateRecord(FILE8, 1, FILE8_REC1_29B_BYTES);

    /* Insert additional non modifying commands (should not affect the session buffer) */
    for (int i = 0; i < 4; i++)
        poTransaction->prepareReadRecordFile(FILE8, 1);

    poTransaction->processPoCommands();

    /*
     * 24 x update (29 b) = 24 x (29 + 6) = 840 consumed in the session buffer
     * force multiple cycles
     */
    for (int i = 0; i < 24; i++)
        poTransaction->prepareUpdateRecord(FILE8, 1, FILE8_REC1_29B_BYTES);

    /* 4 additional bytes (10 b consumed) */
    poTransaction->prepareUpdateRecord(FILE8, 1, FILE8_REC1_5B_BYTES);

    /* PoTransaction after a session is open */
    poTransaction->prepareReleasePoChannel();
    poTransaction->processClosing();

    tearDown();
}

/* open, cancel and reopen */
TEST(PoTransactionTest, testProcessCancel_open_cancelOpen)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);

    std::shared_ptr<PoSecuritySettings> poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT,
                               DEFAULT_KIF_DEBIT)
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
    poCommandsTestSet.insert({PO_OPEN_SECURE_SESSION_CMD, PO_OPEN_SECURE_SESSION_RSP});
    poCommandsTestSet.insert({PO_READ_REC_SFI8_REC1_CMD, PO_READ_REC_SFI8_REC1_RSP});
    /* Abort session */
    poCommandsTestSet.insert({PO_ABORT_SECURE_SESSION_CMD, SW1SW2_OK_RSP});

    poTransaction->prepareReadRecordFile(FILE7, 1);
    poTransaction->prepareReadRecordFile(FILE8, 1);
    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE7)->getData()->getContent(),
              FILE7_REC1_29B_BYTES);
    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE8)->getData()->getContent(),
              FILE8_REC1_29B_BYTES);
    ASSERT_TRUE(calypsoPoRev31->isDfRatified());

    poTransaction->processCancel();
    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

    tearDown();
}

/* Verify PIN Po commands */
TEST(PoTransactionTest, testProcessVerifyPin_no_pin_command_executed)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);
    auto se = std::make_shared<SeResource<CalypsoPo>>(poReader, calypsoPoRev31);
    poTransaction = std::make_shared<PoTransaction>(se);

    EXPECT_THROW(calypsoPoRev31->getPinAttemptRemaining(), IllegalStateException);

    tearDown();
}

TEST(PoTransactionTest, testProcessVerifyPin_plain_outside_secureSession)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);
    auto se = std::make_shared<SeResource<CalypsoPo>>(poReader, calypsoPoRev31);
    poTransaction = std::make_shared<PoTransaction>(se);

    poCommandsTestSet.insert({PO_VERIFY_PIN_PLAIN_OK_CMD, PO_VERIFY_PIN_OK_RSP});

    poTransaction->processVerifyPin(PIN_OK);

    ASSERT_EQ(calypsoPoRev31->getPinAttemptRemaining(), 3);

    tearDown();
}

TEST(PoTransactionTest, testProcessCheckPinStatus_outside_secureSession)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);
    auto se = std::make_shared<SeResource<CalypsoPo>>(poReader, calypsoPoRev31);
    poTransaction = std::make_shared<PoTransaction>(se);

    poCommandsTestSet.insert({PO_CHECK_PIN_CMD, PO_VERIFY_PIN_OK_RSP});

    poTransaction->prepareCheckPinStatus();
    poTransaction->processPoCommands();

    ASSERT_EQ(calypsoPoRev31->getPinAttemptRemaining(), 3);

    poCommandsTestSet.insert({PO_CHECK_PIN_CMD, PO_VERIFY_PIN_KO_RSP});

    poTransaction->prepareCheckPinStatus();
    poTransaction->prepareReleasePoChannel();
    poTransaction->processPoCommands();

    ASSERT_EQ(calypsoPoRev31->getPinAttemptRemaining(), 2);

    tearDown();
}

TEST(PoTransactionTest, testProcessVerifyPin_encrypted_outside_secureSession)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);
    auto poSecuritySettings =
        PoSecuritySettings::PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                           DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .pinTransmissionMode(PinTransmissionMode::ENCRYPTED)
            .build();
    auto se = std::make_shared<SeResource<CalypsoPo>>(poReader, calypsoPoRev31);
    poTransaction = std::make_shared<PoTransaction>(se, poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_GIVE_RANDOM_CMD, SAM_GIVE_RANDOM_RSP});
    samCommandsTestSet.insert({SAM_CARD_CIPHER_PIN_CMD, SAM_CARD_CIPHER_PIN_RSP});

    poCommandsTestSet.insert({PO_GET_CHALLENGE_CMD, PO_GET_CHALLENGE_RSP});
    poCommandsTestSet.insert({PO_VERIFY_PIN_ENCRYPTED_OK_CMD, PO_VERIFY_PIN_OK_RSP});

    poTransaction->processVerifyPin(PIN_OK);

    ASSERT_EQ(calypsoPoRev31->getPinAttemptRemaining(), 3);

    tearDown();
}

TEST(PoTransactionTest, testPrepareSelectFile_selectControl)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);
    auto se = std::make_shared<SeResource<CalypsoPo>>(poReader, calypsoPoRev31);
    poTransaction = std::make_shared<PoTransaction>(se);

    poCommandsTestSet.insert({PO_SELECT_FILE_CURRENT_CMD, PO_SELECT_FILE_3F00_RSP});
    poCommandsTestSet.insert({PO_SELECT_FILE_FIRST_CMD, PO_SELECT_FILE_0002_RSP});
    poCommandsTestSet.insert({PO_SELECT_FILE_NEXT_CMD, PO_SELECT_FILE_0003_RSP});

    poTransaction->prepareSelectFile(SelectFileControl::CURRENT_DF);
    poTransaction->prepareSelectFile(SelectFileControl::FIRST_EF);
    poTransaction->prepareSelectFile(SelectFileControl::NEXT_EF);
    poTransaction->processPoCommands();

    std::shared_ptr<DirectoryHeader> directoryHeader = calypsoPoRev31->getDirectoryHeader();
    std::shared_ptr<FileHeader> fileHeader1 = calypsoPoRev31->getFileByLid(0x02)->getHeader();
    std::shared_ptr<FileHeader> fileHeader2 = calypsoPoRev31->getFileByLid(0x03)->getHeader();

    ASSERT_EQ(directoryHeader->getLid(), LID_3F00);
    ASSERT_EQ(directoryHeader->getAccessConditions(),
              ByteArrayUtil::fromHex(ACCESS_CONDITIONS_3F00));
    ASSERT_EQ(directoryHeader->getKeyIndexes(), ByteArrayUtil::fromHex(KEY_INDEXES_3F00));
    ASSERT_EQ(directoryHeader->getDfStatus(), 0x00);
    ASSERT_EQ(directoryHeader->getKif(AccessLevel::SESSION_LVL_PERSO), 0x61);
    ASSERT_EQ(directoryHeader->getKif(AccessLevel::SESSION_LVL_LOAD), 0x67);
    ASSERT_EQ(directoryHeader->getKif(AccessLevel::SESSION_LVL_DEBIT), 0x70);
    ASSERT_EQ(directoryHeader->getKvc(AccessLevel::SESSION_LVL_PERSO), 0x77);
    ASSERT_EQ(directoryHeader->getKvc(AccessLevel::SESSION_LVL_LOAD), 0x78);
    ASSERT_EQ(directoryHeader->getKvc(AccessLevel::SESSION_LVL_DEBIT), 0x79);

    ASSERT_EQ(fileHeader1->getLid(), LID_0002);
    ASSERT_EQ(fileHeader1->getRecordsNumber(), 1);
    ASSERT_EQ(fileHeader1->getRecordSize(), 29);
    ASSERT_EQ(fileHeader1->getType(), FileHeader::FileType::LINEAR);
    ASSERT_EQ(fileHeader1->getAccessConditions(), ByteArrayUtil::fromHex(ACCESS_CONDITIONS_0002));
    ASSERT_EQ(fileHeader1->getKeyIndexes(), ByteArrayUtil::fromHex(KEY_INDEXES_0002));
    ASSERT_EQ(fileHeader1->getDfStatus(), 0x00);
    ASSERT_EQ(fileHeader1->getSharedReference(), 0x3F02);

    ASSERT_EQ(fileHeader2->getLid(), LID_0003);
    ASSERT_EQ(fileHeader2->getRecordsNumber(), 1);
    ASSERT_EQ(fileHeader2->getRecordSize(), 29);
    ASSERT_EQ(fileHeader2->getType(), FileHeader::FileType::LINEAR);
    ASSERT_EQ(fileHeader2->getAccessConditions(), ByteArrayUtil::fromHex(ACCESS_CONDITIONS_0003));
    ASSERT_EQ(fileHeader2->getKeyIndexes(), ByteArrayUtil::fromHex(KEY_INDEXES_0003));
    ASSERT_EQ(fileHeader2->getDfStatus(), 0x00);
    ASSERT_EQ(fileHeader2->getSharedReference(), 0x3F03);

    tearDown();
}

TEST(PoTransactionTest, testPrepareSelectFile_lid)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);
    auto se = std::make_shared<SeResource<CalypsoPo>>(poReader, calypsoPoRev31);
    poTransaction = std::make_shared<PoTransaction>(se);

    poCommandsTestSet.insert({PO_SELECT_FILE_3F00_CMD, PO_SELECT_FILE_3F00_RSP});
    poCommandsTestSet.insert({PO_SELECT_FILE_0002_CMD, PO_SELECT_FILE_0002_RSP});
    poCommandsTestSet.insert({PO_SELECT_FILE_0003_CMD, PO_SELECT_FILE_0003_RSP});

    poTransaction->prepareSelectFile(ByteArrayUtil::fromHex(LID_3F00_STR));
    poTransaction->prepareSelectFile(ByteArrayUtil::fromHex(LID_0002_STR));
    poTransaction->prepareSelectFile(ByteArrayUtil::fromHex(LID_0003_STR));
    poTransaction->processPoCommands();

    std::shared_ptr<DirectoryHeader> directoryHeader =
        calypsoPoRev31->getDirectoryHeader();
    std::shared_ptr<ElementaryFile> file1 = calypsoPoRev31->getFileByLid(0x02);
    std::shared_ptr<ElementaryFile> file2 = calypsoPoRev31->getFileByLid(0x03);

    const uint8_t sfi1 = file1->getSfi();
    const uint8_t sfi2 = file2->getSfi();

    ASSERT_EQ(calypsoPoRev31->getFileBySfi(sfi1), file1);
    ASSERT_EQ(calypsoPoRev31->getFileBySfi(sfi2), file2);

    ASSERT_EQ(directoryHeader->getLid(), LID_3F00);
    ASSERT_EQ(directoryHeader->getAccessConditions(),
              ByteArrayUtil::fromHex(ACCESS_CONDITIONS_3F00));
    ASSERT_EQ(directoryHeader->getKeyIndexes(), ByteArrayUtil::fromHex(KEY_INDEXES_3F00));
    ASSERT_EQ(directoryHeader->getDfStatus(), 0x00);
    ASSERT_EQ(directoryHeader->getKif(AccessLevel::SESSION_LVL_PERSO), 0x61);
    ASSERT_EQ(directoryHeader->getKif(AccessLevel::SESSION_LVL_LOAD), 0x67);
    ASSERT_EQ(directoryHeader->getKif(AccessLevel::SESSION_LVL_DEBIT), 0x70);
    ASSERT_EQ(directoryHeader->getKvc(AccessLevel::SESSION_LVL_PERSO), 0x77);
    ASSERT_EQ(directoryHeader->getKvc(AccessLevel::SESSION_LVL_LOAD), 0x78);
    ASSERT_EQ(directoryHeader->getKvc(AccessLevel::SESSION_LVL_DEBIT), 0x79);

    std::shared_ptr<FileHeader> fileHeader1 = file1->getHeader();
    ASSERT_EQ(fileHeader1->getLid(), LID_0002);
    ASSERT_EQ(fileHeader1->getRecordsNumber(), 1);
    ASSERT_EQ(fileHeader1->getRecordSize(), 29);
    ASSERT_EQ(fileHeader1->getType(), FileHeader::FileType::LINEAR);
    ASSERT_EQ(fileHeader1->getAccessConditions(), ByteArrayUtil::fromHex(ACCESS_CONDITIONS_0002));
    ASSERT_EQ(fileHeader1->getKeyIndexes(), ByteArrayUtil::fromHex(KEY_INDEXES_0002));
    ASSERT_EQ(fileHeader1->getDfStatus(), 0x00);
    ASSERT_EQ(fileHeader1->getSharedReference(), 0x3F02);

    std::shared_ptr<FileHeader> fileHeader2 = file2->getHeader();
    ASSERT_EQ(fileHeader2->getLid(), LID_0003);
    ASSERT_EQ(fileHeader2->getRecordsNumber(), 1);
    ASSERT_EQ(fileHeader2->getRecordSize(), 29);
    ASSERT_EQ(fileHeader2->getType(), FileHeader::FileType::LINEAR);
    ASSERT_EQ(fileHeader2->getAccessConditions(), ByteArrayUtil::fromHex(ACCESS_CONDITIONS_0003));
    ASSERT_EQ(fileHeader2->getKeyIndexes(), ByteArrayUtil::fromHex(KEY_INDEXES_0003));
    ASSERT_EQ(fileHeader2->getDfStatus(), 0x00);
    ASSERT_EQ(fileHeader2->getSharedReference(), 0x3F03);

    tearDown();
}

TEST(PoTransactionTest, testPrepareReadCounterFile)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);
    auto se = std::make_shared<SeResource<CalypsoPo>>(poReader, calypsoPoRev31);
    poTransaction = std::make_shared<PoTransaction>(se);

    poCommandsTestSet.insert({PO_READ_REC_SFI7_REC1_6B_COUNTER_CMD,
                              PO_READ_REC_SFI7_REC1_6B_COUNTER_RSP});

    poTransaction->prepareReadCounterFile(FILE7, 2);
    poTransaction->processPoCommands();

    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE7)->getData()->getContentAsCounterValue(1),
              ByteArrayUtil::threeBytesToInt(ByteArrayUtil::fromHex(FILE7_REC1_COUNTER1), 0));
    ASSERT_EQ(calypsoPoRev31->getFileBySfi(FILE7)->getData()->getContentAsCounterValue(2),
              ByteArrayUtil::threeBytesToInt(ByteArrayUtil::fromHex(FILE7_REC1_COUNTER2), 0));

    tearDown();
}

TEST(PoTransactionTest, testPrepareSvGet_Reload)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);
    auto poSecuritySettings = std::make_shared<PoSecuritySettingsBuilder>(samResource)->build();
    auto seResource = std::make_shared<SeResource<CalypsoPo>>(poReader, calypsoPoRev31);
    poTransaction = std::make_shared<PoTransaction>(seResource, poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_PREPARE_LOAD_CMD, SAM_PREPARE_LOAD_RSP});
    samCommandsTestSet.insert({SAM_SV_CHECK_CMD, SW1SW2_OK});

    poCommandsTestSet.insert({PO_SV_GET_RELOAD_CMD, PO_SV_GET_RELOAD_RSP});
    poCommandsTestSet.insert({PO_SV_RELOAD_CMD, PO_SV_RELOAD_RSP});

    poTransaction->prepareSvGet(Operation::RELOAD, PoTransaction::SvSettings::Action::DO);
    poTransaction->processPoCommands();
    poTransaction->prepareSvReload(2);
    poTransaction->prepareReleasePoChannel();
    poTransaction->processPoCommands();

    ASSERT_EQ(calypsoPoRev31->getSvBalance(), SV_BALANCE);
    ASSERT_NE(calypsoPoRev31->getSvLoadLogRecord(), nullptr);

    EXPECT_THROW(calypsoPoRev31->getSvDebitLogLastRecord(), NoSuchElementException);

    tearDown();
}

TEST(PoTransactionTest, testPrepareSvGet_Reload_AllLogs)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);
    auto poSecuritySettings = std::make_shared<PoSecuritySettingsBuilder>(samResource)
                                  ->svGetLogReadMode(LogRead::ALL)
                                   .build();
    auto seResource = std::make_shared<SeResource<CalypsoPo>>(poReader, calypsoPoRev31);
    poTransaction = std::make_shared<PoTransaction>(seResource, poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_PREPARE_LOAD_CMD, SAM_PREPARE_LOAD_RSP});
    samCommandsTestSet.insert({SAM_SV_CHECK_CMD, SW1SW2_OK});

    poCommandsTestSet.insert({PO_SV_GET_DEBIT_CMD, PO_SV_GET_DEBIT_RSP});
    poCommandsTestSet.insert({PO_SV_GET_RELOAD_CMD, PO_SV_GET_RELOAD_RSP});
    poCommandsTestSet.insert({PO_SV_RELOAD_CMD, PO_SV_RELOAD_RSP});

    poTransaction->prepareSvGet(Operation::RELOAD, PoTransaction::SvSettings::Action::DO);
    poTransaction->processPoCommands();
    poTransaction->prepareSvReload(2);
    poTransaction->prepareReleasePoChannel();
    poTransaction->processPoCommands();

    ASSERT_EQ(calypsoPoRev31->getSvBalance(), SV_BALANCE);
    ASSERT_NE(calypsoPoRev31->getSvLoadLogRecord(), nullptr);
    ASSERT_NE(calypsoPoRev31->getSvDebitLogLastRecord(), nullptr);

    tearDown();
}

TEST(PoTransactionTest, testPrepareSvGet_Debit)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);
    auto poSecuritySettings = std::make_shared<PoSecuritySettingsBuilder>(samResource)->build();
    auto seResource = std::make_shared<SeResource<CalypsoPo>>(poReader, calypsoPoRev31);
    poTransaction = std::make_shared<PoTransaction>(seResource, poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_PREPARE_DEBIT_CMD, SAM_PREPARE_DEBIT_RSP});
    samCommandsTestSet.insert({SAM_SV_CHECK_CMD, SW1SW2_OK});

    poCommandsTestSet.insert({PO_SV_GET_DEBIT_CMD, PO_SV_GET_DEBIT_RSP});
    poCommandsTestSet.insert({PO_SV_DEBIT_CMD, PO_SV_DEBIT_RSP});

    poTransaction->prepareSvGet(Operation::DEBIT, PoTransaction::SvSettings::Action::DO);
    poTransaction->processPoCommands();
    poTransaction->prepareSvDebit(2);
    poTransaction->prepareReleasePoChannel();
    poTransaction->processPoCommands();

    ASSERT_EQ(calypsoPoRev31->getSvBalance(), SV_BALANCE);
    ASSERT_NE(calypsoPoRev31->getSvDebitLogLastRecord(), nullptr);

    tearDown();
}

TEST(PoTransactionTest, testPrepareSvGet_Undebit)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);
    auto poSecuritySettings = std::make_shared<PoSecuritySettingsBuilder>(samResource)->build();
    auto seResource = std::make_shared<SeResource<CalypsoPo>>(poReader, calypsoPoRev31);
    poTransaction = std::make_shared<PoTransaction>(seResource, poSecuritySettings);

    samCommandsTestSet.insert({SAM_SELECT_DIVERSIFIER_CMD, SW1SW2_OK_RSP});
    samCommandsTestSet.insert({SAM_PREPARE_UNDEBIT_CMD, SAM_PREPARE_UNDEBIT_RSP});
    samCommandsTestSet.insert({SAM_SV_CHECK_CMD, SW1SW2_OK});

    poCommandsTestSet.insert({PO_SV_GET_DEBIT_CMD, PO_SV_GET_DEBIT_RSP});
    poCommandsTestSet.insert({PO_SV_UNDEBIT_CMD, PO_SV_UNDEBIT_RSP});

    poTransaction->prepareSvGet(Operation::DEBIT, PoTransaction::SvSettings::Action::UNDO);
    poTransaction->processPoCommands();
    poTransaction->prepareSvDebit(2);
    poTransaction->prepareReleasePoChannel();
    poTransaction->processPoCommands();

    ASSERT_EQ(calypsoPoRev31->getSvBalance(), SV_BALANCE);
    ASSERT_NE(calypsoPoRev31->getSvDebitLogLastRecord(), nullptr);

    tearDown();
}

TEST(PoTransactionTest, testPrepareSvReadAllLogs)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_STORED_VALUE_REV31);
    auto seResource = std::make_shared<SeResource<CalypsoPo>>(poReader, calypsoPoRev31);
    poTransaction = std::make_shared<PoTransaction>(seResource);

    poCommandsTestSet.insert({PO_READ_SV_LOAD_LOG_FILE_CMD, PO_READ_SV_LOAD_LOG_FILE_RSP});
    poCommandsTestSet.insert({PO_READ_SV_DEBIT_LOG_FILE_CMD, PO_READ_SV_DEBIT_LOG_FILE_RSP});

    poTransaction->prepareSvReadAllLogs();
    poTransaction->prepareReleasePoChannel();
    poTransaction->processPoCommands();

    ASSERT_NE(calypsoPoRev31->getSvLoadLogRecord(), nullptr);
    ASSERT_NE(calypsoPoRev31->getSvDebitLogLastRecord(), nullptr);

    std::vector<std::shared_ptr<SvDebitLogRecord>> allDebitLogs =
        calypsoPoRev31->getSvDebitLogAllRecords();

    ASSERT_EQ(static_cast<int>(calypsoPoRev31->getSvDebitLogAllRecords().size()), 3);
    ASSERT_EQ(allDebitLogs[0]->getSamId(), 0xAABBCC01);
    ASSERT_EQ(allDebitLogs[1]->getSamId(), 0xAABBCC02);
    ASSERT_EQ(allDebitLogs[2]->getSamId(), 0xAABBCC03);

    tearDown();
}

TEST(PoTransactionTest, testPrepareInvalidate_notInvalidated)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);
    auto seResource = std::make_shared<SeResource<CalypsoPo>>(poReader, calypsoPoRev31);
    poTransaction = std::make_shared<PoTransaction>(seResource);

    poCommandsTestSet.insert({PO_INVALIDATE_CMD, SW1SW2_OK_RSP});

    poTransaction->prepareInvalidate();
    poTransaction->prepareReleasePoChannel();
    poTransaction->processPoCommands();

    tearDown();
}

TEST(PoTransactionTest, testPrepareInvalidate_invalidated)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31_INVALIDATED);
    auto seResource = std::make_shared<SeResource<CalypsoPo>>(poReader, calypsoPoRev31);
    poTransaction = std::make_shared<PoTransaction>(seResource);

    poCommandsTestSet.insert({PO_INVALIDATE_CMD, SW1SW2_OK_RSP});

    poTransaction->prepareInvalidate();
    poTransaction->prepareReleasePoChannel();

    EXPECT_THROW(poTransaction->processPoCommands(), CalypsoPoTransactionIllegalStateException);

    tearDown();
}

TEST(PoTransactionTest, testPrepareRehabilitate_notInvalidated)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);
    auto seResource = std::make_shared<SeResource<CalypsoPo>>(poReader, calypsoPoRev31);
    poTransaction = std::make_shared<PoTransaction>(seResource);

    poCommandsTestSet.insert({PO_REHABILITATE_CMD, SW1SW2_OK_RSP});

    poTransaction->prepareRehabilitate();
    poTransaction->prepareReleasePoChannel();

    EXPECT_THROW(poTransaction->processPoCommands(), CalypsoPoTransactionIllegalStateException);

    tearDown();
}

TEST(PoTransactionTest, testPrepareRehabilitate_invalidated)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31_INVALIDATED);
    auto seResource = std::make_shared<SeResource<CalypsoPo>>(poReader, calypsoPoRev31);
    poTransaction = std::make_shared<PoTransaction>(seResource);

    poCommandsTestSet.insert({PO_REHABILITATE_CMD, SW1SW2_OK_RSP});

    poTransaction->prepareRehabilitate();
    poTransaction->prepareReleasePoChannel();
    poTransaction->processPoCommands();

    tearDown();
}

TEST(PoTransactionTest, testPoIoException)
{
    setUp();

    std::shared_ptr<CalypsoPo> calypsoPoRev31 = createCalypsoPo(FCI_REV31);
    auto se = std::make_shared<SeResource<CalypsoPo>>(poReader, calypsoPoRev31);
    poTransaction = std::make_shared<PoTransaction>(se);

    poTransaction->prepareReadRecordFile(FILE7, 1);
    poTransaction->prepareReleasePoChannel();

    EXPECT_THROW(poTransaction->processPoCommands(), CalypsoPoIOException);

    tearDown();
}

TEST(PoTransactionTest, testAccessLevel)
{
    setUp();

    ASSERT_EQ(AccessLevel::SESSION_LVL_PERSO.getName(), "perso");
    ASSERT_EQ(AccessLevel::SESSION_LVL_LOAD.getName(), "load");
    ASSERT_EQ(AccessLevel::SESSION_LVL_DEBIT.getName(), "debit");
    ASSERT_EQ(AccessLevel::SESSION_LVL_PERSO.getSessionKey(), 0x01);
    ASSERT_EQ(AccessLevel::SESSION_LVL_LOAD.getSessionKey(), 0x02);
    ASSERT_EQ(AccessLevel::SESSION_LVL_DEBIT.getSessionKey(), 0x03);

    tearDown();
}
