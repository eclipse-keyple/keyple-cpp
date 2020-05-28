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

#include "AbstractSeSelectionRequest.h"

#include "AbstractMatchingSe.h"
#include "SeCommonProtocols.h"
#include "SeRequest.h"
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
        if (!seResponse)
            return nullptr;

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

static const std::vector<uint8_t> apdu = {0x11, 0x22, 0x33, 0x44, 0x55};

static std::shared_ptr<SeSelector> seSelector =
    std::make_shared<SeSelector>(
        SeCommonProtocols::PROTOCOL_ISO14443_4,
        nullptr,
        std::make_shared<SeSelector::AidSelector>(
            std::make_shared<SeSelector::AidSelector::IsoAid>(
                apdu),
            nullptr,
            SeSelector::AidSelector::FileOccurrence::NEXT,
            SeSelector::AidSelector::FileControlInformation::FCI),
        "extra selector info");

TEST(AbstractSeSelectionRequestTest, AbstractSeSelectionRequest_NullPtr)
{
    SeSelectionRequestMock request(nullptr);
}

TEST(AbstractSeSelectionRequestTest, AbstractSeSelectionRequest)
{
    SeSelectionRequestMock request(seSelector);
}

TEST(AbstractSeSelectionRequestTest, getSelectionRequest)
{
    SeSelectionRequestMock request(seSelector);

    ASSERT_NE(request.getSelectionRequest(), nullptr);
    ASSERT_EQ(request.getSelectionRequest()->getSeSelector(), seSelector);
}

TEST(AbstractSeSelectionRequestTest, getSeSelector_Nullptr)
{
    SeSelectionRequestMock request(nullptr);

    ASSERT_EQ(request.getSeSelector(), nullptr);
}

TEST(AbstractSeSelectionRequestTest, getSeSelector)
{
    SeSelectionRequestMock request(seSelector);

    ASSERT_EQ(request.getSeSelector(), seSelector);
}
