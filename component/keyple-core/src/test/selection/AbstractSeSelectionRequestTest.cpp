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

#include "AbstractApduCommandBuilder.h"
#include "AbstractApduResponseParser.h"
#include "AbstractMatchingSe.h"
#include "ByteArrayUtil.h"
#include "SeCommonProtocols.h"
#include "SeRequest.h"
#include "SeResponse.h"

using namespace keyple::core::command;
using namespace keyple::core::selection;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::util;

using namespace testing;

class ASSR_MatchingSeMock : public AbstractMatchingSe {
public:
    ASSR_MatchingSeMock(std::shared_ptr<SeResponse> selectionResponse,
                        TransmissionMode transmissionMode)
    : AbstractMatchingSe(selectionResponse, transmissionMode)
    {
    }
};

class ASSR_SeSelectionRequestMock
: public AbstractSeSelectionRequest<AbstractApduCommandBuilder> {
public:
    ASSR_SeSelectionRequestMock(std::shared_ptr<SeSelector> seSelector)
    : AbstractSeSelectionRequest(seSelector) {}

protected:
    MOCK_METHOD(const std::shared_ptr<AbstractMatchingSe>, parse,
                (std::shared_ptr<SeResponse> seResponse), (override));
};

class ASSR_CommandBuilderMock : public AbstractApduCommandBuilder {
public:
    ASSR_CommandBuilderMock(const std::shared_ptr<SeCommand> commandRef,
                       const std::shared_ptr<ApduRequest> request)
    : AbstractApduCommandBuilder(commandRef, request) {}

};

class ASSR_SeCommandMock : public SeCommand {
public:
    enum class commands {
        COMMAND1,
        COMMAND2
    };

    static const std::shared_ptr<ASSR_SeCommandMock> COMMAND1;
    static const std::shared_ptr<ASSR_SeCommandMock> COMMAND2;

    ASSR_SeCommandMock(const std::string& name, uint8_t instructionByte)
    : mName(name), mInstructionByte(instructionByte) {}

    uint8_t getInstructionByte() const override
    {
        return mInstructionByte;
    }

    const std::string& getName() const override
    {
        return mName;
    }

    MOCK_METHOD(const std::type_info&, getCommandBuilderClass, (),
                (const, override));

    MOCK_METHOD(const std::type_info&, getResponseParserClass, (),
                (const, override));


private:
    const std::string mName;
    const uint8_t mInstructionByte;
};

const std::shared_ptr<ASSR_SeCommandMock> ASSR_SeCommandMock::COMMAND1 =
    std::make_shared<ASSR_SeCommandMock>(
        "Command 1", static_cast<uint8_t>(0x01));
const std::shared_ptr<ASSR_SeCommandMock> ASSR_SeCommandMock::COMMAND2 =
    std::make_shared<ASSR_SeCommandMock>(
       "Command 2", static_cast<uint8_t>(0x02));

static const std::string AID = "112233445566";
static const std::string APDU1 = "00 11 2233 01 11";
static const std::string APDU2 = "00 11 2233 01 22";

static const std::vector<uint8_t> apdu = {0x11, 0x22, 0x33, 0x44, 0x55};

static std::shared_ptr<SeSelector> selector =
    SeSelector::builder()
        ->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
        .aidSelector(SeSelector::AidSelector::builder()->aidToSelect(AID)
            .build())
        .build();

TEST(AbstractSeSelectionRequestTest, AbstractSeSelectionRequest_NullPtr)
{
    ASSR_SeSelectionRequestMock request(nullptr);
}

TEST(AbstractSeSelectionRequestTest, AbstractSeSelectionRequest)
{
    ASSR_SeSelectionRequestMock request(selector);
}

TEST(AbstractSeSelectionRequestTest, getSelectionRequest)
{
    ASSR_SeSelectionRequestMock request(selector);

    ASSERT_NE(request.getSelectionRequest(), nullptr);
    ASSERT_EQ(request.getSelectionRequest()->getSeSelector(), selector);
}

TEST(AbstractSeSelectionRequestTest, getSeSelector_Nullptr)
{
    ASSR_SeSelectionRequestMock request(nullptr);

    ASSERT_EQ(request.getSeSelector(), nullptr);
}

TEST(AbstractSeSelectionRequestTest, getSeSelector)
{
    ASSR_SeSelectionRequestMock request(selector);

    ASSERT_EQ(request.getSeSelector(), selector);
}

TEST(AbstractSeSelectionRequestTest,
     addCommandBuilder_GetCommandBuilders_getSelectionRequest)
{
    ASSR_SeSelectionRequestMock request(selector);

    std::shared_ptr<ApduRequest> apduRequest1 =
        std::make_shared<ApduRequest>(ByteArrayUtil::fromHex(APDU1), true);
    std::shared_ptr<ApduRequest> apduRequest2 =
        std::make_shared<ApduRequest>(ByteArrayUtil::fromHex(APDU2), true);

    std::shared_ptr<ASSR_CommandBuilderMock> builder1 =
        std::make_shared<ASSR_CommandBuilderMock>(ASSR_SeCommandMock::COMMAND1,
                                             apduRequest1);
    std::shared_ptr<ASSR_CommandBuilderMock> builder2 =
        std::make_shared<ASSR_CommandBuilderMock>(ASSR_SeCommandMock::COMMAND1,
                                             apduRequest2);

    request.addCommandBuilder(builder1);
    request.addCommandBuilder(builder2);

    std::vector<std::shared_ptr<AbstractApduCommandBuilder>> builders =
        request.getCommandBuilders();

    ASSERT_EQ(builders[0], builder1);
    ASSERT_EQ(builders[1], builder2);

    std::shared_ptr<SeRequest> selectionRequest = request.getSelectionRequest();

    std::vector<std::shared_ptr<ApduRequest>> apduRequests =
        selectionRequest->getApduRequests();

    ASSERT_EQ(apduRequests[0], apduRequest1);
    ASSERT_EQ(apduRequests[1], apduRequest2);
}
