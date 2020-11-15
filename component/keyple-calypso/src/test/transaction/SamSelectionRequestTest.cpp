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

#include "SamSelectionRequest.h"

/* Calypso */
#include "CalypsoSam.h"
#include "CalypsoSamCommandException.h"
#include "SamRevision.h"
#include "SamSelector.h"

/* Core */
#include "ByteArrayUtil.h"
#include "SeCommonProtocols.h"
#include "SeResponse.h"

using namespace testing;

using namespace keyple::calypso::command::sam::exception;
using namespace keyple::calypso::transaction;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;

static const std::string REVISION = "C1";
static const std::string SN_STR = "11223344";
static const std::string ATR_STR = "3B3F9600805A4880" + REVISION + "205017" + SN_STR + "829000";
static const std::string UNLOCK_DATA_STR = "00112233445566778899AABBCCDDEEFF";

static const std::vector<uint8_t> SW1SW2_OK = ByteArrayUtil::fromHex("9000");
static const std::vector<uint8_t> SW1SW2_CONDITIONS_NOT_SATISFIED = ByteArrayUtil::fromHex("6985");
static const std::vector<uint8_t> SN = ByteArrayUtil::fromHex(SN_STR);
static const std::vector<uint8_t> ATR = ByteArrayUtil::fromHex(ATR_STR);
static const std::vector<uint8_t> UNLOCK_DATA = ByteArrayUtil::fromHex(UNLOCK_DATA_STR);

static const std::shared_ptr<ApduResponse> UNLOCK_APDU_RESPONSE_OK =
    std::make_shared<ApduResponse>(SW1SW2_OK, nullptr);
static const std::shared_ptr<ApduResponse> UNLOCK_APDU_RESPONSE_KO =
    std::make_shared<ApduResponse>(SW1SW2_CONDITIONS_NOT_SATISFIED, nullptr);

TEST(SamSelectionRequestTest, samSelectionRequest_parse)
{
    auto seSelector = SamSelector::builder()->samRevision(SamRevision::AUTO).build();
    auto samSelector = std::dynamic_pointer_cast<SamSelector>(seSelector);
    auto samSelectionRequest = std::make_shared<SamSelectionRequest>(samSelector);
    auto atr = std::make_shared<AnswerToReset>(ATR);
    auto selectionStatus = std::make_shared<SelectionStatus>(atr, nullptr, true);
    auto empty = std::vector<std::shared_ptr<ApduResponse>>{};
    auto seResponse = std::make_shared<SeResponse>(true, true, selectionStatus, empty);
    std::shared_ptr<AbstractMatchingSe> matchingSe = samSelectionRequest->parse(seResponse);
    auto calypsoSam = std::dynamic_pointer_cast<CalypsoSam>(matchingSe);

    /* Minimal checks on the CalypsoSam result */
    ASSERT_EQ(calypsoSam->getSamRevision(), SamRevision::C1);
    ASSERT_EQ(calypsoSam->getSerialNumber(), SN);
}

TEST(SamSelectionRequestTest, samSelectionRequest_unlock_ok)
{
    auto seSelector = SamSelector::builder()->samRevision(SamRevision::AUTO)
                                              .unlockData(UNLOCK_DATA)
                                              .build();
    auto samSelector = std::dynamic_pointer_cast<SamSelector>(seSelector);
    auto samSelectionRequest = std::make_shared<SamSelectionRequest>(samSelector);
    auto atr = std::make_shared<AnswerToReset>(ATR);
    auto selectionStatus = std::make_shared<SelectionStatus>(atr, nullptr, true);

    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
    apduResponses.push_back(UNLOCK_APDU_RESPONSE_OK);

    auto seResponse = std::make_shared<SeResponse>(true, true, selectionStatus, apduResponses);
    std::shared_ptr<AbstractMatchingSe> matchingSe = samSelectionRequest->parse(seResponse);
    auto calypsoSam = std::dynamic_pointer_cast<CalypsoSam>(matchingSe);

    /* Minimal checks on the CalypsoSam result */
    ASSERT_EQ(calypsoSam->getSamRevision(), SamRevision::C1);
    ASSERT_EQ(calypsoSam->getSerialNumber(), SN);
}

TEST(SamSelectionRequestTest, samSelectionRequest_unlock_ko)
{
    auto seSelector = SamSelector::builder()->samRevision(SamRevision::AUTO)
                                              .unlockData(UNLOCK_DATA)
                                              .build();
    auto samSelector = std::dynamic_pointer_cast<SamSelector>(seSelector);
    auto samSelectionRequest = std::make_shared<SamSelectionRequest>(samSelector);
    auto atr = std::make_shared<AnswerToReset>(ATR);
    auto selectionStatus = std::make_shared<SelectionStatus>(atr, nullptr, true);

    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
    apduResponses.push_back(UNLOCK_APDU_RESPONSE_KO);

    auto seResponse = std::make_shared<SeResponse>(true, true, selectionStatus, apduResponses);

    EXPECT_THROW(samSelectionRequest->parse(seResponse), KeypleSeCommandException);
}
