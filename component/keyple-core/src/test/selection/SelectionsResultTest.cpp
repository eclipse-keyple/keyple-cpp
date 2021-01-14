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

#include "SelectionsResult.h"

#include "AbstractApduResponseParser.h"
#include "AbstractMatchingSe.h"
#include "AbstractSeSelectionRequest.h"
#include "IllegalStateException.h"
#include "SeCommonProtocols.h"
#include "SelectionStatus.h"
#include "SeResponse.h"

using namespace keyple::core::command;
using namespace keyple::core::selection;

using namespace testing;

class MatchingSeMock : public AbstractMatchingSe {
public:
    MatchingSeMock(std::shared_ptr<SeResponse> selectionResponse,
                   const TransmissionMode& transmissionMode)
    : AbstractMatchingSe(selectionResponse, transmissionMode) {}
};

static const std::string aid = "315449432E494341";
static std::vector<std::shared_ptr<ApduResponse>> emptyApduResponseVector;
static const std::vector<uint8_t> emptyUint8tVector;

static std::shared_ptr<SeSelector::AidSelector> aidSelector =
    SeSelector::AidSelector::builder()
        ->aidToSelect(aid)
        .fileControlInformation(SeSelector::AidSelector::FileControlInformation
                                ::FCI)
        .fileOccurrence(SeSelector::AidSelector::FileOccurrence::NEXT)
        .build();

static std::shared_ptr<SeResponse> seResponse =
        std::make_shared<SeResponse>(true, true, nullptr,
	                             emptyApduResponseVector);

static std::shared_ptr<SelectionStatus> matchedSelectionStatus =
    std::make_shared<SelectionStatus>(
        std::make_shared<AnswerToReset>(emptyUint8tVector),
        std::make_shared<ApduResponse>(emptyUint8tVector, nullptr), true);

static std::shared_ptr<SelectionStatus> unmatchedSelectionStatus =
    std::make_shared<SelectionStatus>(
        std::make_shared<AnswerToReset>(emptyUint8tVector),
        std::make_shared<ApduResponse>(emptyUint8tVector, nullptr), false);

static std::shared_ptr<SeResponse> matchedSeResponse =
    std::make_shared<SeResponse>(true, true, matchedSelectionStatus,
                                 emptyApduResponseVector);

static std::shared_ptr<SeResponse> unmatchedSeResponse =
    std::make_shared<SeResponse>(true, true, unmatchedSelectionStatus,
                                 emptyApduResponseVector);

static std::shared_ptr<MatchingSeMock> activeMatchingSe =
    std::make_shared<MatchingSeMock>(matchedSeResponse,
                                     TransmissionMode::CONTACTLESS);

static std::shared_ptr<MatchingSeMock> inactiveMatchingSe =
    std::make_shared<MatchingSeMock>(unmatchedSeResponse,
                                     TransmissionMode::CONTACTLESS);

TEST(SelectionsResultTest, SelectionsResult)
{
    SelectionsResult selectionsResult;
}

TEST(SelectionsResultTest, addMatchingSelection_NullPtr)
{
    SelectionsResult selectionResult;

    selectionResult.addMatchingSe(0, nullptr, false);

    ASSERT_EQ(selectionResult.getMatchingSe(0), nullptr);

    EXPECT_THROW(selectionResult.getActiveMatchingSe(), IllegalStateException);

    std::map<int, std::shared_ptr<AbstractMatchingSe>> matchingSelections =
        selectionResult.getMatchingSelections();

    ASSERT_EQ(static_cast<int>(matchingSelections.size()), 0);
}

TEST(SelectionsResultTest, addMatchingSelection_Inactive)
{
    SelectionsResult selectionResult;

    selectionResult.addMatchingSe(0, inactiveMatchingSe, false);

    ASSERT_EQ(selectionResult.getMatchingSe(0), inactiveMatchingSe);
    ASSERT_EQ(selectionResult.getMatchingSe(1), nullptr);

    std::map<int, std::shared_ptr<AbstractMatchingSe>> matchingSelections =
        selectionResult.getMatchingSelections();

    ASSERT_EQ(static_cast<int>(matchingSelections.size()), 1);
}

TEST(SelectionsResultTest, addMatchingSelection_Active)
{
    SelectionsResult selectionResult;

    selectionResult.addMatchingSe(1, activeMatchingSe, true);

    ASSERT_EQ(selectionResult.getMatchingSe(0), nullptr);
    ASSERT_EQ(selectionResult.getMatchingSe(1), activeMatchingSe);
    ASSERT_EQ(selectionResult.getActiveMatchingSe(), activeMatchingSe);

    std::map<int, std::shared_ptr<AbstractMatchingSe>> matchingSelections =
        selectionResult.getMatchingSelections();

    ASSERT_EQ(static_cast<int>(matchingSelections.size()), 1);
}

TEST(SelectionsResultTest, getActiveSelection_Empty)
{
    SelectionsResult selectionResult;

    EXPECT_THROW(selectionResult.getActiveMatchingSe(), IllegalStateException);
}

TEST(SelectionsResultTest, getActiveSelection_NotEmpty)
{
    SelectionsResult selectionResult;

    selectionResult.addMatchingSe(0, activeMatchingSe, true);

    ASSERT_EQ(selectionResult.getActiveMatchingSe(), activeMatchingSe);
}

TEST(SelectionsResultTest, getMatchingSelections_Empty)
{
    SelectionsResult selectionResult;

    std::map<int, std::shared_ptr<AbstractMatchingSe>> matchingSelections =
        selectionResult.getMatchingSelections();

    ASSERT_EQ(static_cast<int>(matchingSelections.size()), 0);
}

TEST(SelectionsResultTest, getMatchingSelections_Two)
{
    SelectionsResult selectionResult;

    selectionResult.addMatchingSe(0, inactiveMatchingSe, false);
    selectionResult.addMatchingSe(1, activeMatchingSe, true);

    std::map<int, std::shared_ptr<AbstractMatchingSe>> matchingSelections =
        selectionResult.getMatchingSelections();

    ASSERT_EQ(static_cast<int>(matchingSelections.size()), 2);
}

TEST(SelectionsResultTest, getMatchingSelection_NoMatching)
{
    SelectionsResult selectionResult;

    ASSERT_EQ(selectionResult.getMatchingSe(0), nullptr);
    ASSERT_EQ(selectionResult.getMatchingSe(1), nullptr);
    ASSERT_EQ(selectionResult.getMatchingSe(2), nullptr);
}

TEST(SelectionsResultTest, getMatchingSelection_Matching)
{
    SelectionsResult selectionResult;

    ASSERT_EQ(selectionResult.getMatchingSe(0), nullptr);
    ASSERT_EQ(selectionResult.getMatchingSe(1), nullptr);
    ASSERT_EQ(selectionResult.getMatchingSe(2), nullptr);

    selectionResult.addMatchingSe(0, inactiveMatchingSe, false);

    ASSERT_EQ(selectionResult.getMatchingSe(0), inactiveMatchingSe);
    ASSERT_EQ(selectionResult.getMatchingSe(1), nullptr);
    ASSERT_EQ(selectionResult.getMatchingSe(2), nullptr);

    selectionResult.addMatchingSe(1, activeMatchingSe, true);

    ASSERT_EQ(selectionResult.getMatchingSe(0), inactiveMatchingSe);
    ASSERT_EQ(selectionResult.getMatchingSe(1), activeMatchingSe);
    ASSERT_EQ(selectionResult.getMatchingSe(2), nullptr);
}

TEST(SelectionsResultTest, hasActivateSelection_Not)
{
    SelectionsResult selectionResult;

    ASSERT_FALSE(selectionResult.hasActiveSelection());

    selectionResult.addMatchingSe(0, inactiveMatchingSe, false);

    ASSERT_FALSE(selectionResult.hasActiveSelection());
}

TEST(SelectionsResultTest, hasActivateSelection)
{
    SelectionsResult selectionResult;

    selectionResult.addMatchingSe(0, activeMatchingSe, true);

    ASSERT_TRUE(selectionResult.hasActiveSelection());
}