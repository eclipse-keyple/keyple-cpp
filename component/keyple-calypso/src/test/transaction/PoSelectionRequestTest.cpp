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

#include "PoSelectionRequest.h"

/* Calypso */
#include "CalypsoPo.h"
#include "ElementaryFile.h"
#include "FileData.h"
#include "PoSelector.h"
#include "SelectFileControl.h"

/* Core */
#include "ByteArrayUtil.h"
#include "SeCommonProtocols.h"

using namespace testing;

using namespace keyple::calypso;
using namespace keyple::calypso::transaction;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;

using AtrFilter = PoSelector::AtrFilter;
using FileType = FileHeader::FileType;
using InvalidatedPo = PoSelector::InvalidatedPo;

static const std::string ATR_VALUE = "3B8F8001805A08030400020011223344829000F3";
static const std::string FCI = "6F238409315449432E49434131A516BF0C13C70800000" \
                               "0001122334453070A3C2F051410019000";
static const std::string RECORD_CONTENT1 = "00112233445566778899";
static const std::string RECORD_CONTENT2 = "99887766554433221100";
static const std::string EF_SELECT_DATA = "85170804041D031F101010000303030000" \
                                          "0000000000002010";
static const std::string DF_SELECT_DATA = "8517000100000010100000010300000079" \
                                          "7979616770003F00";

static const std::shared_ptr<ApduResponse> SELECT_APPLICATION_RESPONSE =
    std::make_shared<ApduResponse>(ByteArrayUtil::fromHex(FCI), nullptr);
static const std::shared_ptr<ApduResponse> READ_REC_APDU_RESPONSE1 =
    std::make_shared<ApduResponse>(
        ByteArrayUtil::fromHex(RECORD_CONTENT1 + "9000"), nullptr);
static const std::shared_ptr<ApduResponse> READ_REC_APDU_RESPONSE2 =
    std::make_shared<ApduResponse>(
        ByteArrayUtil::fromHex(RECORD_CONTENT2 + "9000"), nullptr);
static const std::shared_ptr<ApduResponse> SELECT_DF_APDU_RESPONSE =
    std::make_shared<ApduResponse>(
        ByteArrayUtil::fromHex(DF_SELECT_DATA + "9000"), nullptr);
static const std::shared_ptr<ApduResponse> SELECT_EF_APDU_RESPONSE =
    std::make_shared<ApduResponse>(
        ByteArrayUtil::fromHex(EF_SELECT_DATA + "9000"), nullptr);

TEST(PoSelectionRequestTest, testPrepareReadRecordFile)
{
    const uint8_t sfi1 = 0x10;
    const uint8_t sfi2 = 0x11;
    const int recNumber = 1;

    PoSelectionRequest poSelectionRequest(
        std::dynamic_pointer_cast<PoSelector>(
            PoSelector::builder()
                ->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
                .atrFilter(std::make_shared<AtrFilter>(".*"))
                .invalidatedPo(InvalidatedPo::REJECT)
                .build()));

    poSelectionRequest.prepareReadRecordFile(sfi1, recNumber);
    poSelectionRequest.prepareReadRecordFile(sfi2, recNumber);

    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
    apduResponses.push_back(READ_REC_APDU_RESPONSE1);
    apduResponses.push_back(READ_REC_APDU_RESPONSE2);

    auto selectionStatus =
        std::make_shared<SelectionStatus>(
            std::make_shared<AnswerToReset>(ByteArrayUtil::fromHex(ATR_VALUE)),
            SELECT_APPLICATION_RESPONSE,
            true);

    auto seResponse = std::make_shared<SeResponse>(
                          true, true, selectionStatus, apduResponses);

    std::shared_ptr<AbstractMatchingSe> matchingSe =
        poSelectionRequest.parse(seResponse);
    auto calypsoPo = std::dynamic_pointer_cast<CalypsoPo>(matchingSe);

    std::shared_ptr<ElementaryFile> ef1 = calypsoPo->getFileBySfi(sfi1);
    std::shared_ptr<FileData> records1 = ef1->getData();
    const std::vector<uint8_t>& record1 = records1->getContent();

    ASSERT_EQ(record1, ByteArrayUtil::fromHex(RECORD_CONTENT1));

    std::shared_ptr<ElementaryFile> ef2 = calypsoPo->getFileBySfi(sfi2);
    std::shared_ptr<FileData> records2 = ef2->getData();
    const std::vector<uint8_t>& record2 = records2->getContent();

    ASSERT_EQ(record2, ByteArrayUtil::fromHex(RECORD_CONTENT2));
}

TEST(PoSelectionRequestTest, testPrepareSelectFile1_lid)
{
    const uint16_t lid1 = 0x2010;
    const uint16_t lid2 = 0x3F00;

    PoSelectionRequest poSelectionRequest(
        std::dynamic_pointer_cast<PoSelector>(
            PoSelector::builder()
                ->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
                .atrFilter(std::make_shared<AtrFilter>(".*"))
                .invalidatedPo(InvalidatedPo::REJECT)
                .build()));

    poSelectionRequest.prepareSelectFile(lid1);
    poSelectionRequest.prepareSelectFile(lid2);

    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
    apduResponses.push_back(SELECT_EF_APDU_RESPONSE);
    apduResponses.push_back(SELECT_DF_APDU_RESPONSE);

    auto selectionStatus =
        std::make_shared<SelectionStatus>(
            std::make_shared<AnswerToReset>(ByteArrayUtil::fromHex(ATR_VALUE)),
            SELECT_APPLICATION_RESPONSE,
            true);

    auto seResponse = std::make_shared<SeResponse>(
                          true, true, selectionStatus, apduResponses);

    std::shared_ptr<AbstractMatchingSe> matchingSe =
        poSelectionRequest.parse(seResponse);
    auto calypsoPo = std::dynamic_pointer_cast<CalypsoPo>(matchingSe);

    std::shared_ptr<ElementaryFile> ef = calypsoPo->getFileByLid(lid1);
    std::shared_ptr<FileHeader> efHeader = ef->getHeader();

    ASSERT_EQ(efHeader->getLid(), lid1);
    ASSERT_EQ(efHeader->getRecordSize(), 29);
    ASSERT_EQ(efHeader->getRecordsNumber(), 3);
    ASSERT_EQ(efHeader->getType(), FileType::CYCLIC);
    ASSERT_EQ(efHeader->getAccessConditions(),
              ByteArrayUtil::fromHex("1F101010"));
    ASSERT_EQ(efHeader->getKeyIndexes(), ByteArrayUtil::fromHex("00030303"));
    ASSERT_EQ(efHeader->getDfStatus(), 0);
    ASSERT_EQ(efHeader->getSharedReference(), 0);

    std::shared_ptr<DirectoryHeader> dfHeader = calypsoPo->getDirectoryHeader();

    ASSERT_EQ(dfHeader->getLid(), lid2);
    ASSERT_EQ(dfHeader->getAccessConditions(),
              ByteArrayUtil::fromHex("10100000"));
    ASSERT_EQ(dfHeader->getKeyIndexes(), ByteArrayUtil::fromHex("01030000"));
    ASSERT_EQ(dfHeader->getDfStatus(), 0);
}

TEST(PoSelectionRequestTest, testPrepareSelectFile_control)
{
    const uint16_t lid1 = 0x2010;
    const uint16_t lid2 = 0x3F00;

    PoSelectionRequest poSelectionRequest(
        std::dynamic_pointer_cast<PoSelector>(
            PoSelector::builder()
                ->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
                .atrFilter(std::make_shared<AtrFilter>(".*"))
                .invalidatedPo(InvalidatedPo::REJECT)
                .build()));

    poSelectionRequest.prepareSelectFile(SelectFileControl::CURRENT_DF);
    poSelectionRequest.prepareSelectFile(SelectFileControl::FIRST_EF);

    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
    apduResponses.push_back(SELECT_EF_APDU_RESPONSE);
    apduResponses.push_back(SELECT_DF_APDU_RESPONSE);

    auto selectionStatus =
        std::make_shared<SelectionStatus>(
            std::make_shared<AnswerToReset>(ByteArrayUtil::fromHex(ATR_VALUE)),
            SELECT_APPLICATION_RESPONSE,
            true);

    auto seResponse = std::make_shared<SeResponse>(
                          true, true, selectionStatus, apduResponses);

    std::shared_ptr<AbstractMatchingSe> matchingSe =
        poSelectionRequest.parse(seResponse);
    auto calypsoPo = std::dynamic_pointer_cast<CalypsoPo>(matchingSe);

    std::shared_ptr<ElementaryFile> ef = calypsoPo->getFileByLid(lid1);
    std::shared_ptr<FileHeader> efHeader = ef->getHeader();

    ASSERT_EQ(efHeader->getLid(), lid1);
    ASSERT_EQ(efHeader->getRecordSize(), 29);
    ASSERT_EQ(efHeader->getRecordsNumber(), 3);
    ASSERT_EQ(efHeader->getType(), FileType::CYCLIC);
    ASSERT_EQ(efHeader->getAccessConditions(),
              ByteArrayUtil::fromHex("1F101010"));
    ASSERT_EQ(efHeader->getKeyIndexes(), ByteArrayUtil::fromHex("00030303"));
    ASSERT_EQ(efHeader->getDfStatus(), 0);
    ASSERT_EQ(efHeader->getSharedReference(), 0);

    std::shared_ptr<DirectoryHeader> dfHeader = calypsoPo->getDirectoryHeader();

    ASSERT_EQ(dfHeader->getLid(), lid2);
    ASSERT_EQ(dfHeader->getAccessConditions(),
              ByteArrayUtil::fromHex("10100000"));
    ASSERT_EQ(dfHeader->getKeyIndexes(), ByteArrayUtil::fromHex("01030000"));
    ASSERT_EQ(dfHeader->getDfStatus(), 0);
}