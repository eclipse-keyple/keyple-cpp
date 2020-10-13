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

#include "CalypsoSam.h"

/* Calypso */
#include "SamSelector.h"
#include "SeResponse.h"
#include "SamSelectionRequest.h"
#include "SelectionStatus.h"

/* Common */
#include "IllegalStateException.h"

/* Core */
#include "ByteArrayUtil.h"
#include "TransmissionMode.h"

using namespace testing;

using namespace keyple::calypso::transaction;
using namespace keyple::common;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;

static const std::string ATR1 = "3B001122805A0180D002030411223344829000";
static const std::string ATR2 = "3B001122805A0180D102030411223344829000";
static const std::string ATR3 = "3B001122805A0180D202030411223344829000";
static const std::string ATR4 = "3B001122805A0180C102030411223344829000";
static const std::string ATR5 = "3B001122805A0180E102030411223344829000";
static const std::string ATR6 = "3B001122805A0180E202030411223344829000";
static const std::string ATR7 = "3B001122805A0180E202030411223344820000";

/** basic CalypsoSam test: nominal ATR parsing */
TEST(CalypsoTestSam, test_CalypsoSam_1)
{
    std::shared_ptr<SamSelector> samSelector =
        std::dynamic_pointer_cast<SamSelector>(
            SamSelector::builder()->samRevision(SamRevision::AUTO).build());
    std::shared_ptr<SamSelectionRequest> samSelectionRequest =
        std::make_shared<SamSelectionRequest>(samSelector);
    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(
            std::make_shared<AnswerToReset>(ByteArrayUtil::fromHex(ATR1)),
            nullptr,
            true);

    CalypsoSam calypsoSam(
        std::make_shared<SeResponse>(
            true,
            true,
            selectionStatus,
            std::vector<std::shared_ptr<ApduResponse>>{}),
        TransmissionMode::CONTACTS);

    ASSERT_EQ(calypsoSam.getSamRevision(), SamRevision::S1D);
    ASSERT_EQ(calypsoSam.getApplicationType(), 0x80);
    ASSERT_EQ(calypsoSam.getApplicationSubType(), 0xD0);
    ASSERT_EQ(calypsoSam.getPlatform(), 0x01);
    ASSERT_EQ(calypsoSam.getSoftwareIssuer(), 0x02);
    ASSERT_EQ(calypsoSam.getSoftwareVersion(), 0x03);
    ASSERT_EQ(calypsoSam.getSoftwareRevision(), 0x04);
    ASSERT_EQ(ByteArrayUtil::toHex(calypsoSam.getSerialNumber()), "11223344");
}

/* S1D D1 */
TEST(CalypsoTestSam, test_CalypsoSam_2)
{
    std::shared_ptr<SamSelector> samSelector =
        std::dynamic_pointer_cast<SamSelector>(
            SamSelector::builder()->samRevision(SamRevision::AUTO).build());
    std::shared_ptr<SamSelectionRequest> samSelectionRequest =
        std::make_shared<SamSelectionRequest>(samSelector);
    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(
            std::make_shared<AnswerToReset>(ByteArrayUtil::fromHex(ATR2)),
            nullptr,
            true);

    CalypsoSam calypsoSam(
        std::make_shared<SeResponse>(
            true,
            true,
            selectionStatus,
            std::vector<std::shared_ptr<ApduResponse>>{}),
        TransmissionMode::CONTACTS);

    ASSERT_EQ(calypsoSam.getSamRevision(), SamRevision::S1D);
    ASSERT_EQ(calypsoSam.getApplicationSubType(), 0xD1);
}

/* S1D D2 */
TEST(CalypsoTestSam, test_CalypsoSam_3)
{
    std::shared_ptr<SamSelector> samSelector =
        std::dynamic_pointer_cast<SamSelector>(
            SamSelector::builder()->samRevision(SamRevision::AUTO).build());
    std::shared_ptr<SamSelectionRequest> samSelectionRequest =
        std::make_shared<SamSelectionRequest>(samSelector);
    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(
            std::make_shared<AnswerToReset>(ByteArrayUtil::fromHex(ATR3)),
            nullptr,
            true);

    CalypsoSam calypsoSam(
        std::make_shared<SeResponse>(
            true,
            true,
            selectionStatus,
            std::vector<std::shared_ptr<ApduResponse>>{}),
        TransmissionMode::CONTACTS);

    ASSERT_EQ(calypsoSam.getSamRevision(), SamRevision::S1D);
    ASSERT_EQ(calypsoSam.getApplicationSubType(), 0xD2);
}

/* C1 */
TEST(CalypsoTestSam, test_CalypsoSam_4)
{
    std::shared_ptr<SamSelector> samSelector =
        std::dynamic_pointer_cast<SamSelector>(
            SamSelector::builder()->samRevision(SamRevision::AUTO).build());
    std::shared_ptr<SamSelectionRequest> samSelectionRequest =
        std::make_shared<SamSelectionRequest>(samSelector);
    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(
            std::make_shared<AnswerToReset>(ByteArrayUtil::fromHex(ATR4)),
            nullptr,
            true);

    CalypsoSam calypsoSam(
        std::make_shared<SeResponse>(
            true,
            true,
            selectionStatus,
            std::vector<std::shared_ptr<ApduResponse>>{}),
        TransmissionMode::CONTACTS);

    ASSERT_EQ(calypsoSam.getSamRevision(), SamRevision::C1);
    ASSERT_EQ(calypsoSam.getApplicationSubType(), 0xC1);
}

/* E1 */
TEST(CalypsoTestSam, test_CalypsoSam_5)
{
    std::shared_ptr<SamSelector> samSelector =
        std::dynamic_pointer_cast<SamSelector>(
            SamSelector::builder()->samRevision(SamRevision::AUTO).build());
    std::shared_ptr<SamSelectionRequest> samSelectionRequest =
        std::make_shared<SamSelectionRequest>(samSelector);
    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(
            std::make_shared<AnswerToReset>(ByteArrayUtil::fromHex(ATR5)),
            nullptr,
            true);

    CalypsoSam calypsoSam(
        std::make_shared<SeResponse>(
            true,
            true,
            selectionStatus,
            std::vector<std::shared_ptr<ApduResponse>>{}),
        TransmissionMode::CONTACTS);

    ASSERT_EQ(calypsoSam.getSamRevision(), SamRevision::S1E);
    ASSERT_EQ(calypsoSam.getApplicationSubType(), 0xE1);
}

/* Unrecognized E2 */
TEST(CalypsoTestSam, test_CalypsoSam_6)
{
    std::shared_ptr<SamSelector> samSelector =
        std::dynamic_pointer_cast<SamSelector>(
            SamSelector::builder()->samRevision(SamRevision::AUTO).build());
    std::shared_ptr<SamSelectionRequest> samSelectionRequest =
        std::make_shared<SamSelectionRequest>(samSelector);
    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(
            std::make_shared<AnswerToReset>(ByteArrayUtil::fromHex(ATR6)),
            nullptr,
            true);

    EXPECT_THROW(
        std::make_shared<CalypsoSam>(
            std::make_shared<SeResponse>(
                true,
                true,
                selectionStatus,
                std::vector<std::shared_ptr<ApduResponse>>{}),
            TransmissionMode::CONTACTS),
        IllegalStateException);
}

/* Bad Calypso SAM ATR (0000 instead of 9000) */
TEST(CalypsoTestSam, test_CalypsoSam_7)
{
    std::shared_ptr<SamSelector> samSelector =
        std::dynamic_pointer_cast<SamSelector>(
            SamSelector::builder()->samRevision(SamRevision::AUTO).build());
    std::shared_ptr<SamSelectionRequest> samSelectionRequest =
        std::make_shared<SamSelectionRequest>(samSelector);
    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(
            std::make_shared<AnswerToReset>(ByteArrayUtil::fromHex(ATR7)),
            nullptr,
            true);

    EXPECT_THROW(
        std::make_shared<CalypsoSam>(
            std::make_shared<SeResponse>(
                true,
                true,
                selectionStatus,
                std::vector<std::shared_ptr<ApduResponse>>{}),
            TransmissionMode::CONTACTS),
        IllegalStateException);
}

/* Bad Calypso SAM ATR (empty array) */
TEST(CalypsoTestSam, test_CalypsoSam_8)
{
    std::shared_ptr<SamSelector> samSelector =
        std::dynamic_pointer_cast<SamSelector>(
            SamSelector::builder()->samRevision(SamRevision::AUTO).build());
    std::shared_ptr<SamSelectionRequest> samSelectionRequest =
        std::make_shared<SamSelectionRequest>(samSelector);
    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(
            std::make_shared<AnswerToReset>(ByteArrayUtil::fromHex("")),
            nullptr,
            true);

    EXPECT_THROW(
        std::make_shared<CalypsoSam>(
            std::make_shared<SeResponse>(
                true,
                true,
                selectionStatus,
                std::vector<std::shared_ptr<ApduResponse>>{}),
            TransmissionMode::CONTACTS),
        IllegalStateException);
}

