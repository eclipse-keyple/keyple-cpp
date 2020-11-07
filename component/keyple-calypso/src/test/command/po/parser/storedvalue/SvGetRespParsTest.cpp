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

#include "SvGetRespPars.h"

/* Common */
#include "IllegalStateException.h"

/* Core */
#include "ByteArrayUtil.h"

using namespace testing;

using namespace keyple::calypso::command::po::parser::storedvalue;
using namespace keyple::common::exception;
using namespace keyple::core::util;

TEST(SvGetRespParsTest, badLength)
{
    std::vector<uint8_t> header = ByteArrayUtil::fromHex("7C000721");
    auto apduResponse = std::make_shared<ApduResponse>(ByteArrayUtil::fromHex("0011 9000"), nullptr);

    EXPECT_THROW(SvGetRespPars(header, apduResponse, nullptr), IllegalStateException);
}

TEST(SvGetRespParsTest, modeRevision3_2_base)
{
    std::vector<uint8_t> header = ByteArrayUtil::fromHex("7C000721");
    auto apduResponse = std::make_shared<ApduResponse>(
                            ByteArrayUtil::fromHex(
                                /* Challenge (8) */ std::string("0011223344556677") +
                                /* Current KVC (1) */ "55" +
                                /* SV TNum (2) */ "A55A" +
                                /* Previous SignatureLo (6) */ "665544332211" +
                                /* SV Balance (3) */ "000000" +
                                /* Load Date (2) */ "1234" +
                                /* Load Free1 (1) */ "22" +
                                /* Load KVC (1) */ "AA" +
                                /* Load Free2 (1) */ "33" +
                                /* Load Balance (3) */ "001121" +
                                /* Load Amount (3) */ "000001" +
                                /* Load Time (2) */ "5678" +
                                /* Load SAM ID (4) */ "AABBCCDD" +
                                /* Load SAM TNum (3) */ "D23456" +
                                /* Load SV TNum (2) */ "E567" +
                                /* Debit Amount (2) */ "0001" +
                                /* Debit Date (2) */ "1235" +
                                /* Debit Time (2) */ "6789" +
                                /* Debit KVC (1) */ "BB" +
                                /* Debit SAM ID (4) */ "BBCCDDEE" +
                                /* Debit SAM TNum (3) */ "A34567" +
                                /* Debit Balance (3) */ "001120" +
                                /* Debit SV TNum (2) */ "F568" +
                                /* Successful execution status word */ "9000"),
                            nullptr);

    SvGetRespPars svGetRespPars(header, apduResponse, nullptr);

    /* Check length */
    ASSERT_EQ(svGetRespPars.getApduResponse()->getBytes().size(), 0x3D + 2);

    /* Check common fields */
    ASSERT_EQ(svGetRespPars.getChallengeOut(), ByteArrayUtil::fromHex("0011223344556677"));
    ASSERT_EQ(svGetRespPars.getBalance(), 0);
    ASSERT_EQ(svGetRespPars.getCurrentKVC(), 0x55);
    ASSERT_EQ(svGetRespPars.getTransactionNumber(), 0xA55A);
    ASSERT_EQ(svGetRespPars.getPreviousSignatureLo(), ByteArrayUtil::fromHex("665544332211"));

    // TODO Review this
    // /* check load log fields */
    // ASSERT_EQ(ByteArrayUtil::fromHex("1234"),
    // svGetRespPars.getLoadLog()->getDate());
    // ASSERT_EQ(ByteArrayUtil::fromHex("5678"),
    // svGetRespPars.getLoadLog()->getTime());
    // ASSERT_EQ(ByteArrayUtil::fromHex("2233"),
    // svGetRespPars.getLoadLog()->getFree());
    // ASSERT_EQ(0xAA, svGetRespPars.getLoadLog()->getKVC());
    // ASSERT_EQ(0x1121, svGetRespPars.getLoadLog()->getBalance());
    // ASSERT_EQ(0x01, svGetRespPars.getLoadLog()->getAmount());
    // ASSERT_EQ(ByteArrayUtil::fromHex("AABBCCDD"),
    // svGetRespPars.getLoadLog()->getSamID());
    // ASSERT_EQ(0xD23456, svGetRespPars.getLoadLog()->getSamTransactionNumber());
    // ASSERT_EQ(0xE567, svGetRespPars.getLoadLog()->getSvTransactionNumber());
    //
    // /* check debit log fields */
    // ASSERT_EQ(ByteArrayUtil::fromHex("1235"),
    // svGetRespPars.getDebitLog()->getDate());
    // ASSERT_EQ(ByteArrayUtil::fromHex("6789"),
    // svGetRespPars.getDebitLog()->getTime());
    // ASSERT_EQ(0xBB, svGetRespPars.getDebitLog()->getKVC());
    // ASSERT_EQ(0x1120, svGetRespPars.getDebitLog()->getBalance());
    // ASSERT_EQ(0x01, svGetRespPars.getDebitLog()->getAmount());
    // ASSERT_EQ(ByteArrayUtil::fromHex("BBCCDDEE"),
    // svGetRespPars.getDebitLog()->getSamID());
    // ASSERT_EQ(0xA34567, svGetRespPars.getDebitLog()->getSamTransactionNumber());
    // ASSERT_EQ(0xF568, svGetRespPars.getDebitLog()->getSvTransactionNumber());
}

TEST(SvGetRespParsTest, modeRevision3_2_B1_LB_1_LA_1_DB_1_DA_1)
{
    std::vector<uint8_t> header = ByteArrayUtil::fromHex("7C000721");
    /*
     * B1_LB1_LA1_DB1_DA1: balance 1, load balance 1, load amount 1, debit balance 1, debit
     * amount 1
     */
    auto apduResponse = std::make_shared<ApduResponse>(
                            ByteArrayUtil::fromHex(
                                std::string("001122334455667755A55A665544332211") +
                                /* B 1 */ "000001" + "123422AA33" +
                                /* LB 1 LA 1 */ "000001000001" + "5678AABBCCDD1234564567" +
                                /* DA 1 */ "0001" + "12356789BBBBCCDDEE234567" +
                                /* DB 1 */ "000001" + "45689000"),
                            nullptr);

    SvGetRespPars svGetRespPars(header, apduResponse, nullptr);

    ASSERT_EQ(svGetRespPars.getBalance(), 1);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getBalance(), 1);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getAmount(), 1);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getBalance(), 1);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getAmount(), 1);
}

TEST(SvGetRespParsTest, modeRevision3_2_B256_LB_257_LA_256_DB_256_DA_257)
{
    std::vector<uint8_t> header = ByteArrayUtil::fromHex("7C000721");
    auto apduResponse = std::make_shared<ApduResponse>(
                            ByteArrayUtil::fromHex(
                                std::string("001122334455667755A55A665544332211") +
                                /* B 256 */ "000100" + "123422AA33" +
                                /* LB 257 LA 256 */ "000101000100" + "5678AABBCCDD1234564567" +
                                /* DA 257 */ "0101" + "12356789BBBBCCDDEE234567" +
                                /* DB 256 */ "000100" + "45689000"),
                            nullptr);

    SvGetRespPars svGetRespPars(header, apduResponse, nullptr);

    ASSERT_EQ(svGetRespPars.getBalance(), 256);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getBalance(), 257);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getAmount(), 256);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getBalance(), 256);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getAmount(), 257);
}

TEST(SvGetRespParsTest, modeRevision3_2_B65536_LB_65537_LA_65536_DB_65536_DA_m32768)
{
    std::vector<uint8_t> header = ByteArrayUtil::fromHex("7C000721");
    auto apduResponse = std::make_shared<ApduResponse>(
                            ByteArrayUtil::fromHex(
                                std::string("001122334455667755A55A665544332211") +
                                /* B 65536 */ "010000" + "123422AA33" +
                                /* LB 65537 LA 65536 */ "010001010000" + "5678AABBCCDD1234564567" +
                                /* DA -32768 */ "8000" + "12356789BBBBCCDDEE234567" +
                                /* DB 65536 */ "010000" + "45689000"),
                            nullptr);

    SvGetRespPars svGetRespPars(header, apduResponse, nullptr);

    ASSERT_EQ(svGetRespPars.getBalance(), 65536);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getBalance(), 65537);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getAmount(), 65536);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getBalance(), 65536);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getAmount(), -32768);
}

/* Highest value */
TEST(SvGetRespParsTest, modeRevision3_2_B8388607_LB8388607_LA_8388607_DA_32767_DB8388607)
{
    std::vector<uint8_t> header = ByteArrayUtil::fromHex("7C000721");
    auto apduResponse = std::make_shared<ApduResponse>(
                            ByteArrayUtil::fromHex(
                                std::string("001122334455667755A55A665544332211") +
                                /* 8388607 */ "7FFFFF" + "123422AA33" +
                                /* LB 8388606 LA 8388607 */ "7FFFFF7FFFFF" + "5678AABBCCDD1234564567" +
                                /* DA 32767 */ "7FFF" + "12356789BBBBCCDDEE234567" +
                                /* DB 8388607 */ "7FFFFF" + "45689000"),
                            nullptr);

    SvGetRespPars svGetRespPars(header, apduResponse, nullptr);

    ASSERT_EQ(svGetRespPars.getBalance(), 8388607);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getBalance(), 8388607);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getAmount(), 8388607);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getBalance(), 8388607);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getAmount(), 32767);
}

TEST(SvGetRespParsTest, modeRevision3_2_Bm1_LBm1_LAm1_DBm1_DAm1)
{
    std::vector<uint8_t> header = ByteArrayUtil::fromHex("7C000721");
    auto apduResponse = std::make_shared<ApduResponse>(
                            ByteArrayUtil::fromHex(
                                std::string("001122334455667755A55A665544332211") +
                                /* -1 */ "FFFFFF" + "123422AA33" +
                                /* LB -1 LA -1 */ "FFFFFFFFFFFF" + "5678AABBCCDD1234564567" +
                                /* DA -1 */ "FFFF" + "12356789BBBBCCDDEE234567" +
                                /* DB -1 */ "FFFFFF" + "45689000"),
                            nullptr);

    SvGetRespPars svGetRespPars(header, apduResponse, nullptr);

    ASSERT_EQ(svGetRespPars.getBalance(), -1);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getBalance(), -1);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getAmount(), -1);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getBalance(), -1);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getAmount(), -1);
}

/* lowest values */
TEST(SvGetRespParsTest, modeRevision3_2_Bm8388608)
{
    std::vector<uint8_t> header = ByteArrayUtil::fromHex("7C000721");
    auto apduResponse = std::make_shared<ApduResponse>(
                            ByteArrayUtil::fromHex(
                                std::string("001122334455667755A55A665544332211") +
                                /* -8388608 */ "800000" + "123422AA33" +
                                /* LB -8388608 LA -8388608 */ "800000800000" + "5678AABBCCDD1234564567" +
                                /* DA -32768 */ "8000" + "12356789BBBBCCDDEE234567" +
                                /* DB -8388608 */ "800000" + "45689000"),
                            nullptr);

    SvGetRespPars svGetRespPars(header, apduResponse, nullptr);

    ASSERT_EQ(svGetRespPars.getBalance(), -8388608);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getBalance(), -8388608);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getAmount(), -8388608);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getBalance(), -8388608);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getAmount(), -32768);
}

TEST(SvGetRespParsTest, modeCompat_base_Reload)
{
    std::vector<uint8_t> header = ByteArrayUtil::fromHex("7C000721");
    auto apduResponse = std::make_shared<ApduResponse>(
                            ByteArrayUtil::fromHex(
                                /* Current KVC (1) */ std::string("55") +
                                /* SV TNum (2) */ "A55A" +
                                /* Previous SignatureLo (6) */ "665544" +
                                /* Challenge out */ "1122" +
                                /* SV Balance (3) */ "000000" +
                                /* Load Date (2) */ "1234" +
                                /* Load Free1 (1) */ "22" +
                                /* Load KVC (1) */ "AA" +
                                /* Load Free2 (1) */ "33" +
                                /* Load Balance (3) */ "001121" +
                                /* Load Amount (3) */ "000001" +
                                /* Load Time (2) */ "5678" +
                                /* Load SAM ID (4) */ "AABBCCDD" +
                                /* Load SAM TNum (3) */ "D23456" +
                                /* Load SV TNum (2) */ "E567" +
                                /* Successful execution status word */ "9000"),
                            nullptr);

    SvGetRespPars svGetRespPars(header, apduResponse, nullptr);

    /* Check length */
    ASSERT_EQ(svGetRespPars.getApduResponse()->getBytes().size(), 0x21 + 2);

    /* Check common fields */
    ASSERT_EQ(svGetRespPars.getChallengeOut(), ByteArrayUtil::fromHex("1122"));
    ASSERT_EQ(svGetRespPars.getBalance(), 0);
    ASSERT_EQ(svGetRespPars.getCurrentKVC(), 0x55);
    ASSERT_EQ(svGetRespPars.getTransactionNumber(), 0xA55A);
    ASSERT_EQ(svGetRespPars.getPreviousSignatureLo(), ByteArrayUtil::fromHex("665544"));

    // TODO Review this
    // /* check load log fields */
    // ASSERT_EQ(ByteArrayUtil::fromHex("1234"),
    // svGetRespPars.getLoadLog()->getDate());
    // ASSERT_EQ(ByteArrayUtil::fromHex("5678"),
    // svGetRespPars.getLoadLog()->getTime());
    // ASSERT_EQ(ByteArrayUtil::fromHex("2233"),
    // svGetRespPars.getLoadLog()->getFree());
    // ASSERT_EQ(0xAA, svGetRespPars.getLoadLog()->getKVC());
    // ASSERT_EQ(0x1121, svGetRespPars.getLoadLog()->getBalance());
    // ASSERT_EQ(0x01, svGetRespPars.getLoadLog()->getAmount());
    // ASSERT_EQ(ByteArrayUtil::fromHex("AABBCCDD"),
    // svGetRespPars.getLoadLog()->getSamID());
    // ASSERT_EQ(0xD23456, svGetRespPars.getLoadLog()->getSamTransactionNumber());
    // ASSERT_EQ(0xE567, svGetRespPars.getLoadLog()->getSvTransactionNumber());
}

TEST(SvGetRespParsTest, modeCompat_Reload_B1_LB1_LA1)
{
    std::vector<uint8_t> header = ByteArrayUtil::fromHex("7C000721");
    auto apduResponse = std::make_shared<ApduResponse>(
                            ByteArrayUtil::fromHex(
                                std::string("55A55A6655441122") +
                                /* B1 */ "000001" + "123422AA33" +
                                /* LB 1 */ "000001" +
                                /* LA 1 */"000001" + "5678AABBCCDDD23456E5679000"),
                            nullptr);

    SvGetRespPars svGetRespPars(header, apduResponse, nullptr);

    ASSERT_EQ(svGetRespPars.getBalance(), 1);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getBalance(), 1);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getAmount(), 1);
}

TEST(SvGetRespParsTest, modeCompat_Reload_B256_LB257_LA256)
{
    std::vector<uint8_t> header = ByteArrayUtil::fromHex("7C000721");
    auto apduResponse = std::make_shared<ApduResponse>(
                             ByteArrayUtil::fromHex(
                                 std::string("55A55A6655441122") +
                                /* B1 */ "000100" + "123422AA33" +
                                /* LB 1 */ "000101" +
                                /* LA 1 */"000100" + "5678AABBCCDDD23456E5679000"),
                            nullptr);

    SvGetRespPars svGetRespPars(header, apduResponse, nullptr);

    ASSERT_EQ(svGetRespPars.getBalance(), 256);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getBalance(), 257);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getAmount(), 256);
}

TEST(SvGetRespParsTest, modeCompat_Reload_B65536_LB65537_LA65536)
{
    std::vector<uint8_t> header = ByteArrayUtil::fromHex("7C000721");
    auto apduResponse = std::make_shared<ApduResponse>(
                            ByteArrayUtil::fromHex(
                                std::string("55A55A6655441122") +
                                /* B1 */ "010000" + "123422AA33" +
                                /* LB 1 */ "010001" +
                                /* LA 1 */"010000" + "5678AABBCCDDD23456E5679000"),
                            nullptr);

    SvGetRespPars svGetRespPars(header, apduResponse, nullptr);

    ASSERT_EQ(svGetRespPars.getBalance(), 65536);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getBalance(), 65537);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getAmount(), 65536);
}

TEST(SvGetRespParsTest, modeCompat_Reload_B8388607_LB8388607_LA_8388607)
{
    std::vector<uint8_t> header = ByteArrayUtil::fromHex("7C000721");
    auto apduResponse = std::make_shared<ApduResponse>(
                            ByteArrayUtil::fromHex(
                                std::string("55A55A6655441122") +
                                /* B1 */ "7FFFFF" + "123422AA33" +
                                /* LB 1 */ "7FFFFF" +
                                /* LA 1 */"7FFFFF" + "5678AABBCCDDD23456E5679000"),
                            nullptr);

    SvGetRespPars svGetRespPars(header, apduResponse, nullptr);

    ASSERT_EQ(svGetRespPars.getBalance(), 8388607);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getBalance(), 8388607);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getAmount(), 8388607);
}

TEST(SvGetRespParsTest, modeCompat_Reload_Bm8388608)
{
    std::vector<uint8_t> header = ByteArrayUtil::fromHex("7C000721");
    auto apduResponse = std::make_shared<ApduResponse>(
                            ByteArrayUtil::fromHex(
                                std::string("55A55A6655441122") +
                                /* B1 */ "800000" + "123422AA33" +
                                /* LB 1 */ "800000" +
                                /* LA 1 */"800000" + "5678AABBCCDDD23456E5679000"),
                            nullptr);

    SvGetRespPars svGetRespPars(header, apduResponse, nullptr);

    ASSERT_EQ(svGetRespPars.getBalance(), -8388608);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getBalance(), -8388608);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getAmount(), -8388608);
}

TEST(SvGetRespParsTest, modeCompat_Reload_Bm1)
{
    std::vector<uint8_t> header = ByteArrayUtil::fromHex("7C000721");
    auto apduResponse = std::make_shared<ApduResponse>(
                            ByteArrayUtil::fromHex(
                                std::string("55A55A6655441122") +
                                /* B1 */ "FFFFFF" + "123422AA33" +
                                /* LB 1 */ "FFFFFF" +
                                /* LA 1 */"FFFFFF" + "5678AABBCCDDD23456E5679000"),
                            nullptr);

    SvGetRespPars svGetRespPars(header, apduResponse, nullptr);

    ASSERT_EQ(svGetRespPars.getBalance(), -1);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getBalance(), -1);
    ASSERT_EQ(svGetRespPars.getLoadLog()->getAmount(), -1);
}

TEST(SvGetRespParsTest, modeCompat_base_Debit)
{
    std::vector<uint8_t> header = ByteArrayUtil::fromHex("7C000721");
    auto apduResponse = std::make_shared<ApduResponse>(
                            ByteArrayUtil::fromHex(
                                /* Current KVC (1) */ std::string("55") +
                                /* SV TNum (2) */ "A55A" +
                                /* Previous SignatureLo (6) */ "665544" +
                                /* Challenge out */ "1122" +
                                /* SV Balance (3) */ "000000" +
                                /* Debit Amount (2) */ "0001" +
                                /* Debit Date (2) */ "1235" +
                                /* Debit Time (2) */ "6789" +
                                /* Debit KVC (1) */ "BB" +
                                /* Debit SAM ID (4) */ "BBCCDDEE" +
                                /* Debit SAM TNum (3) */ "A34567" +
                                /* Debit Balance (3) */ "001120" +
                                /* Debit SV TNum (2) */ "F568" +
                                /* Successful execution status word */ "9000"),
                            nullptr);

    SvGetRespPars svGetRespPars(header, apduResponse, nullptr);

    /* check length */
    ASSERT_EQ(0x1E + 2, svGetRespPars.getApduResponse()->getBytes().size());

    /* check common fields */
    ASSERT_EQ(ByteArrayUtil::fromHex("1122"), svGetRespPars.getChallengeOut());
    ASSERT_EQ(0, svGetRespPars.getBalance());
    ASSERT_EQ(0x55, svGetRespPars.getCurrentKVC());
    ASSERT_EQ(0xA55A, svGetRespPars.getTransactionNumber());
    ASSERT_EQ(ByteArrayUtil::fromHex("665544"),
            svGetRespPars.getPreviousSignatureLo());

    // TODO Review this
    // /* check debit log fields */
    // ASSERT_EQ(ByteArrayUtil::fromHex("1235"),
    // svGetRespPars.getDebitLog()->getDate());
    // ASSERT_EQ(ByteArrayUtil::fromHex("6789"),
    // svGetRespPars.getDebitLog()->getTime());
    // ASSERT_EQ(0xBB, svGetRespPars.getDebitLog()->getKVC());
    // ASSERT_EQ(0x1120, svGetRespPars.getDebitLog()->getBalance());
    // ASSERT_EQ(0x01, svGetRespPars.getDebitLog()->getAmount());
    // ASSERT_EQ(ByteArrayUtil::fromHex("BBCCDDEE"),
    // svGetRespPars.getDebitLog()->getSamID());
    // ASSERT_EQ(0xA34567, svGetRespPars.getDebitLog()->getSamTransactionNumber());
    // ASSERT_EQ(0xF568, svGetRespPars.getDebitLog()->getSvTransactionNumber());
}

TEST(SvGetRespParsTest, modeCompat_Debit_B1_LB1_LA1)
{
    std::vector<uint8_t> header = ByteArrayUtil::fromHex("7C000721");
    auto apduResponse = std::make_shared<ApduResponse>(
                            ByteArrayUtil::fromHex(
                                std::string("55A55A6655441122") +
                                /* B1 */ "000001" +
                                /* DA 1 */ "0001" + "12356789BBBBCCDDEEA34567" +
                                /* DB 1 */ "000001" + "F5689000"),
                            nullptr);

    SvGetRespPars svGetRespPars(header, apduResponse, nullptr);

    ASSERT_EQ(svGetRespPars.getBalance(), 1);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getBalance(), 1);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getAmount(), 1);
}

TEST(SvGetRespParsTest, modeCompat_Debit_B256_LB257_LA256)
{
    std::vector<uint8_t> header = ByteArrayUtil::fromHex("7C000721");
    auto apduResponse = std::make_shared<ApduResponse>(
                            ByteArrayUtil::fromHex(
                                std::string("55A55A6655441122") +
                                /* B1 */ "000100" +
                                /* DA 1 */ "0101" + "12356789BBBBCCDDEEA34567" +
                                /* DB 1 */ "000100" + "F5689000"),
                            nullptr);

    SvGetRespPars svGetRespPars(header, apduResponse, nullptr);

    ASSERT_EQ(svGetRespPars.getBalance(), 256);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getBalance(), 256);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getAmount(), 257);
}

TEST(SvGetRespParsTest, modeCompat_Debit_B65536_LB8000_LA65536)
{
    std::vector<uint8_t> header = ByteArrayUtil::fromHex("7C000721");
    auto apduResponse = std::make_shared<ApduResponse>(
                            ByteArrayUtil::fromHex(
                                std::string("55A55A6655441122") +
                                /* B1 */ "010000" +
                                /* DA 1 */ "8000" + "12356789BBBBCCDDEEA34567" +
                                /* DB 1 */ "010000" + "F5689000"),
                            nullptr);

    SvGetRespPars svGetRespPars(header, apduResponse, nullptr);

    ASSERT_EQ(svGetRespPars.getBalance(), 65536);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getBalance(), 65536);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getAmount(), -32768);
}

TEST(SvGetRespParsTest, modeCompat_Debit_B8388607_LB32767_LA8388607)
{
    std::vector<uint8_t> header = ByteArrayUtil::fromHex("7C000721");
    auto apduResponse = std::make_shared<ApduResponse>(
                            ByteArrayUtil::fromHex(
                                std::string("55A55A6655441122") +
                                /* B1 */ "7FFFFF" +
                                /* DA 1 */ "7FFF" + "12356789BBBBCCDDEEA34567" +
                                /* DB 1 */ "7FFFFF" + "F5689000"),
                            nullptr);

    SvGetRespPars svGetRespPars(header, apduResponse, nullptr);

    ASSERT_EQ(svGetRespPars.getBalance(), 8388607);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getBalance(), 8388607);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getAmount(), 32767);
}

TEST(SvGetRespParsTest, modeCompat_Debit_mB8388608)
{
    std::vector<uint8_t> header = ByteArrayUtil::fromHex("7C000721");
    auto apduResponse = std::make_shared<ApduResponse>(
                            ByteArrayUtil::fromHex(
                                std::string("55A55A6655441122") +
                                /* B1 */ "800000" +
                                /* DA 1 */ "8000" + "12356789BBBBCCDDEEA34567" +
                                /* DB 1 */ "800000" + "F5689000"),
                            nullptr);

    SvGetRespPars svGetRespPars(header, apduResponse, nullptr);

    ASSERT_EQ(svGetRespPars.getBalance(), -8388608);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getBalance(), -8388608);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getAmount(), -32768);
}

TEST(SvGetRespParsTest, modeCompat_Debit_m1)
{
    std::vector<uint8_t> header = ByteArrayUtil::fromHex("7C000721");
    auto apduResponse = std::make_shared<ApduResponse>(
                            ByteArrayUtil::fromHex(
                                std::string("55A55A6655441122") +
                                /* B1 */ "FFFFFF" +
                                /* DA 1 */ "FFFF" + "12356789BBBBCCDDEEA34567" +
                                /* DB 1 */ "FFFFFF" + "F5689000"),
                            nullptr);

    SvGetRespPars svGetRespPars(header, apduResponse, nullptr);

    ASSERT_EQ(svGetRespPars.getBalance(), -1);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getBalance(), -1);
    ASSERT_EQ(svGetRespPars.getDebitLog()->getAmount(), -1);
}
