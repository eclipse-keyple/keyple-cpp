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

#include "CalypsoSamTest.h"
#include "SeCommonProtocols.h"
#include "SamSelectionRequest.h"
#include "ChannelControl.h"
#include "CalypsoSam.h"
#include "AnswerToReset.h"
#include "SelectionStatus.h"
#include "ByteArrayUtil.h"
#include "SeResponse.h"

#include "LoggerFactory.h"

using namespace keyple::calypso::transaction;

namespace keyple {
namespace calypso {
namespace transaction {

using SamSelectionRequest  = keyple::calypso::transaction::SamSelectionRequest;
using SeSelector           = keyple::core::seproxy::SeSelector;
using AnswerToReset        = keyple::core::seproxy::message::AnswerToReset;
using SeResponse           = keyple::core::seproxy::message::SeResponse;
using SelectionStatus      = keyple::core::seproxy::message::SelectionStatus;
using ByteArrayUtils       = keyple::core::util::ByteArrayUtil;
using ContactlessProtocols = SeCommonProtocols;

void CalypsoSamTest::test_CalypsoSam_1()
{
    std::vector<uint8_t> szAnswerToReset =
        ByteArrayUtils::fromHex("3B001122805A0180D002030411223344829000");
    std::shared_ptr<AnswerToReset> lAnswerToReset =
        std::make_shared<AnswerToReset>(szAnswerToReset);
    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(lAnswerToReset, nullptr, true);
    std::vector<std::shared_ptr<ApduResponse>> lResp = {nullptr};
    std::shared_ptr<SeResponse> seReponse =
        std::make_shared<SeResponse>(true, true, selectionStatus, lResp);
    std::shared_ptr<CalypsoSam> calypsoSam =
        std::make_shared<CalypsoSam>(seReponse, TransmissionMode::NO_MODE, "");

    ASSERT_EQ(SamRevision::S1D.valueOf, calypsoSam->getSamRevision().valueOf);
    ASSERT_EQ(0x80, calypsoSam->getApplicationType());
    ASSERT_EQ(0xD0, calypsoSam->getApplicationSubType());
    ASSERT_EQ(0x01, calypsoSam->getPlatform());
    ASSERT_EQ(0x02, calypsoSam->getSoftwareIssuer());
    ASSERT_EQ(0x03, calypsoSam->getSoftwareVersion());
    ASSERT_EQ(0x04, calypsoSam->getSoftwareRevision());
    ASSERT_EQ("11223344", ByteArrayUtils::toHex(calypsoSam->getSerialNumber()));
}

void CalypsoSamTest::test_CalypsoSam_2()
{
    std::vector<uint8_t> szAnswerToReset =
        ByteArrayUtils::fromHex("3B001122805A0180D102030411223344829000");
    std::shared_ptr<AnswerToReset> lAnswerToReset =
        std::make_shared<AnswerToReset>(szAnswerToReset);
    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(lAnswerToReset, nullptr, true);

    std::vector<std::shared_ptr<ApduResponse>> lResp = {nullptr};
    std::shared_ptr<SeResponse> seReponse =
        std::make_shared<SeResponse>(true, true, selectionStatus, lResp);
    std::shared_ptr<CalypsoSam> calypsoSam =
        std::make_shared<CalypsoSam>(seReponse, TransmissionMode::NO_MODE, "");

    ASSERT_EQ(SamRevision::S1D.valueOf, calypsoSam->getSamRevision().valueOf);
    ASSERT_EQ(0xD1, calypsoSam->getApplicationSubType());
}

void CalypsoSamTest::test_CalypsoSam_3()
{
    std::vector<uint8_t> szAnswerToReset =
        ByteArrayUtils::fromHex("3B001122805A0180D202030411223344829000");
    std::shared_ptr<AnswerToReset> lAnswerToReset =
        std::make_shared<AnswerToReset>(szAnswerToReset);
    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(lAnswerToReset, nullptr, true);

    std::vector<std::shared_ptr<ApduResponse>> lResp = {nullptr};
    std::shared_ptr<SeResponse> seReponse =
        std::make_shared<SeResponse>(true, true, selectionStatus, lResp);
    std::shared_ptr<CalypsoSam> calypsoSam =
        std::make_shared<CalypsoSam>(seReponse, TransmissionMode::NO_MODE, "");

    ASSERT_EQ(SamRevision::S1D.valueOf, calypsoSam->getSamRevision().valueOf);
    ASSERT_EQ(0xD2, calypsoSam->getApplicationSubType());
}

void CalypsoSamTest::test_CalypsoSam_4()
{
    std::vector<uint8_t> szAnswerToReset =
        ByteArrayUtils::fromHex("3B001122805A0180C102030411223344829000");
    std::shared_ptr<AnswerToReset> lAnswerToReset =
        std::make_shared<AnswerToReset>(szAnswerToReset);
    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(lAnswerToReset, nullptr, true);

    std::vector<std::shared_ptr<ApduResponse>> lResp = {nullptr};
    std::shared_ptr<SeResponse> seReponse =
        std::make_shared<SeResponse>(true, true, selectionStatus, lResp);
    std::shared_ptr<CalypsoSam> calypsoSam =
        std::make_shared<CalypsoSam>(seReponse, TransmissionMode::NO_MODE, "");

    ASSERT_EQ(SamRevision::C1.valueOf, calypsoSam->getSamRevision().valueOf);
    ASSERT_EQ(0xC1, calypsoSam->getApplicationSubType());
}

void CalypsoSamTest::test_CalypsoSam_5()
{
    std::vector<uint8_t> szAnswerToReset =
        ByteArrayUtils::fromHex("3B001122805A0180E102030411223344829000");
    std::shared_ptr<AnswerToReset> lAnswerToReset =
        std::make_shared<AnswerToReset>(szAnswerToReset);
    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(lAnswerToReset, nullptr, true);

    std::vector<std::shared_ptr<ApduResponse>> lResp = {nullptr};
    std::shared_ptr<SeResponse> seReponse =
        std::make_shared<SeResponse>(true, true, selectionStatus, lResp);
    std::shared_ptr<CalypsoSam> calypsoSam =
        std::make_shared<CalypsoSam>(seReponse, TransmissionMode::NO_MODE, "");

    ASSERT_EQ(SamRevision::S1E.valueOf, calypsoSam->getSamRevision().valueOf);
    ASSERT_EQ(0xE1, calypsoSam->getApplicationSubType());
}

void CalypsoSamTest::test_CalypsoSam_6()
{
    std::vector<uint8_t> szAnswerToReset =
        ByteArrayUtils::fromHex("3B001122805A0180E202030411223344829000");
    std::shared_ptr<AnswerToReset> lAnswerToReset =
        std::make_shared<AnswerToReset>(szAnswerToReset);
    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(lAnswerToReset, nullptr, true);

    std::vector<std::shared_ptr<ApduResponse>> lResp = {nullptr};
    std::shared_ptr<SeResponse> seReponse =
        std::make_shared<SeResponse>(true, true, selectionStatus, lResp);
    try {
        std::shared_ptr<CalypsoSam> calypsoSam = std::make_shared<CalypsoSam>(
            seReponse, TransmissionMode::NO_MODE, "");
    } catch (...) {
    }
}

void CalypsoSamTest::test_CalypsoSam_7()
{
    std::vector<uint8_t> szAnswerToReset =
        ByteArrayUtils::fromHex("3B001122805A0180E202030411223344820000");
    std::shared_ptr<AnswerToReset> lAnswerToReset =
        std::make_shared<AnswerToReset>(szAnswerToReset);
    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(lAnswerToReset, nullptr, true);

    std::vector<std::shared_ptr<ApduResponse>> lResp = {nullptr};
    std::shared_ptr<SeResponse> seReponse =
        std::make_shared<SeResponse>(true, true, selectionStatus, lResp);

    try {
        /* code */
        std::shared_ptr<CalypsoSam> calypsoSam = std::make_shared<CalypsoSam>(
            seReponse, TransmissionMode::NO_MODE, "");
    } catch (...) {
    }
}

void CalypsoSamTest::test_CalypsoSam_8()
{
    std::vector<uint8_t> szAnswerToReset = ByteArrayUtils::fromHex("");
    std::shared_ptr<AnswerToReset> lAnswerToReset =
        std::make_shared<AnswerToReset>(szAnswerToReset);
    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(lAnswerToReset, nullptr, true);

    std::vector<std::shared_ptr<ApduResponse>> lResp = {nullptr};
    std::shared_ptr<SeResponse> seReponse =
        std::make_shared<SeResponse>(true, true, selectionStatus, lResp);
    try {
        std::shared_ptr<CalypsoSam> calypsoSam = std::make_shared<CalypsoSam>(
            seReponse, TransmissionMode::NO_MODE, "");
    } catch (...) {
    }
}
}
}
}

TEST(CalypsoSamTest, testA)
{
    std::shared_ptr<CalypsoSamTest> LocalTest =
        std::make_shared<CalypsoSamTest>();
    LocalTest->test_CalypsoSam_1();
}

TEST(CalypsoSamTest, testB)
{
    std::shared_ptr<CalypsoSamTest> LocalTest =
        std::make_shared<CalypsoSamTest>();
    LocalTest->test_CalypsoSam_2();
}

TEST(CalypsoSamTest, testC)
{
    std::shared_ptr<CalypsoSamTest> LocalTest =
        std::make_shared<CalypsoSamTest>();
    LocalTest->test_CalypsoSam_3();
}

TEST(CalypsoSamTest, testD)
{
    std::shared_ptr<CalypsoSamTest> LocalTest =
        std::make_shared<CalypsoSamTest>();
    LocalTest->test_CalypsoSam_4();
}

TEST(CalypsoSamTest, testE)
{
    std::shared_ptr<CalypsoSamTest> LocalTest =
        std::make_shared<CalypsoSamTest>();
    LocalTest->test_CalypsoSam_5();
}

TEST(CalypsoSamTest, testF)
{
    std::shared_ptr<CalypsoSamTest> LocalTest =
        std::make_shared<CalypsoSamTest>();
    LocalTest->test_CalypsoSam_6();
}

TEST(CalypsoSamTest, testG)
{
    std::shared_ptr<CalypsoSamTest> LocalTest =
        std::make_shared<CalypsoSamTest>();
    LocalTest->test_CalypsoSam_7();
}

TEST(CalypsoSamTest, testH)
{
    std::shared_ptr<CalypsoSamTest> LocalTest =
        std::make_shared<CalypsoSamTest>();
    LocalTest->test_CalypsoSam_8();
}
