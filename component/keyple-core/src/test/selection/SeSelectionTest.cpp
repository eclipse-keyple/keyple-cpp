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
#include "AbstractSeSelectionRequest.h"
#include "ApduResponse.h"
#include "ByteArrayUtil.h"
#include "DefaultSelectionsRequest.h"
#include "SeCommonProtocols.h"
#include "SelectionStatus.h"

using namespace keyple::core::selection;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::util;

using namespace testing;

class SS_MatchingSeMock : public AbstractMatchingSe {
public:
    SS_MatchingSeMock(const std::shared_ptr<SeResponse> selectionResponse,
                   const TransmissionMode& transmissionMode)
    : AbstractMatchingSe(selectionResponse, transmissionMode) {}
};

class SS_AbstractApduCommandBuilderMock : public AbstractApduCommandBuilder {
public:
    SS_AbstractApduCommandBuilderMock(
      const std::shared_ptr<SeCommand> commandRef,
      const std::shared_ptr<ApduRequest> request)
    : AbstractApduCommandBuilder(commandRef, request) {}
};

class SS_SeCommandMock : public SeCommand {
public:

    static const std::shared_ptr<SS_SeCommandMock> COMMAND_1;
    static const std::shared_ptr<SS_SeCommandMock> COMMAND_2;

    SS_SeCommandMock(const std::string name, const uint8_t instructionByte)
    : mName(name), mInstructionByte(instructionByte) {}

    const std::string& getName() const override
    {
        return mName;
    }

    uint8_t getInstructionByte() const override
    {
        return mInstructionByte;
    }

    MOCK_METHOD((const std::type_info&),
                getCommandBuilderClass,
                (),
                (const));

    MOCK_METHOD((const std::type_info&),
                getResponseParserClass,
                (),
                (const));

private:
    const std::string mName;
    const uint8_t mInstructionByte;
};

const std::shared_ptr<SS_SeCommandMock> SS_SeCommandMock::COMMAND_1 =
    std::make_shared<SS_SeCommandMock>(
        "COMMAND_1", static_cast<uint8_t>(0xc1));

const std::shared_ptr<SS_SeCommandMock> SS_SeCommandMock::COMMAND_2 =
    std::make_shared<SS_SeCommandMock>(
        "COMMAND_2", static_cast<uint8_t>(0xc2));

class SS_AbstractSeSelectionRequestMock
: public AbstractSeSelectionRequest<AbstractApduCommandBuilder> {
public:
    SS_AbstractSeSelectionRequestMock(
      std::shared_ptr<SeSelector> seSelector,
      std::vector<std::shared_ptr<AbstractApduCommandBuilder>> commandBuilders)
    : AbstractSeSelectionRequest<AbstractApduCommandBuilder>(seSelector)
    {
        for (const auto& command : commandBuilders)
            addCommandBuilder(command);
    }

    const std::shared_ptr<AbstractMatchingSe> parse(
        std::shared_ptr<SeResponse> seResponse) override
    {
        return std::make_shared<SS_MatchingSeMock>(
                   seResponse,
                   mSeSelector->getSeProtocol()->getTransmissionMode());
    }
};

std::unique_ptr<SeSelection> createSeSelection()
{
    std::unique_ptr<SeSelection> selection(new SeSelection());

    /* Create and add two selection cases */
    std::shared_ptr<SeSelector::AidSelector> aidSelector1 =
        SeSelector::AidSelector::builder()
            ->aidToSelect("AABBCCDDEE")
            .fileOccurrence(SeSelector::AidSelector::FileOccurrence::FIRST)
            .fileControlInformation(SeSelector::AidSelector
                                    ::FileControlInformation::FCI)
            .build();

    std::shared_ptr<SeSelector> seSelector1 =
        SeSelector::builder()
            ->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
            .aidSelector(aidSelector1)
            .build();

    /* APDU requests */
    std::vector<std::shared_ptr<AbstractApduCommandBuilder>> commandBuilders;
    commandBuilders.push_back(
        std::make_shared<SS_AbstractApduCommandBuilderMock>(
            SS_SeCommandMock::COMMAND_1,
            std::make_shared<ApduRequest>(
                "Apdu 001122334455", ByteArrayUtil::fromHex("001122334455"),
                false)));
    commandBuilders.push_back(
        std::make_shared<SS_AbstractApduCommandBuilderMock>(
            SS_SeCommandMock::COMMAND_1,
            std::make_shared<ApduRequest>(
                "Apdu 66778899AABB", ByteArrayUtil::fromHex("66778899AABB"),
                 true)));

    selection->prepareSelection(
        std::make_shared<SS_AbstractSeSelectionRequestMock>(seSelector1,
                                                            commandBuilders));

    std::shared_ptr<SeSelector::AidSelector> aidSelector2 =
        SeSelector::AidSelector::builder()
            ->aidToSelect("1122334455")
            .fileOccurrence(SeSelector::AidSelector::FileOccurrence::NEXT)
            .fileControlInformation(SeSelector::AidSelector
                                    ::FileControlInformation::FCP)
            .build();

    aidSelector2->addSuccessfulStatusCode(0x6283);

    std::shared_ptr<SeSelector> seSelector2 =
        SeSelector::builder()
            ->seProtocol(SeCommonProtocols::PROTOCOL_B_PRIME)
            .aidSelector(aidSelector2)
            .build();

    commandBuilders.clear();

    selection->prepareSelection(
        std::make_shared<SS_AbstractSeSelectionRequestMock>(seSelector2,
                                                            commandBuilders));

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
              selectionOperation->getMultiSeRequestProcessing());

    ASSERT_EQ(ChannelControl::KEEP_OPEN,
              selectionOperation->getChannelControl());

    /* Get the serequest set */
    std::vector<std::shared_ptr<SeRequest>> selectionSeRequests =
        selectionOperation->getSelectionSeRequests();

    ASSERT_EQ(2, static_cast<int>(selectionSeRequests.size()));

    /* Get the two se requests */
    std::shared_ptr<SeRequest> seRequest1 = *(selectionSeRequests.begin());
    std::shared_ptr<SeRequest> seRequest2 = *(++selectionSeRequests.begin());

    /* Check selectors */
    ASSERT_EQ("AABBCCDDEE",
              ByteArrayUtil::toHex(
                  seRequest1->getSeSelector()->getAidSelector()
                  ->getAidToSelect()));
    ASSERT_EQ("1122334455",
              ByteArrayUtil::toHex(
                  seRequest2->getSeSelector()->getAidSelector()
                  ->getAidToSelect()));

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
              static_cast<int>(seRequest2->getSeSelector()->getAidSelector()
              ->getSuccessfulSelectionStatusCodes()->size()));
    ASSERT_EQ(0x6283,
              *(*seRequest2->getSeSelector()->getAidSelector()
                 ->getSuccessfulSelectionStatusCodes().get()).begin());

    ASSERT_EQ(nullptr,seRequest1->getSeSelector()->getAtrFilter());

    /* This test does not make sense since getAtrFilter is nullptr... */
    //ASSERT_EQ(".*", seRequest2->getSeSelector()->getAtrFilter()->getAtrRegex());

    ASSERT_EQ(2, static_cast<int>(seRequest1->getApduRequests().size()));
    ASSERT_EQ(0, static_cast<int>(seRequest2->getApduRequests().size()));

    std::vector<std::shared_ptr<ApduRequest>> apduRequests =
        seRequest1->getApduRequests();

    ASSERT_EQ(apduRequests[0]->getBytes(),
              ByteArrayUtil::fromHex("001122334455"));
    ASSERT_EQ(apduRequests[1]->getBytes(),
              ByteArrayUtil::fromHex("66778899AABB"));

    ASSERT_FALSE(apduRequests[0]->isCase4());
    ASSERT_TRUE(apduRequests[1]->isCase4());
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
    std::vector<std::shared_ptr<SeResponse>> seResponses;
    std::shared_ptr<AbstractDefaultSelectionsResponse>
        defaultSelectionsResponse =
            std::make_shared<DefaultSelectionsResponse>(seResponses);

    std::shared_ptr<SelectionsResult> selectionsResult = nullptr;
    try {
        selectionsResult =
            seSelection->processDefaultSelection(defaultSelectionsResponse);
    } catch (const KeypleException& e) {
        FAIL() << "Exception raise: " << e.getMessage();
    }

    ASSERT_FALSE(selectionsResult->hasActiveSelection());
    ASSERT_EQ(
        static_cast<int>(selectionsResult->getMatchingSelections().size()), 0);
}

TEST(SeSelectionTest, SeSelection2_processDefaultSelection_NotMatching)
{
    /* Create a SeSelection */
    std::unique_ptr<SeSelection> seSelection = createSeSelection();

    /* Create a selection response */
    std::vector<std::shared_ptr<SeResponse>> seResponses;

    std::shared_ptr<ApduResponse> apduResponse =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex("CC 11223344 9999 00112233445566778899AABB" \
                                   "CCDDEEFF 00112233445566778899AABBCC 9000"),
            nullptr);

    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
    apduResponses.push_back(apduResponse);

    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(
            nullptr,
            std::make_shared<ApduResponse>(
                ByteArrayUtil::fromHex("001122334455669000"), nullptr),
            false);

    std::shared_ptr<SeResponse> seResponse =
        std::make_shared<SeResponse>(true, true, selectionStatus,
                                     apduResponses);

    seResponses.push_back(seResponse);

    std::shared_ptr<AbstractDefaultSelectionsResponse>
        defaultSelectionsResponse =
            std::make_shared<DefaultSelectionsResponse>(seResponses);

    /* Process the selection response with the SeSelection */
    std::shared_ptr<SelectionsResult> selectionsResult = nullptr;

    try {
        selectionsResult =
            seSelection->processDefaultSelection(defaultSelectionsResponse);
    } catch (const KeypleException& e) {
        FAIL() << "Exception raised: " << e.getMessage();
    }

    ASSERT_FALSE(selectionsResult->hasActiveSelection());

    try {
        selectionsResult->getActiveMatchingSe();
    } catch (const Exception& e) {
        ASSERT_TRUE(e.getMessage().find("No active Matching SE is available")
                    != std::string::npos);
    }
}

TEST(SeSelectionTest, SeSelection2_processDefaultSelection_Matching)
{
    /* Create a SeSelection */
    std::unique_ptr<SeSelection> seSelection = createSeSelection();

    /* Create a selection response */
    std::vector<std::shared_ptr<SeResponse>> seResponses;

    std::shared_ptr<ApduResponse> apduResponse =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex("CC 11223344 9999 00112233445566778899AABB" \
                                   "CCDDEEFF 00112233445566778899AABBCC 9000"),
            nullptr);

    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
    apduResponses.push_back(apduResponse);

    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(
            nullptr,
            std::make_shared<ApduResponse>(
                ByteArrayUtil::fromHex("001122334455669000"), nullptr),
            true);

     std::shared_ptr<SeResponse> seResponse =
        std::make_shared<SeResponse>(true, true, selectionStatus,
                                     apduResponses);

    seResponses.push_back(seResponse);

    std::shared_ptr<AbstractDefaultSelectionsResponse>
        defaultSelectionsResponse =
            std::make_shared<DefaultSelectionsResponse>(seResponses);

    /* Process the selection response with the SeSelection */
    std::shared_ptr<SelectionsResult> selectionsResult = nullptr;

    try {
        selectionsResult =
            seSelection->processDefaultSelection(defaultSelectionsResponse);
    } catch (const KeypleException& e) {
        FAIL() << "Exception raised: " << e.getMessage();
    }

    ASSERT_TRUE(selectionsResult->hasActiveSelection());
    ASSERT_NE(selectionsResult->getActiveMatchingSe(), nullptr);

    std::shared_ptr<AbstractMatchingSe> matchingSe =
        selectionsResult->getActiveMatchingSe();

    ASSERT_EQ(matchingSe->getTransmissionMode(), TransmissionMode::CONTACTLESS);
}
