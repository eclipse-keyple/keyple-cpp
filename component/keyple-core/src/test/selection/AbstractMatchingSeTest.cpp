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

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "AbstractMatchingSe.h"

#include "ApduResponse.h"
#include "ByteArrayUtil.h"
#include "IllegalStateException.h"
#include "SeResponse.h"

using namespace keyple::core::selection;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::util;

using namespace testing;

static const std::string FCI_REV31 =
    "6F238409315449432E49434131A516BF0C13C708 0000000011223344 " \
    "53070A3C23121410019000";
static const std::string ATR1 = "3B3F9600805A0080C120000012345678829000";

class MatchingSeMock : public AbstractMatchingSe {
public:
    MatchingSeMock(std::shared_ptr<SeResponse> selectionResponse,
                   TransmissionMode transmissionMode)
    : AbstractMatchingSe(selectionResponse, transmissionMode) {}
};

static std::vector<std::shared_ptr<ApduResponse>> empty;

static std::shared_ptr<SeResponse> seResponse =
    std::make_shared<SeResponse>(true, true, nullptr, empty);

TEST(AbstractMatchingSeTest, AbstractMatchingSe_Nullptr)
{
    MatchingSeMock matchingSe(nullptr, TransmissionMode::CONTACTLESS);
}

TEST(AbstractMatchingSeTest, AbstractMatchingSe)
{
    MatchingSeMock matchingSe(seResponse, TransmissionMode::CONTACTLESS);
}

TEST(AbstractMatchingSeTest, getTransmissionMode)
{
    std::shared_ptr<ApduResponse> fci =
        std::make_shared<ApduResponse>(ByteArrayUtil::fromHex(FCI_REV31),
                                       nullptr);
    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(nullptr, fci, true);
    std::shared_ptr<SeResponse> seResponse =
        std::make_shared<SeResponse>(true, false, selectionStatus, empty);

    MatchingSeMock matchingSe1(seResponse, TransmissionMode::CONTACTLESS);

    ASSERT_EQ(matchingSe1.getTransmissionMode(), TransmissionMode::CONTACTLESS);

    MatchingSeMock matchingSe2(seResponse, TransmissionMode::CONTACTS);

    ASSERT_EQ(matchingSe2.getTransmissionMode(), TransmissionMode::CONTACTS);
}

TEST(AbstractMatchingSeTest, hasAtr_true_HasFci_false_getAtrBytes)
{
    std::shared_ptr<AnswerToReset> answerToReset =
        std::make_shared<AnswerToReset>(ByteArrayUtil::fromHex(ATR1));
    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(answerToReset, nullptr, true);
    std::shared_ptr<SeResponse> seResponse =
        std::make_shared<SeResponse>(true, false, selectionStatus, empty);

    MatchingSeMock matchingSe(seResponse, TransmissionMode::CONTACTLESS);

    ASSERT_TRUE(matchingSe.hasAtr());
    ASSERT_FALSE(matchingSe.hasFci());
    ASSERT_EQ(matchingSe.getAtrBytes(), ByteArrayUtil::fromHex(ATR1));
    EXPECT_THROW(matchingSe.getFciBytes(), IllegalStateException);
}


TEST(AbstractMatchingSeTest, hasAtr_false_HasFci_true_getFciBytes)
{
    std::shared_ptr<ApduResponse> fci =
        std::make_shared<ApduResponse>(ByteArrayUtil::fromHex(FCI_REV31),
                                       nullptr);
    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(nullptr, fci, true);
    std::shared_ptr<SeResponse> seResponse =
        std::make_shared<SeResponse>(true, false, selectionStatus, empty);

    MatchingSeMock matchingSe(seResponse, TransmissionMode::CONTACTLESS);

    ASSERT_FALSE(matchingSe.hasAtr());
    ASSERT_TRUE(matchingSe.hasFci());
    ASSERT_EQ(matchingSe.getFciBytes(), ByteArrayUtil::fromHex(FCI_REV31));
    EXPECT_THROW(matchingSe.getAtrBytes(), IllegalStateException);
}

TEST(AbstractMatchingSeTest, hasAtr_true_HasFci_true_getAtrBytes_getFciBytes)
{
    std::shared_ptr<AnswerToReset> answerToReset =
        std::make_shared<AnswerToReset>(ByteArrayUtil::fromHex(ATR1));
    std::shared_ptr<ApduResponse> fci =
        std::make_shared<ApduResponse>(ByteArrayUtil::fromHex(FCI_REV31),
                                       nullptr);
    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(answerToReset, fci, true);
    std::shared_ptr<SeResponse> seResponse =
        std::make_shared<SeResponse>(true, false, selectionStatus, empty);

    MatchingSeMock matchingSe(seResponse, TransmissionMode::CONTACTLESS);

    ASSERT_TRUE(matchingSe.hasAtr());
    ASSERT_TRUE(matchingSe.hasFci());
    ASSERT_EQ(matchingSe.getAtrBytes(), ByteArrayUtil::fromHex(ATR1));
    ASSERT_EQ(matchingSe.getFciBytes(), ByteArrayUtil::fromHex(FCI_REV31));
}
