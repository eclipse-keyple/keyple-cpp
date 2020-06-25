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

#include "MatchingSelection.h"

#include "AbstractMatchingSe.h"
#include "AbstractSeSelectionRequest.h"
#include "ApduResponse.h"
#include "SeCommonProtocols.h"
#include "SeResponse.h"

using namespace keyple::core::selection;
using namespace keyple::core::seproxy::message;

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
        if (!seResponse)
            return nullptr;

        return std::make_shared<ApduResponseParser>(
                   seResponse->getApduResponses()[commandIndex]);
    }

protected:
    const std::shared_ptr<AbstractMatchingSe> parse(
        std::shared_ptr<SeResponse> seResponse) override
    {
        return std::make_shared<MatchingSeMock>(
                   seResponse,
                   seSelector->getSeProtocol()->getTransmissionMode(),
                   seSelector->getExtraInfo());
    }
};

static const std::vector<std::shared_ptr<ApduRequest>> emptyApduRequestVector;
static std::vector<std::shared_ptr<ApduResponse>> emptyApduResponseVector;
static const std::vector<uint8_t> aid = {0xa0, 0x00, 0x00, 0x00, 0x25};

static std::shared_ptr<SeSelector> seSelector =
    std::make_shared<SeSelector>(
        SeCommonProtocols::PROTOCOL_ISO14443_4,
        nullptr,
        std::make_shared<SeSelector::AidSelector>(
            std::make_shared<SeSelector::AidSelector::IsoAid>(
                aid),
            nullptr,
            SeSelector::AidSelector::FileOccurrence::NEXT,
            SeSelector::AidSelector::FileControlInformation::FCI),
        "extra selector info");

static std::shared_ptr<SeSelectionRequestMock> request =
    std::make_shared<SeSelectionRequestMock>(seSelector,
                                             emptyApduRequestVector);
static std::shared_ptr<SeResponse> response =
    std::make_shared<SeResponse>(true, true, nullptr, emptyApduResponseVector);

static std::shared_ptr<MatchingSeMock> se =
    std::make_shared<MatchingSeMock>(response, TransmissionMode::CONTACTLESS,
                                     "extrainfo");

TEST(MatchingSelectionTest, MatchingSelection)
{
    MatchingSelection selection_0(0, nullptr, nullptr, nullptr);
    MatchingSelection selection_1(1, request, nullptr, nullptr);
    MatchingSelection selection_2(2, nullptr, se,      nullptr);
    MatchingSelection selection_3(3, nullptr, nullptr, response);
    MatchingSelection selection_4(4, request, se,      nullptr);
    MatchingSelection selection_5(5, request, nullptr, response);
    MatchingSelection selection_6(6, nullptr, se,      response);
    MatchingSelection selection_7(7, request, se,      response);
}

TEST(MatchingSelectionTest, getMatchingSe_NullPtr)
{
    MatchingSelection selection_0(0, nullptr, nullptr, nullptr);

    ASSERT_EQ(selection_0.getMatchingSe(), nullptr);
}

TEST(MatchingSelectionTest, getMatchingSe)
{
    MatchingSelection selection_2(2, nullptr, se, nullptr);

    ASSERT_EQ(selection_2.getMatchingSe(), se);
}

TEST(MatchingSelectionTest, getResponseParser_NullPtr)
{
    MatchingSelection selection_0(0, nullptr, nullptr, nullptr);

    ASSERT_EQ(selection_0.getResponseParser(0), nullptr);
}

TEST(MatchingSelectionTest, getResponseParser)
{
    const std::vector<uint8_t> vect = {0x90, 0x00};
    std::vector<std::shared_ptr<ApduResponse>> okApduResponse = {
        std::make_shared<ApduResponse>(vect, nullptr)
    };
    std::shared_ptr<SeResponse> okResponse =
        std::make_shared<SeResponse>(true, true, nullptr, okApduResponse);

    MatchingSelection selection_7(7, request, se, okResponse);

    ASSERT_NE(selection_7.getResponseParser(0), nullptr);
}

TEST(MatchingSelectionTest, getExtraInfo_Empty)
{
    MatchingSelection selection_0(0, nullptr, nullptr, nullptr);

    ASSERT_EQ(selection_0.getExtraInfo(), "");
}

TEST(MatchingSelectionTest, getExtraInfo)
{
    MatchingSelection selection_1(1, request, nullptr, nullptr);

    ASSERT_EQ(selection_1.getExtraInfo(), "extra selector info");
}

TEST(MatchingSelectionTest, getSelectionIndex)
{
    MatchingSelection selection_0(0, nullptr, nullptr, nullptr);
    MatchingSelection selection_1(1, request, nullptr, nullptr);
    MatchingSelection selection_2(2, nullptr, se,      nullptr);
    MatchingSelection selection_3(3, nullptr, nullptr, response);
    MatchingSelection selection_4(4, request, se,      nullptr);
    MatchingSelection selection_5(5, request, nullptr, response);
    MatchingSelection selection_6(6, nullptr, se,      response);
    MatchingSelection selection_7(7, request, se,      response);

    ASSERT_EQ(selection_0.getSelectionIndex(), 0);
    ASSERT_EQ(selection_1.getSelectionIndex(), 1);
    ASSERT_EQ(selection_2.getSelectionIndex(), 2);
    ASSERT_EQ(selection_3.getSelectionIndex(), 3);
    ASSERT_EQ(selection_4.getSelectionIndex(), 4);
    ASSERT_EQ(selection_5.getSelectionIndex(), 5);
    ASSERT_EQ(selection_6.getSelectionIndex(), 6);
    ASSERT_EQ(selection_7.getSelectionIndex(), 7);
}
