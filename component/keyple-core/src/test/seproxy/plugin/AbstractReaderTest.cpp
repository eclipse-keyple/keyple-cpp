/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#include <mutex>
#include <thread>
#include <random>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "AbstractReader.h"

#include "AnswerToReset.h"
#include "ApduResponse.h"
#include "ByteArrayUtil.h"
#include "ChannelControl.h"
#include "ByteArrayUtil.h"
#include "IllegalArgumentException.h"
#include "MultiSeRequestProcessing.h"
#include "SeCommonProtocols.h"
#include "SelectionStatus.h"
#include "SeSelector.h"

using namespace testing;

using namespace keyple::common::exception;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::plugin;
using namespace keyple::core::util;

class AR_AbstractReaderMock : public AbstractReader {
public:
    AR_AbstractReaderMock(const std::string& pluginName, const std::string& name)
    : AbstractReader(pluginName, name) {}

    MOCK_METHOD((const std::map<const std::string, const std::string>&),
                getParameters,
                (),
                (const, override));

    MOCK_METHOD(void,
                setParameter,
                (const std::string&, const std::string&),
                (override));

    MOCK_METHOD(const TransmissionMode&,
                getTransmissionMode,
                (),
                (const, override));

    MOCK_METHOD(std::vector<std::shared_ptr<SeResponse>>,
                processSeRequests,
                (const std::vector<std::shared_ptr<SeRequest>>&,
                 const MultiSeRequestProcessing&, const ChannelControl&),
                (override));

    MOCK_METHOD(std::shared_ptr<SeResponse>,
                processSeRequest,
                (const std::shared_ptr<SeRequest>, const ChannelControl&),
                (override));

    MOCK_METHOD(bool,
                isSePresent,
                (),
                (override));

    MOCK_METHOD(void,
                addSeProtocolSetting,
                (std::shared_ptr<SeProtocol>, const std::string&),
                (override));

    MOCK_METHOD(void,
                setSeProtocolSetting,
                ((const std::map<std::shared_ptr<SeProtocol>, std::string>&)),
                (override));
    MOCK_METHOD(void,
                setParameters,
                ((const std::map<const std::string, const std::string>&)),
                (override));

    const std::string& getName() const override
    {
        return mName;
    }

private:
    const std::string mName = "AR_AbstractReaderMock";
};

static const std::string PLUGIN_NAME = "AbstractReaderTestP";
static const std::string READER_NAME = "AbstractReaderTest";

static std::shared_ptr<std::set<int>> getSuccessFulStatusCode()
{
    std::shared_ptr<std::set<int>> successfulStatusCodes =
        std::make_shared< std::set<int>>();

    int iData = std::stoi("9000", 0, 16);
    successfulStatusCodes->insert(iData);

    return successfulStatusCodes;
}

static std::shared_ptr<ApduRequest> getApduSample()
{
    std::vector<uint8_t> command = {0xFE, 0xDC, 0xBA, 0x98, 0x90, 0x05};

    std::shared_ptr<ApduRequest> request =
        std::make_shared<ApduRequest>(command, true, getSuccessFulStatusCode());
    request->setName("TEST");

    return request;
}

static std::vector<std::shared_ptr<ApduRequest>> getAapduLists()
{
    std::vector<std::shared_ptr<ApduRequest>> apdus;

    apdus.push_back(getApduSample());
    apdus.push_back(getApduSample());

    return apdus;
}

static std::shared_ptr<SeSelector> getSelector(
    std::shared_ptr<std::set<int>> selectionStatusCode)
{
    /*
     * We can use a fake AID here because it is not fully interpreted, the
     * purpose of this unit test is to verify the proper format of the request.
     */
    const std::vector<uint8_t> aid = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

    std::shared_ptr<SeSelector::AidSelector> aidSelector =
        SeSelector::AidSelector::builder()
            ->aidToSelect(aid)
            .build();

    for (const auto& code : *selectionStatusCode.get())
        aidSelector->addSuccessfulStatusCode(code);

    return SeSelector::builder()
            ->aidSelector(aidSelector)
            .atrFilter(nullptr)
            .seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
            .build();
}

static std::shared_ptr<SeRequest> getSeRequestSample()
{
    std::vector<std::shared_ptr<ApduRequest>> apdus = getAapduLists();
    std::shared_ptr<std::set<int>> selectionStatusCode =
        getSuccessFulStatusCode();

    return std::make_shared<SeRequest>(getSelector(selectionStatusCode), apdus);

}

static std::vector<std::shared_ptr<SeRequest>> getSeRequestSet()
{
    std::vector<std::shared_ptr<SeRequest>> set;
    set.push_back(getSeRequestSample());

    return set;
}

static std::shared_ptr<AnswerToReset> getAAtr()
{
    return std::make_shared<AnswerToReset>(
        ByteArrayUtil::fromHex("3B8F8001804F0CA000000306030001000000006A"));
}

static std::shared_ptr<ApduResponse> getAFCI()
{
    return std::make_shared<ApduResponse>(ByteArrayUtil::fromHex("9000"),
                                          nullptr);
}

static std::shared_ptr<ApduResponse> getSuccessfullResponse()
{
    return std::make_shared<ApduResponse>(
               ByteArrayUtil::fromHex("FEDCBA98 9000h"), nullptr);
}

static std::vector<std::shared_ptr<ApduResponse>> getAListOfAPDUs()
{
    std::vector<std::shared_ptr<ApduResponse>> apdus;

    apdus.push_back(getSuccessfullResponse());

    return apdus;
}

static std::shared_ptr<SeResponse> getASeResponse()
{
    std::vector<std::shared_ptr<ApduResponse>> apdus = getAListOfAPDUs();
    return std::make_shared<SeResponse>(
        true, true,
        std::make_shared<SelectionStatus>(getAAtr(), getAFCI(), true), apdus);
}

static std::vector<std::shared_ptr<SeResponse>> getSeResponses()
{
    std::vector<std::shared_ptr<SeResponse>> responses;

    responses.push_back(getASeResponse());

    return responses;
}

TEST(AbstractReaderTest, AbstractReader)
{
    AR_AbstractReaderMock r(PLUGIN_NAME, READER_NAME);

    ASSERT_EQ(r.getPluginName(), PLUGIN_NAME);
    ASSERT_EQ(r.getName(), READER_NAME);
}

TEST(AbstractReaderTest, transmitSet)
{
    AR_AbstractReaderMock r(PLUGIN_NAME, READER_NAME);

    std::vector<std::shared_ptr<SeRequest>> set = getSeRequestSet();

    EXPECT_CALL(r, processSeRequests(_, _, _))
        .Times(1)
        .WillOnce(Return(getSeResponses()));

    std::vector<std::shared_ptr<SeResponse>> responses =
        r.transmitSeRequests(set, MultiSeRequestProcessing::FIRST_MATCH,
                             ChannelControl::CLOSE_AFTER);


    ASSERT_NE(static_cast<int>(responses.size()), 0);
}

TEST(AbstractReaderTest, transmit_NullPtr1)
{
    AR_AbstractReaderMock r(PLUGIN_NAME, READER_NAME);

    r.transmitSeRequests(std::vector<std::shared_ptr<SeRequest>>{},
                         MultiSeRequestProcessing::FIRST_MATCH,
                         ChannelControl::CLOSE_AFTER);
}

TEST(AbstractReaderTest, transmit_NullPtr2)
{
    AR_AbstractReaderMock r(PLUGIN_NAME, READER_NAME);

    r.transmitSeRequests(std::vector<std::shared_ptr<SeRequest>>{},
                         MultiSeRequestProcessing::FIRST_MATCH,
                         ChannelControl::KEEP_OPEN);
}

TEST(AbstractReaderTest, transmit)
{
    AR_AbstractReaderMock r(PLUGIN_NAME, READER_NAME);

    std::shared_ptr<SeRequest> request = getSeRequestSample();

    EXPECT_CALL(r, processSeRequest(_, _))
        .Times(1)
        .WillOnce(Return(getASeResponse()));

    std::shared_ptr<SeResponse> response =
        r.transmitSeRequest(request, ChannelControl::CLOSE_AFTER);

    ASSERT_NE(response, nullptr);
}
