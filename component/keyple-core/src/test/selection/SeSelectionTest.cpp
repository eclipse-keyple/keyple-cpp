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

#include "SeSelection.h"

#include "AbstractMatchingSe.h"
#include "ApduResponse.h"
#include "ByteArrayUtil.h"
#include "DefaultSelectionsRequest.h"
#include "MatchingSelection.h"
#include "SeCommonProtocols.h"
#include "SelectionStatus.h"

using namespace keyple::core::selection;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::util;

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
      std::shared_ptr<SeSelector> seSelector,
      const std::vector<std::shared_ptr<ApduRequest>>& apduRequestList)
    : AbstractSeSelectionRequest(seSelector)
    {
        for (const auto& apduRequest : apduRequestList)
            addApduRequest(apduRequest);
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

std::unique_ptr<SeSelection> createSeSelection()
{
    std::unique_ptr<SeSelection> selection(new SeSelection());

    /* Create and add two selection cases */
    std::shared_ptr<SeSelector> seSelector1 =
        std::make_shared<SeSelector>(
            SeCommonProtocols::PROTOCOL_ISO14443_4,
            nullptr,
            std::make_shared<SeSelector::AidSelector>(
                std::make_shared<SeSelector::AidSelector::IsoAid>("AABBCCDDEE"),
                nullptr, SeSelector::AidSelector::FileOccurrence::FIRST,
                SeSelector::AidSelector::FileControlInformation::FCI),
            "Se Selector #1");

    /* APDU requests */
    std::vector<std::shared_ptr<ApduRequest>> apduRequestList;
    apduRequestList.push_back(
        std::make_shared<ApduRequest>("Apdu 001122334455",
                                      ByteArrayUtil::fromHex("001122334455"),
                                      false));
    apduRequestList.push_back(
        std::make_shared<ApduRequest>("Apdu 66778899AABB",
                                      ByteArrayUtil::fromHex("66778899AABB"),
                                      true));

    selection->prepareSelection(
        std::make_shared<SeSelectionRequestMock>(seSelector1, apduRequestList));

    std::shared_ptr<std::set<int>> successfulSelectionStatusCodes =
        std::make_shared<std::set<int>>();
    successfulSelectionStatusCodes->insert(0x6283);

    std::shared_ptr<SeSelector> seSelector2 =
        std::make_shared<SeSelector>(
            SeCommonProtocols::PROTOCOL_B_PRIME,
            std::make_shared<SeSelector::AtrFilter>(".*"),
            std::make_shared<SeSelector::AidSelector>(
                std::make_shared<SeSelector::AidSelector::IsoAid>("1122334455"),
                successfulSelectionStatusCodes,
                SeSelector::AidSelector::FileOccurrence::NEXT,
               SeSelector::AidSelector::FileControlInformation::FCP),
            "Se Selector #2");

    apduRequestList.clear();

    selection->prepareSelection(
        std::make_shared<SeSelectionRequestMock>(seSelector2, apduRequestList));

    return selection;
}

TEST(SeSelectionTest, SeSelection1)
{
    /* Instantiate SeSelection using first constructor */

    SeSelection selection(MultiSeRequestProcessing::FIRST_MATCH,
                          ChannelControl::CLOSE_AFTER);
}

TEST(SeSelectionTest, SeSelection2)
{
    /* Instantiate SeSelection using second constructor */

    SeSelection selection;
}

TEST(SeSelectionTest, SeSelection2_prepareSelection)
{
    std::unique_ptr<SeSelection> seSelection = createSeSelection();

    /*
     * Let's check if the result is as expected (see
     * createSelectionSelectionSelection to have a look at the expected values)
     */

    /* Get the selection operation */
    std::shared_ptr<AbstractDefaultSelectionsRequest> selectionOperation =
        seSelection->getSelectionOperation();

    /* Check common flags */
    ASSERT_EQ(MultiSeRequestProcessing::FIRST_MATCH,
              (std::static_pointer_cast<DefaultSelectionsRequest>(
                  selectionOperation)->getMultiSeRequestProcessing()));

    ASSERT_EQ(ChannelControl::KEEP_OPEN,
              (std::static_pointer_cast<DefaultSelectionsRequest>(
                  selectionOperation)->getChannelControl()));

    /* Get the serequest set */
    std::vector<std::shared_ptr<SeRequest>> selectionSeRequestSet =
        (std::static_pointer_cast<DefaultSelectionsRequest>(selectionOperation))
            ->getSelectionSeRequestSet();

    ASSERT_EQ(2, selectionSeRequestSet.size());

    /* Get the two se requests */
    std::shared_ptr<SeRequest> seRequest1 = *(selectionSeRequestSet.begin());
    std::shared_ptr<SeRequest> seRequest2 = *(++selectionSeRequestSet.begin());

    /* Check selectors */
    ASSERT_EQ("AABBCCDDEE",
              ByteArrayUtil::toHex(
                  seRequest1->getSeSelector()->getAidSelector()
                  ->getAidToSelect()->getValue()));
    ASSERT_EQ("1122334455",
              ByteArrayUtil::toHex(
                  seRequest2->getSeSelector()->getAidSelector()
                  ->getAidToSelect()->getValue()));

    ASSERT_EQ(SeSelector::AidSelector::FileOccurrence::FIRST,
              seRequest1->getSeSelector()->getAidSelector()
              ->getFileOccurrence());
    ASSERT_EQ(SeSelector::AidSelector::FileOccurrence::NEXT,
              seRequest2->getSeSelector()->getAidSelector()
              ->getFileOccurrence());

    ASSERT_EQ(SeSelector::AidSelector::FileControlInformation::FCI,
              seRequest1->getSeSelector()->getAidSelector()
              ->getFileControlInformation());
    ASSERT_EQ(SeSelector::AidSelector::FileControlInformation::FCP,
              seRequest2->getSeSelector()->getAidSelector()
              ->getFileControlInformation());

    ASSERT_EQ(nullptr,
             seRequest1->getSeSelector()->getAidSelector()
             ->getSuccessfulSelectionStatusCodes());

    ASSERT_EQ(1,
              seRequest2->getSeSelector()->getAidSelector()
              ->getSuccessfulSelectionStatusCodes()->size());
    ASSERT_EQ(0x6283,
              *(*seRequest2->getSeSelector()->getAidSelector()
                 ->getSuccessfulSelectionStatusCodes().get()).begin());

    ASSERT_EQ(nullptr,seRequest1->getSeSelector()->getAtrFilter());
    ASSERT_EQ(".*", seRequest2->getSeSelector()->getAtrFilter()->getAtrRegex());

    ASSERT_EQ(2, seRequest1->getApduRequests().size());
    ASSERT_EQ(0, seRequest2->getApduRequests().size());

    std::vector<std::shared_ptr<ApduRequest>> apduRequests =
        seRequest1->getApduRequests();

    ASSERT_EQ(apduRequests[0]->getBytes(),
              ByteArrayUtil::fromHex("001122334455"));
    ASSERT_EQ(apduRequests[1]->getBytes(),
              ByteArrayUtil::fromHex("66778899AABB"));

    ASSERT_EQ(apduRequests[0]->isCase4(), false);
    ASSERT_EQ(apduRequests[1]->isCase4(), true);
}

TEST(SeSelectionTest, SeSelection2_processDefaultSelection_Null)
{
    SeSelection seSelection;

    ASSERT_EQ(seSelection.processDefaultSelection(nullptr), nullptr);
}

TEST(SeSelectionTest, SeSelection2_processDefaultSelection_Empty)
{
    /* Create a SeSelection */
    std::unique_ptr<SeSelection> seSelection = createSeSelection();

    /* Create a selection response */
    std::list<std::shared_ptr<SeResponse>> seResponseList;
    std::shared_ptr<AbstractDefaultSelectionsResponse>
        defaultSelectionsResponse =
            std::make_shared<DefaultSelectionsResponse>(seResponseList);

    std::shared_ptr<SelectionsResult> selectionsResult =
            seSelection->processDefaultSelection(defaultSelectionsResponse);

    ASSERT_FALSE(selectionsResult->hasActiveSelection());
    ASSERT_EQ(selectionsResult->getMatchingSelections().size(), 0);
}

TEST(SeSelectionTest, SeSelection2_processDefaultSelection_NotMatching)
{
    /* Create a SeSelection */
    std::unique_ptr<SeSelection> seSelection = createSeSelection();

    /* Create a selection response */
    std::list<std::shared_ptr<SeResponse>> seResponseList;

    std::shared_ptr<ApduResponse> apduResponse =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex("CC 11223344 9999 00112233445566778899AABB" \
                                   "CCDDEEFF 00112233445566778899AABBCC 9000"),
            nullptr);

    std::vector<std::shared_ptr<ApduResponse>> apduResponseList;
    apduResponseList.push_back(apduResponse);

    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(
            nullptr,
            std::make_shared<ApduResponse>(
                ByteArrayUtil::fromHex("001122334455669000"), nullptr),
            false);

    std::shared_ptr<SeResponse> seResponse =
        std::make_shared<SeResponse>(true, true, selectionStatus,
                                     apduResponseList);

    seResponseList.push_back(seResponse);

    std::shared_ptr<AbstractDefaultSelectionsResponse>
        defaultSelectionsResponse =
            std::make_shared<DefaultSelectionsResponse>(seResponseList);

    /* Process the selection response with the SeSelection */
    std::shared_ptr<SelectionsResult> selectionsResult =
            seSelection->processDefaultSelection(defaultSelectionsResponse);

    ASSERT_FALSE(selectionsResult->hasActiveSelection());
    ASSERT_EQ(selectionsResult->getActiveSelection(), nullptr);
}

TEST(SeSelectionTest, SeSelection2_processDefaultSelection_Matching)
{
    /* Create a SeSelection */
    std::unique_ptr<SeSelection> seSelection = createSeSelection();

    /* Create a selection response */
    std::list<std::shared_ptr<SeResponse>> seResponseList;

    std::shared_ptr<ApduResponse> apduResponse =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex("CC 11223344 9999 00112233445566778899AABB" \
                                   "CCDDEEFF 00112233445566778899AABBCC 9000"),
            nullptr);

    std::vector<std::shared_ptr<ApduResponse>> apduResponseList;
    apduResponseList.push_back(apduResponse);

    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(
            nullptr,
            std::make_shared<ApduResponse>(
                ByteArrayUtil::fromHex("001122334455669000"), nullptr),
            true);

     std::shared_ptr<SeResponse> seResponse =
        std::make_shared<SeResponse>(true, true, selectionStatus,
                                     apduResponseList);

    seResponseList.push_back(seResponse);

    std::shared_ptr<AbstractDefaultSelectionsResponse>
        defaultSelectionsResponse =
            std::make_shared<DefaultSelectionsResponse>(seResponseList);

    /* Process the selection response with the SeSelection */
    std::shared_ptr<SelectionsResult> selectionsResult =
            seSelection->processDefaultSelection(defaultSelectionsResponse);

    ASSERT_TRUE(selectionsResult->hasActiveSelection());
    ASSERT_NE(selectionsResult->getActiveSelection(), nullptr);

    std::shared_ptr<MatchingSelection> matchingSelection =
        selectionsResult->getActiveSelection();

    std::shared_ptr<MatchingSeMock> matchingSe =
        std::static_pointer_cast<MatchingSeMock>(
            matchingSelection->getMatchingSe());

    ASSERT_TRUE(matchingSe->isSelected());
    ASSERT_TRUE(matchingSe->getSelectionStatus()->hasMatched());
    ASSERT_EQ(matchingSe->getTransmissionMode(), TransmissionMode::CONTACTLESS);
    ASSERT_EQ(matchingSe->getSelectionExtraInfo(), "Se Selector #1");
    ASSERT_EQ(matchingSelection->getSelectionIndex(), 0);

    std::shared_ptr<AbstractApduResponseParser> responseParser =
        matchingSelection->getResponseParser(0);

    ASSERT_TRUE(responseParser->isSuccessful());
    ASSERT_EQ(matchingSelection->getExtraInfo(), "Se Selector #1");
}