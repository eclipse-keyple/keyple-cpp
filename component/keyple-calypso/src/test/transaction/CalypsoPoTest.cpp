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

#include "CalypsoPo.h"

/* Calypso */
#include "DirectoryHeader.h"
#include "PoRevision.h"

/* Common */
#include "Arrays.h"
#include "IllegalStateException.h"
#include "NoSuchElementException.h"
#include "stringhelper.h"

/* Core */
#include "AnswerToReset.h"
#include "ByteArrayUtil.h"
#include "SelectionStatus.h"
#include "TransmissionMode.h"

using namespace testing;

using namespace keyple::calypso::command::po;
using namespace keyple::calypso::transaction;
using namespace keyple::common;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;

static const std::string ATR_VALUE = "3B8F8001805A08030400020011223344829000F3";
static const std::string ATR_VALUE_2 = "3B8F8001805A08030400020011223344829000";
/*
 * Rev 3.1 FCI sample data
 * 6F
 * 23
 *    84
 *    09
 *    315449432E49434131      // AID
 *    A5
 *    16
 *        BF0C
 *        13
 *            C7
 *            08
 *            0000000011223344 // SERIAL NUMBER
 *            53
 *            07
 *            0A3C2305141001   // STARTUP INFORMATION
 *
 * STARTUP INFORMATION
 *    0A Buffer size indicator
 *    3C Type of platform
 *    2F Calypso revision
 *    05 File structure reference
 *    14 Software issuer reference
 *    10 Software version (MSB)
 *    01 Software version (LSB)
 */
static const std::string FCI_REV31 = "6F238409315449432E49434131A516BF0C13C70" \
                                     "8000000001122334453070A3C2F051410019000";
static const std::string FCI_REV31_FLAGS_FALSE = "6F238409315449432E49434131A" \
                                                 "516BF0C13C70800000000112233" \
                                                 "4453070A3C20051410019000";
static const std::string FCI_REV31_INVALIDATED = "6F238409315449432E49434131A" \
                                                 "516BF0C13C70800000000112233" \
                                                 "4453070A3C2F051410016283";
static const std::string FCI_REV31_HCE = "6F238409315449432E49434131A516BF0C1" \
                                         "3C708998811223344556653070A3C2F0514" \
                                         "10019000";
static const std::string DF_NAME = "315449432E494341";
static const std::string SERIAL_NUMBER = "0000000011223344";

/**
 * Build a CalypsoPo from atr and fci provided as hex strings
 *
 * @param atrStr the PO ATR
 * @param fciStr the FCI data
 * @return a CalypsoPo object
 */
static std::shared_ptr<CalypsoPo> getCalypsoPo(const std::string& atrStr,
                                               const std::string& fciStr)
{
    std::shared_ptr<AnswerToReset> atr = nullptr;

    if (atrStr.size() == 0)
        atr = nullptr;
    else
        atr = std::make_shared<AnswerToReset>(ByteArrayUtil::fromHex(atrStr));

    std::shared_ptr<ApduResponse> fci = nullptr;

    if (fciStr.size() == 0)
        fci = std::make_shared<ApduResponse>(ByteArrayUtil::fromHex("6700"),
                                             nullptr);
    else
        fci = std::make_shared<ApduResponse>(ByteArrayUtil::fromHex(fciStr),
                                             nullptr);

    std::shared_ptr<SeResponse> selectionData =
        std::make_shared<SeResponse>(
            true,
            false,
            std::make_shared<SelectionStatus>(atr, fci, true),
            std::vector<std::shared_ptr<ApduResponse>>{});

    return std::make_shared<CalypsoPo>(selectionData,
                                       TransmissionMode::CONTACTLESS);
}

/* Building FCI data with the application byte as a variant and initialize PO */
static std::shared_ptr<CalypsoPo> getPoApplicationByte(
    const uint8_t applicationByte)
{
    const std::string fciStr = "6F 22 84 08 " +
                               DF_NAME +
                               "A5 16 BF0C 13 C7 08 " +
                               SERIAL_NUMBER +
                               "53 07 060A " +
                               StringHelper::uint8ToHexString(applicationByte) +
                               " 02200311 9000";

    return getCalypsoPo(ATR_VALUE, fciStr);
}

TEST(CalypsoPoTest, getRevision)
{
    ASSERT_EQ(getPoApplicationByte(0x01)->getRevision(), PoRevision::REV2_4);
    ASSERT_EQ(getPoApplicationByte(0x04)->getRevision(), PoRevision::REV2_4);
    ASSERT_EQ(getPoApplicationByte(0x06)->getRevision(), PoRevision::REV2_4);
    ASSERT_EQ(getPoApplicationByte(0x1F)->getRevision(), PoRevision::REV2_4);
    ASSERT_EQ(getPoApplicationByte(0x20)->getRevision(), PoRevision::REV3_1);
    ASSERT_EQ(getPoApplicationByte(0x27)->getRevision(), PoRevision::REV3_1);
    ASSERT_EQ(getPoApplicationByte(0x28)->getRevision(), PoRevision::REV3_2);
    ASSERT_EQ(getPoApplicationByte(0x2F)->getRevision(), PoRevision::REV3_2);
    ASSERT_EQ(getPoApplicationByte(0x90)->getRevision(),
              PoRevision::REV3_1_CLAP);
}

TEST(CalypsoPoTest, getDfNameBytes)
{
    ASSERT_EQ(getPoApplicationByte(0x01)->getDfNameBytes(),
              ByteArrayUtil::fromHex(DF_NAME));
}

TEST(CalypsoPoTest, getDfName)
{
    ASSERT_EQ(getPoApplicationByte(0x01)->getDfName(), DF_NAME);
}

TEST(CalypsoPoTest, getApplicationSerialNumber)
{
    ASSERT_EQ(getPoApplicationByte(0x01)->getApplicationSerialNumberBytes(),
              ByteArrayUtil::fromHex(SERIAL_NUMBER));
}

TEST(CalypsoPoTest, getAtrBytes)
{
    ASSERT_EQ(getPoApplicationByte(0x01)->getAtrBytes(),
              ByteArrayUtil::fromHex(ATR_VALUE));
}

TEST(CalypsoPoTest, getAtr)
{
    ASSERT_EQ(getPoApplicationByte(0x01)->getAtr(), ATR_VALUE);
}

TEST(CalypsoPoTest, isModificationsCounterInBytes)
{
    /* False for rev 2 */
    ASSERT_FALSE(getPoApplicationByte(0x01)->isModificationsCounterInBytes());

    /* True for rev 3 */
    ASSERT_TRUE(getPoApplicationByte(0x27)->isModificationsCounterInBytes());
}

TEST(CalypsoPoTest, getModificationsCounter)
{
    /* 6 for rev 2 */
    ASSERT_EQ(getPoApplicationByte(0x01)->getModificationsCounter(), 6);

    /* 215 bytes for rev 3 */
    ASSERT_EQ(getPoApplicationByte(0x27)->getModificationsCounter(), 215);
}

TEST(CalypsoPoTest, getPoClass)
{
    /* LEGACY for rev 2 */
    ASSERT_EQ(getPoApplicationByte(0x01)->getPoClass(), PoClass::LEGACY);

    /* ISO bytes for rev 3 */
    ASSERT_EQ(getPoApplicationByte(0x27)->getPoClass(), PoClass::ISO);
}

TEST(CalypsoPoTest, getStartupInfo)
{
    std::shared_ptr<CalypsoPo> calypsoPo = getCalypsoPo(ATR_VALUE, FCI_REV31);

    /* Startup info */
    ASSERT_EQ(calypsoPo->getStartupInfo(), "0A3C2F05141001");
}

/* Cannot test this because isSerialNumberExpiring is protected */
// TEST(CalypsoPoTest, isSerialNumberExpiring)
// {
//     EXPECT_THROW(calypsoPo->isSerialNumberExpiring(), IllegalStateException);
// }


/* Cannot test this because getSerialNumberExpirationBytes is protected */
// TEST(CalypsoPoTest, getSerialNumberExpirationBytes)
// {
//     EXPECT_THROW(calypsoPo->getSerialNumberExpirationBytes(),
//                  IllegalStateException);
// }

TEST(CalypsoPoTest, getCalypsoAndApplicationSerialNumber)
{
    std::shared_ptr<CalypsoPo> calypsoPo =
        getCalypsoPo(ATR_VALUE, FCI_REV31_HCE);

    /* Cannot test this because getCalypsoSerialNumber is protected */
    //ASSERT_EQ(calypsoPo->getCalypsoSerialNumber(),
    //          ByteArrayUtil::fromHex("9988112233445566"));
    ASSERT_EQ(calypsoPo->getApplicationSerialNumber(), "0000112233445566");
}

TEST(CalypsoPoTest, testRev1_1)
{
    std::shared_ptr<AnswerToReset> atr =
        std::make_shared<AnswerToReset>(ByteArrayUtil::fromHex(ATR_VALUE_2));
    std::shared_ptr<ApduResponse> fciData =
        std::make_shared<ApduResponse>(std::vector<uint8_t>{}, nullptr);
    std::shared_ptr<SeResponse> selectionData =
        std::make_shared<SeResponse>(
            true,
            false,
            std::make_shared<SelectionStatus>(atr, fciData, true),
            std::vector<std::shared_ptr<ApduResponse>>{});

    EXPECT_THROW(
        std::make_shared<CalypsoPo>(selectionData,
                                    TransmissionMode::CONTACTLESS),
        IllegalStateException);
}

TEST(CalypsoPoTest, testRev1_2)
{
    std::shared_ptr<AnswerToReset> atr =
        std::make_shared<AnswerToReset>(ByteArrayUtil::fromHex(ATR_VALUE));
    std::shared_ptr<ApduResponse> fciData =
        std::make_shared<ApduResponse>(std::vector<uint8_t>{}, nullptr);
    std::shared_ptr<SeResponse> selectionData =
        std::make_shared<SeResponse>(
            true,
            false,
            std::make_shared<SelectionStatus>(atr, fciData, true),
            std::vector<std::shared_ptr<ApduResponse>>{});

    std::shared_ptr<CalypsoPo> calypsoPo =
        std::make_shared<CalypsoPo>(selectionData,
                                    TransmissionMode::CONTACTLESS);

    ASSERT_EQ(calypsoPo->getRevision(), PoRevision::REV1_0);
    ASSERT_EQ(static_cast<int>(calypsoPo->getDfNameBytes().size()), 0);
    ASSERT_EQ(calypsoPo->getApplicationSerialNumber(), SERIAL_NUMBER);
    ASSERT_FALSE(calypsoPo->isModificationsCounterInBytes());
    ASSERT_EQ(calypsoPo->getModificationsCounter(), 3);
    ASSERT_EQ(calypsoPo->getPoClass(), PoClass::LEGACY);
}

TEST(CalypsoPoTest, testFlags_true)
{
    std::shared_ptr<CalypsoPo> calypsoPo = getCalypsoPo(ATR_VALUE, FCI_REV31);

    ASSERT_TRUE(calypsoPo->isConfidentialSessionModeSupported());
    /* Negative logic for this one */
    ASSERT_FALSE(calypsoPo->isDeselectRatificationSupported());
    ASSERT_TRUE(calypsoPo->isPinFeatureAvailable());
    ASSERT_TRUE(calypsoPo->isSvFeatureAvailable());
}

TEST(CalypsoPoTest, testFlags_false)
{
    std::shared_ptr<CalypsoPo> calypsoPo = getCalypsoPo(ATR_VALUE,
                                                        FCI_REV31_FLAGS_FALSE);

    ASSERT_FALSE(calypsoPo->isConfidentialSessionModeSupported());
    /* Negative logic for this one */
    ASSERT_TRUE(calypsoPo->isDeselectRatificationSupported());
    ASSERT_FALSE(calypsoPo->isPinFeatureAvailable());
    ASSERT_FALSE(calypsoPo->isSvFeatureAvailable());
}

TEST(CalypsoPoTest, testDfInvalidated)
{
    std::shared_ptr<CalypsoPo> calypsoPo = getCalypsoPo(ATR_VALUE,
                                                        FCI_REV31_INVALIDATED);
    ASSERT_TRUE(calypsoPo->isDfInvalidated());

    calypsoPo = getCalypsoPo(ATR_VALUE, FCI_REV31);

    ASSERT_FALSE(calypsoPo->isDfInvalidated());
}

TEST(CalypsoPoTest, otherAttributes)
{
    /* Rev1 PO */
    std::shared_ptr<CalypsoPo> calypsoPo = getCalypsoPo(ATR_VALUE, "6700");

    ASSERT_FALSE(calypsoPo->isModificationsCounterInBytes());
    ASSERT_EQ(calypsoPo->getModificationsCounter(), 3);
    ASSERT_EQ(calypsoPo->getSessionModification(), 0x03);
    ASSERT_EQ(calypsoPo->getPlatform(), 0x08);
    ASSERT_EQ(calypsoPo->getApplicationType(), 0x03);
    ASSERT_EQ(calypsoPo->getApplicationSubtype(), 0x04);
    ASSERT_EQ(calypsoPo->getSoftwareIssuer(), 0x00);
    ASSERT_EQ(calypsoPo->getSoftwareVersion(), 0x02);
    ASSERT_EQ(calypsoPo->getSoftwareRevision(), 0x00);

    /* Rev 3.1 */
    calypsoPo = getCalypsoPo(ATR_VALUE, FCI_REV31);

    ASSERT_TRUE(calypsoPo->isModificationsCounterInBytes());
    ASSERT_EQ(calypsoPo->getModificationsCounter(), 430);
    ASSERT_EQ(calypsoPo->getSessionModification(), 0x0A);
    ASSERT_EQ(calypsoPo->getPlatform(), 0x3C);
    ASSERT_EQ(calypsoPo->getApplicationType(), 0x2F);
    ASSERT_EQ(calypsoPo->getApplicationSubtype(), 0x05);
    ASSERT_EQ(calypsoPo->getSoftwareIssuer(), 0x14);
    ASSERT_EQ(calypsoPo->getSoftwareVersion(), 0x10);
    ASSERT_EQ(calypsoPo->getSoftwareRevision(), 0x01);
}

TEST(CalypsoPoTest, etDirectoryHeader_whenDfIsNotSet_shouldReturnNull)
{
    std::shared_ptr<CalypsoPo> calypsoPo = getCalypsoPo(ATR_VALUE, FCI_REV31);

    ASSERT_EQ(calypsoPo->getDirectoryHeader(), nullptr);
}

TEST(CalypsoPoTest, getDirectoryHeader_whenDfIsSet_shouldReturnAReference)
{
    std::shared_ptr<DirectoryHeader> df = DirectoryHeader::builder()->build();
    std::shared_ptr<CalypsoPo> calypsoPo = getCalypsoPo(ATR_VALUE, FCI_REV31);
    calypsoPo->setDirectoryHeader(df);

    ASSERT_EQ(calypsoPo->getDirectoryHeader(), df);
}

TEST(CalypsoPoTest, setDirectoryHeader_shouldSetAReference)
{
    std::shared_ptr<DirectoryHeader> df = DirectoryHeader::builder()->build();
    std::shared_ptr<CalypsoPo> calypsoPo = getCalypsoPo(ATR_VALUE, FCI_REV31);
    calypsoPo->setDirectoryHeader(df);

    ASSERT_EQ(calypsoPo->getDirectoryHeader(), df);
}

TEST(CalypsoPoTest, getFileBySfi_whenSfiIsNotFound_shouldThrowNSEE)
{
    std::shared_ptr<CalypsoPo> calypsoPo = getCalypsoPo(ATR_VALUE, FCI_REV31);

    EXPECT_THROW(calypsoPo->getFileBySfi(1), NoSuchElementException);
}

TEST(CalypsoPoTest, getFileBySfi_whenSfiIsFound_shouldReturnAReference)
{
    std::shared_ptr<CalypsoPo> calypsoPo = getCalypsoPo(ATR_VALUE, FCI_REV31);
    calypsoPo->setContent(1, 1, std::vector<uint8_t>(1));
    std::shared_ptr<ElementaryFile> ref1 = calypsoPo->getFileBySfi(1);
    std::shared_ptr<ElementaryFile> ref2 = calypsoPo->getFileBySfi(1);

    ASSERT_EQ(ref1, ref2);
}

TEST(CalypsoPoTest, getFileByLid_whenLidIsNotFound_shouldThrowNSEE)
{
    std::shared_ptr<CalypsoPo> calypsoPo = getCalypsoPo(ATR_VALUE, FCI_REV31);

    EXPECT_THROW(calypsoPo->getFileByLid(1), NoSuchElementException);
}

TEST(CalypsoPoTest, getFileByLid_whenLidIsFound_shouldReturnAReference)
{
    std::shared_ptr<CalypsoPo> calypsoPo = getCalypsoPo(ATR_VALUE, FCI_REV31);
    calypsoPo->setFileHeader(1, FileHeader::builder()->lid(2).build());
    std::shared_ptr<ElementaryFile> ref1 = calypsoPo->getFileByLid(2);
    std::shared_ptr<ElementaryFile> ref2 = calypsoPo->getFileByLid(2);

    ASSERT_EQ(ref1, ref2);
}

TEST(CalypsoPoTest,
     getAllFiles_whenFilesAreNotSet_shouldReturnANotNullInstance)
{
    std::shared_ptr<CalypsoPo> calypsoPo = getCalypsoPo(ATR_VALUE, FCI_REV31);

    /*
     * C++ vs. Java: Java simply tests that the returned data is not null,
     *               which cannot happen here because it's a reference to a
     *               vector. Instead we can make sure the size *is* 0 because
     *               nothing should be in that vector yet.
     */
    ASSERT_EQ(static_cast<int>(calypsoPo->getAllFiles().size()), 0);
}

TEST(CalypsoPoTest, getAllFiles_whenFilesAreSet_shouldReturnAReference)
{
    std::shared_ptr<CalypsoPo> calypsoPo = getCalypsoPo(ATR_VALUE, FCI_REV31);
    calypsoPo->setContent(1, 1, std::vector<uint8_t>(1));
    const auto& ref1 = calypsoPo->getAllFiles();
    const auto& ref2 = calypsoPo->getAllFiles();

    ASSERT_EQ(ref1, ref2);
}

TEST(CalypsoPoTest, setFileHeader_whenSfiIsNotSet_shouldCreateEf)
{
    std::shared_ptr<CalypsoPo> calypsoPo = getCalypsoPo(ATR_VALUE, FCI_REV31);

    EXPECT_THROW(calypsoPo->getFileBySfi(1), NoSuchElementException);

    calypsoPo->setFileHeader(1, FileHeader::builder()->lid(2).build());

    ASSERT_NE(calypsoPo->getFileBySfi(1), nullptr);
}

TEST(CalypsoPoTest, setFileHeader_whenSfiIsSet_shouldReplaceHeader)
{
    std::shared_ptr<FileHeader> h1 = FileHeader::builder()->lid(1).build();
    std::shared_ptr<FileHeader> h2 = FileHeader::builder()->lid(2).build();

    std::shared_ptr<CalypsoPo> calypsoPo = getCalypsoPo(ATR_VALUE, FCI_REV31);
    calypsoPo->setFileHeader(1, h1);
    calypsoPo->setFileHeader(1, h2);

    ASSERT_EQ(calypsoPo->getFileBySfi(1)->getHeader(), h2);
}

TEST(CalypsoPoTest, setContentP3_whenSfiIsNotSet_shouldCreateEf)
{
    std::shared_ptr<CalypsoPo> calypsoPo = getCalypsoPo(ATR_VALUE, FCI_REV31);

    EXPECT_THROW(calypsoPo->getFileBySfi(1), NoSuchElementException);

    calypsoPo->setContent(1, 1, std::vector<uint8_t>(1));

   ASSERT_NE(calypsoPo->getFileBySfi(1), nullptr);
}

TEST(CalypsoPoTest, fillContent_whenSfiIsNotSet_shouldCreateEf)
{
    std::shared_ptr<CalypsoPo> calypsoPo = getCalypsoPo(ATR_VALUE, FCI_REV31);

    EXPECT_THROW(calypsoPo->getFileBySfi(1), NoSuchElementException);

    std::vector<uint8_t> fill(1);
    calypsoPo->fillContent(1, 1, fill);

    ASSERT_NE(calypsoPo->getFileBySfi(1), nullptr);
}

TEST(CalypsoPoTest, setCounter_whenSfiIsNotSet_shouldCreateEf)
{
    std::shared_ptr<CalypsoPo> calypsoPo = getCalypsoPo(ATR_VALUE, FCI_REV31);

    EXPECT_THROW(calypsoPo->getFileBySfi(1), NoSuchElementException);

    calypsoPo->setCounter(1, 1, std::vector<uint8_t>(3));

    ASSERT_NE(calypsoPo->getFileBySfi(1), nullptr);
}

TEST(CalypsoPoTest, setContentP4_whenSfiIsNotSet_shouldCreateEf)
{
    std::shared_ptr<CalypsoPo> calypsoPo = getCalypsoPo(ATR_VALUE, FCI_REV31);

    EXPECT_THROW(calypsoPo->getFileBySfi(1), NoSuchElementException);

    calypsoPo->setContent(1, 1, std::vector<uint8_t>(1));

    ASSERT_NE(calypsoPo->getFileBySfi(1), nullptr);
}

TEST(CalypsoPoTest, addCyclicContent_whenSfiIsNotSet_shouldCreateEf)
{
    std::shared_ptr<CalypsoPo> calypsoPo = getCalypsoPo(ATR_VALUE, FCI_REV31);

    EXPECT_THROW(calypsoPo->getFileBySfi(1), NoSuchElementException);

    calypsoPo->addCyclicContent(1, std::vector<uint8_t>(1));

    ASSERT_NE(calypsoPo->getFileBySfi(1), nullptr);
}

TEST(CalypsoPoTest, backupFiles_and_restoreFiles)
{
    std::shared_ptr<CalypsoPo> calypsoPo = getCalypsoPo(ATR_VALUE, FCI_REV31);
    const std::vector<uint8_t> vec = {1};
    calypsoPo->setContent(1, 1, vec);
    std::vector<uint8_t> content =
        calypsoPo->getFileBySfi(1)->getData()->getContent(1);
    const std::vector<uint8_t> contentV1 = content;

    calypsoPo->backupFiles();

    content = calypsoPo->getFileBySfi(1)->getData()->getContent(1);

    ASSERT_EQ(content, contentV1);

    const std::vector<uint8_t> vecV1 = {2, 2};
    calypsoPo->setContent(1, 1, vecV1);
    content = calypsoPo->getFileBySfi(1)->getData()->getContent(1);

    ASSERT_NE(content, contentV1);

    calypsoPo->restoreFiles();

    content = calypsoPo->getFileBySfi(1)->getData()->getContent(1);

    ASSERT_EQ(content, contentV1);
}
