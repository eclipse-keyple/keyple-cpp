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

#include "SelectionsResult.h"

#include "AbstractMatchingSe.h"
#include "AbstractSeSelectionRequest.h"
#include "MatchingSelection.h"
#include "SeCommonProtocols.h"
#include "SelectionStatus.h"
#include "SeResponse.h"

using namespace keyple::core::selection;

using namespace testing;

class MatchingSeMock : public AbstractMatchingSe {
public:
    MatchingSeMock(std::shared_ptr<SeResponse> selectionResponse,
                   TransmissionMode transmissionMode,
                   const std::string& extraInfo)
    : AbstractMatchingSe(selectionResponse, transmissionMode, extraInfo)
    {
    }
};

class SeSelectionRequestMock : public AbstractSeSelectionRequest {
public:
    SeSelectionRequestMock(
      std::shared_ptr<SeSelector> seSelector)
    : AbstractSeSelectionRequest(seSelector)
    {
    }

    class ApduResponseParser : public AbstractApduResponseParser {
    public:
        ApduResponseParser(std::shared_ptr<ApduResponse> response)
        : AbstractApduResponseParser(response)
        {
        }
    };

    std::shared_ptr<AbstractApduResponseParser> getCommandParser(
        std::shared_ptr<SeResponse> seResponse, int commandIndex) override
    {
        return std::make_shared<ApduResponseParser>(
                   seResponse->getApduResponses()[commandIndex]);
    }

protected:
    std::shared_ptr<AbstractMatchingSe> parse(
        std::shared_ptr<SeResponse> seResponse) override
    {
        return std::make_shared<MatchingSeMock>(
                   seResponse,
                   seSelector->getSeProtocol()->getTransmissionMode(),
                   seSelector->getExtraInfo());
    }
};

static const std::vector<uint8_t> aid =
    {0x31, 0x54, 0x49, 0x43, 0x2E, 0x49, 0x43, 0x41};
static std::vector<std::shared_ptr<ApduResponse>> emptyApduResponseVector;
static const std::vector<uint8_t> emptyUint8tVector;

static std::shared_ptr<SeSelectionRequestMock> selectionRequest =
    std::make_shared<SeSelectionRequestMock>(
        std::make_shared<SeSelector>(
            SeCommonProtocols::PROTOCOL_ISO14443_4, nullptr,
            std::make_shared<SeSelector::AidSelector>(
                std::make_shared<SeSelector::AidSelector::IsoAid>(aid),
                nullptr,
                SeSelector::AidSelector::FileOccurrence::NEXT,
                SeSelector::AidSelector::FileControlInformation::FCI),
            "Initial selection #2"));

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
                                     TransmissionMode::CONTACTLESS,
                                     "extrainfo");

/*
 * For a MatchingSe to be "inactive" either the SelectionStatus has not matched
 * or the SelectionResponse does not have its logical channel open. Let's use
 * the first option.
 */
static std::shared_ptr<MatchingSeMock> inactiveMatchingSe =
    std::make_shared<MatchingSeMock>(unmatchedSeResponse,
                                     TransmissionMode::CONTACTLESS,
                                     "extrainfo");

static std::shared_ptr<MatchingSelection> inactiveMatchingSelection =
    std::make_shared<MatchingSelection>(0, selectionRequest,
                                        inactiveMatchingSe, seResponse);

static std::shared_ptr<MatchingSelection> activeMatchingSelection =
    std::make_shared<MatchingSelection>(1, selectionRequest,
                                        activeMatchingSe, seResponse);

TEST(SelectionsResultTest, SelectionsResult)
{
    SelectionsResult selectionsResult;
}

TEST(SelectionsResultTest, addMatchingSelection_NullPtr)
{
    SelectionsResult selectionResult;

    selectionResult.addMatchingSelection(nullptr);

    ASSERT_EQ(selectionResult.getMatchingSelection(0), nullptr);
    ASSERT_EQ(selectionResult.getActiveSelection(), nullptr);

    std::vector<std::shared_ptr<MatchingSelection>> matchingSelections =
        selectionResult.getMatchingSelections();

    ASSERT_EQ(matchingSelections.size(), 0);
}

TEST(SelectionsResultTest, addMatchingSelection_Inactive)
{
    SelectionsResult selectionResult;

    selectionResult.addMatchingSelection(inactiveMatchingSelection);

    ASSERT_EQ(selectionResult.getMatchingSelection(0),
              inactiveMatchingSelection);
    ASSERT_EQ(selectionResult.getMatchingSelection(1), nullptr);
    ASSERT_EQ(selectionResult.getActiveSelection(), nullptr);

    std::vector<std::shared_ptr<MatchingSelection>> matchingSelections =
        selectionResult.getMatchingSelections();

    ASSERT_EQ(matchingSelections.size(), 1);
}

TEST(SelectionsResultTest, addMatchingSelection_Active)
{
    SelectionsResult selectionResult;

    selectionResult.addMatchingSelection(activeMatchingSelection);

    ASSERT_EQ(selectionResult.getMatchingSelection(0), nullptr);
    ASSERT_EQ(selectionResult.getMatchingSelection(1), activeMatchingSelection);
    ASSERT_EQ(selectionResult.getActiveSelection(), activeMatchingSelection);

    std::vector<std::shared_ptr<MatchingSelection>> matchingSelections =
        selectionResult.getMatchingSelections();

    ASSERT_EQ(matchingSelections.size(), 1);
}

TEST(SelectionsResultTest, getActiveSelection_Empty)
{
    SelectionsResult selectionResult;

    ASSERT_EQ(selectionResult.getActiveSelection(), nullptr);
}

TEST(SelectionsResultTest, getActiveSelection_NotEmpty)
{
    SelectionsResult selectionResult;

    selectionResult.addMatchingSelection(activeMatchingSelection);

    ASSERT_EQ(selectionResult.getActiveSelection(), activeMatchingSelection);
}

TEST(SelectionsResultTest, getMatchingSelections_Empty)
{
    SelectionsResult selectionResult;

    std::vector<std::shared_ptr<MatchingSelection>> matchingSelections =
        selectionResult.getMatchingSelections();

    ASSERT_EQ(matchingSelections.size(), 0);
}

TEST(SelectionsResultTest, getMatchingSelections_Two)
{
    SelectionsResult selectionResult;

    selectionResult.addMatchingSelection(inactiveMatchingSelection);
    selectionResult.addMatchingSelection(activeMatchingSelection);

    std::vector<std::shared_ptr<MatchingSelection>> matchingSelections =
        selectionResult.getMatchingSelections();

    ASSERT_EQ(matchingSelections.size(), 2);
}

TEST(SelectionsResultTest, getMatchingSelection_NoMatching)
{
    SelectionsResult selectionResult;

    ASSERT_EQ(selectionResult.getMatchingSelection(0), nullptr);
    ASSERT_EQ(selectionResult.getMatchingSelection(1), nullptr);
    ASSERT_EQ(selectionResult.getMatchingSelection(2), nullptr);
}

TEST(SelectionsResultTest, getMatchingSelection_Matching)
{
    SelectionsResult selectionResult;

    ASSERT_EQ(selectionResult.getMatchingSelection(0), nullptr);
    ASSERT_EQ(selectionResult.getMatchingSelection(1), nullptr);
    ASSERT_EQ(selectionResult.getMatchingSelection(2), nullptr);

    selectionResult.addMatchingSelection(inactiveMatchingSelection);

    ASSERT_EQ(selectionResult.getMatchingSelection(0),
              inactiveMatchingSelection);
    ASSERT_EQ(selectionResult.getMatchingSelection(1), nullptr);
    ASSERT_EQ(selectionResult.getMatchingSelection(2), nullptr);

    selectionResult.addMatchingSelection(activeMatchingSelection);

    ASSERT_EQ(selectionResult.getMatchingSelection(0),
              inactiveMatchingSelection);
    ASSERT_EQ(selectionResult.getMatchingSelection(1), activeMatchingSelection);
    ASSERT_EQ(selectionResult.getMatchingSelection(2), nullptr);
}

TEST(SelectionsResultTest, hasActivateSelection_Not)
{
    SelectionsResult selectionResult;

    ASSERT_FALSE(selectionResult.hasActiveSelection());

    selectionResult.addMatchingSelection(inactiveMatchingSelection);

    ASSERT_FALSE(selectionResult.hasActiveSelection());
}

TEST(SelectionsResultTest, hasActivateSelection)
{
    SelectionsResult selectionResult;

    selectionResult.addMatchingSelection(activeMatchingSelection);

    ASSERT_TRUE(selectionResult.hasActiveSelection());
}