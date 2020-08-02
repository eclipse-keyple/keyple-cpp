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

#include "AbstractLocalReader.h"

#include "ByteArrayUtil.h"
#include "SeCommonProtocols.h"

using namespace testing;

using namespace keyple::core::seproxy::plugin::local;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;

class ALRT_AbstractLocalReaderMock : public AbstractLocalReader {
public:
    ALRT_AbstractLocalReaderMock(
      const std::string& pluginName, const std::string& readerName)
    : AbstractLocalReader(pluginName, readerName)
    {
    }

    MOCK_METHOD((const std::map<const std::string, const std::string>&),
                getParameters,
                (),
                (const, override));

    MOCK_METHOD(void,
                setParameter,
                (const std::string&, const std::string&),
                (override));

    MOCK_METHOD((const TransmissionMode&),
                getTransmissionMode,
                (),
                (const, override));

    MOCK_METHOD(bool,
                checkSePresence,
                (),
                (override));

    MOCK_METHOD((const std::vector<uint8_t>&),
                getATR,
                (),
                (override));

    MOCK_METHOD(void,
                openPhysicalChannel,
                (),
                (override));

    MOCK_METHOD(void,
                closePhysicalChannel,
                (),
                (override));

    MOCK_METHOD(bool,
                isPhysicalChannelOpen,
                (),
                (override));

    MOCK_METHOD(bool,
                protocolFlagMatches,
                (const std::shared_ptr<SeProtocol>),
                (override));

    MOCK_METHOD(std::vector<uint8_t>,
                transmitApdu,
                (const std::vector<uint8_t>&),
                (override));
};

static const std::string PLUGIN_NAME = "AbsLocalReaderTransmitTestP";
static const std::string READER_NAME = "AbsLocalReaderTransmitTest";

static const std::string ATR = "0000";
static const std::string AID = "A000000291A000000191";

static const std::vector<uint8_t> ATR_vec = {0x00, 0x00};

static const int STATUS_CODE_1 = 1;
static const int STATUS_CODE_2 = 2;

static const std::vector<uint8_t> RESP_SUCCESS = ByteArrayUtil::fromHex("90 00");
static const std::vector<uint8_t> RESP_FAIL = ByteArrayUtil::fromHex("00 00");

static const std::vector<uint8_t> APDU_SUCCESS = ByteArrayUtil::fromHex("00 01");
static const std::vector<uint8_t> APDU_FAIL = ByteArrayUtil::fromHex("00 02");
static const std::vector<uint8_t> APDU_IOEXC = ByteArrayUtil::fromHex("00 03");

/*
 * Partial response set: multiple read records commands, one is not defined in
 * the StubSE
 *
 * An Exception will be thrown.
 */
static std::vector<std::shared_ptr<SeRequest>> getPartialRequestList(
    int scenario)
{
    std::shared_ptr<SeSelector::AtrFilter> atrFilter =
        std::make_shared<SeSelector::AtrFilter>(ATR);

    std::shared_ptr<SeSelector> selector =
        SeSelector::builder()
            ->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
            .atrFilter(atrFilter)
            .build();

    std::shared_ptr<SeSelector> failSelector =
        SeSelector::builder()
            ->seProtocol(SeCommonProtocols::PROTOCOL_MIFARE_UL)
            .atrFilter(atrFilter)
            .build();

    std::shared_ptr<ApduRequest> apduOK =
        std::make_shared<ApduRequest>(APDU_SUCCESS, false);
    std::shared_ptr<ApduRequest> apduKO =
        std::make_shared<ApduRequest>(APDU_IOEXC, false);

    std::vector<std::shared_ptr<ApduRequest>> poApduRequests1;
    poApduRequests1.push_back(apduOK);
    poApduRequests1.push_back(apduOK);
    poApduRequests1.push_back(apduOK);
    poApduRequests1.push_back(apduOK);

    std::vector<std::shared_ptr<ApduRequest>> poApduRequests2;
    poApduRequests2.push_back(apduOK);
    poApduRequests2.push_back(apduOK);
    poApduRequests2.push_back(apduOK);
    poApduRequests2.push_back(apduOK);

    std::vector<std::shared_ptr<ApduRequest>> poApduRequests3;
    poApduRequests3.push_back(apduOK);
    poApduRequests3.push_back(apduOK);
    poApduRequests3.push_back(apduKO);
    poApduRequests3.push_back(apduOK);

    std::shared_ptr<SeRequest> seRequest1 =
        std::make_shared<SeRequest>(selector, poApduRequests1);
    std::shared_ptr<SeRequest> seRequest2 =
        std::make_shared<SeRequest>(selector, poApduRequests2);

    std::shared_ptr<SeRequest> seRequest4 =
        std::make_shared<SeRequest>(failSelector, poApduRequests1);

    /* This SeRequest fails at step 3 */
    std::shared_ptr<SeRequest> seRequest3 =
        std::make_shared<SeRequest>(selector, poApduRequests3);

    std::vector<std::shared_ptr<SeRequest>> seRequests;
    switch (scenario) {
    case 0:
        /* 0 response Set */
        seRequests.push_back(seRequest3); // fails
        seRequests.push_back(seRequest1); // succeeds
        seRequests.push_back(seRequest2); // succeeds
        break;
    case 1:
        /* 1 response Set */
        seRequests.push_back(seRequest1); // succeeds
        seRequests.push_back(seRequest3); // fails
        seRequests.push_back(seRequest2); // succeeds
        break;
    case 2:
        /* 2 responses Set */
        seRequests.push_back(seRequest1); // succeeds
        seRequests.push_back(seRequest2); // succeeds
        seRequests.push_back(seRequest3); // fails
        break;
    case 3:
        /* 3 responses Set */
        seRequests.push_back(seRequest1); // succeeds
        seRequests.push_back(seRequest2); // succeeds
        seRequests.push_back(seRequest4); // selection fails
        break;
    case 4:
        /* 3 responses Set */
        seRequests.push_back(seRequest1); // succeeds
        break;
    default:
        break;
    }

    return seRequests;
}

static std::shared_ptr<SeSelector> getAidSelector()
{
    std::shared_ptr<SeSelector::AidSelector> aidSelector =
        SeSelector::AidSelector::builder()
            ->aidToSelect(AID)
            .build();

    aidSelector->addSuccessfulStatusCode(STATUS_CODE_1);
    aidSelector->addSuccessfulStatusCode(STATUS_CODE_2);

    return SeSelector::builder()
               ->aidSelector(aidSelector)
               .build();
}

static std::shared_ptr<SeRequest> getPartialRequest(int scenario)
{
    std::shared_ptr<SeSelector> aidSelector = getAidSelector();

    std::shared_ptr<ApduRequest> apduOK =
        std::make_shared<ApduRequest>(APDU_SUCCESS, false);
    std::shared_ptr<ApduRequest> apduKO =
        std::make_shared<ApduRequest>(APDU_IOEXC, false);

    std::vector<std::shared_ptr<ApduRequest>> poApduRequests;

    switch (scenario) {
    case 0:
        poApduRequests.push_back(apduKO); // fails
        poApduRequests.push_back(apduOK); // succeeds
        poApduRequests.push_back(apduOK); // succeeds
        break;
    case 1:
        poApduRequests.push_back(apduOK); // succeeds
        poApduRequests.push_back(apduKO); // fails
        poApduRequests.push_back(apduOK); // succeeds
        break;
    case 2:
        poApduRequests.push_back(apduOK); // succeeds
        poApduRequests.push_back(apduOK); // succeeds
        poApduRequests.push_back(apduKO); // fails
        break;
    case 3:
        poApduRequests.push_back(apduOK); // succeeds
        poApduRequests.push_back(apduOK); // succeeds
        poApduRequests.push_back(apduOK); // succeeds
        break;
    default:
        break;
    }

    return std::make_shared<SeRequest>(aidSelector, poApduRequests);
}

static inline void configure(ALRT_AbstractLocalReaderMock& r)
{
    /* Accept PROTOCOL_ISO14443_4 */
    EXPECT_CALL(r,
                protocolFlagMatches(
                    std::dynamic_pointer_cast<SeProtocol>(
                        SeCommonProtocols::PROTOCOL_ISO14443_4)))
        .WillRepeatedly(Return(true));

    /* Refuse PROTOCOL_MIFARE_UL */
    EXPECT_CALL(r,
                protocolFlagMatches(
                    std::dynamic_pointer_cast<SeProtocol>(
                        SeCommonProtocols::PROTOCOL_MIFARE_UL)))
        .WillRepeatedly(Return(false));

    /* Return atr */
    EXPECT_CALL(r, getATR())
        .WillRepeatedly(ReturnRef(ATR_vec));

    /* Success apdu */
    EXPECT_CALL(r, transmitApdu(APDU_SUCCESS))
        .WillRepeatedly(Return(RESP_SUCCESS));

    /* Fail apdu */
    EXPECT_CALL(r, transmitApdu(APDU_FAIL))
        .WillRepeatedly(Return(RESP_FAIL));

    /* I/O exception apdu */
    EXPECT_CALL(r, transmitApdu(APDU_IOEXC))
        .Times(AtMost(1))
        .WillOnce(Throw(KeypleReaderIOException("io exception transmitting")));

    /* Aid selection */
    EXPECT_CALL(r,
                transmitApdu(
                    ByteArrayUtil::fromHex(
                        "00 A4 04 00 0A A0 00 00 02 91 A0 00 00 01 91 00")))
        .Times(AtMost(1))
        .WillOnce(
            Return(
                ByteArrayUtil::fromHex(
                    "6F25840BA000000291A00000019102A516BF0C13C70800000000C0E1" \
                    "1FA653070A3C230C1410019000")));

    /* Physical channel is open */
    EXPECT_CALL(r, isPhysicalChannelOpen())
        .WillRepeatedly(Return(true));

    EXPECT_CALL(r, closePhysicalChannel())
        .WillRepeatedly(Return());
}

TEST(AbstractLocalReaderTransmitTest, transmitPartialResponseSet0)
{
    ALRT_AbstractLocalReaderMock reader(PLUGIN_NAME, READER_NAME);

    configure(reader);

    /* Init Request */
    const std::vector<std::shared_ptr<SeRequest>> seRequests =
        getPartialRequestList(0);

    try {
        /* Test */
        reader.processSeRequests(seRequests,
                                 MultiSeRequestProcessing::PROCESS_ALL,
                                 ChannelControl::CLOSE_AFTER);

        FAIL();

    } catch (const KeypleReaderIOException& ex) {
        ASSERT_EQ(static_cast<int>(ex.getSeResponses().size()), 1);
        ASSERT_EQ(static_cast<int>(ex.getSeResponses()[0]->getApduResponses()
                                       .size()), 2);
    }
}

TEST(AbstractLocalReaderTransmitTest, transmitPartialResponseSet1)
{
    ALRT_AbstractLocalReaderMock reader(PLUGIN_NAME, READER_NAME);

    configure(reader);

    const std::vector<std::shared_ptr<SeRequest>> seRequests =
        getPartialRequestList(1);

    try {
        /* Test */
        reader.processSeRequests(seRequests,
                                 MultiSeRequestProcessing::PROCESS_ALL,
                                 ChannelControl::CLOSE_AFTER);

        FAIL();

    } catch (const KeypleReaderIOException& ex) {
        ASSERT_EQ(static_cast<int>(ex.getSeResponses().size()), 2);
        ASSERT_EQ(static_cast<int>(ex.getSeResponses()[0]->getApduResponses()
                                       .size()), 4);
        ASSERT_EQ(static_cast<int>(ex.getSeResponses()[1]->getApduResponses()
                                       .size()), 2);
    }
}

TEST(AbstractLocalReaderTransmitTest, transmitPartialResponseSet2)
{
    ALRT_AbstractLocalReaderMock reader(PLUGIN_NAME, READER_NAME);

    configure(reader);

    const std::vector<std::shared_ptr<SeRequest>> seRequests =
        getPartialRequestList(2);

    try {
        /* Test */
        reader.processSeRequests(seRequests,
                                 MultiSeRequestProcessing::PROCESS_ALL,
                                 ChannelControl::CLOSE_AFTER);

        FAIL();

    } catch (const KeypleReaderIOException& ex) {
        ASSERT_EQ(static_cast<int>(ex.getSeResponses().size()), 3);
        ASSERT_EQ(static_cast<int>(ex.getSeResponses()[0]->getApduResponses()
                                       .size()), 4);
        ASSERT_EQ(static_cast<int>(ex.getSeResponses()[1]->getApduResponses()
                                       .size()), 4);
        ASSERT_EQ(static_cast<int>(ex.getSeResponses()[2]->getApduResponses()
                                       .size()), 2);
    }
}

TEST(AbstractLocalReaderTransmitTest, transmitPartialResponseSet3)
{
    ALRT_AbstractLocalReaderMock reader(PLUGIN_NAME, READER_NAME);

    configure(reader);

    const std::vector<std::shared_ptr<SeRequest>> seRequests =
        getPartialRequestList(3);

    try {
        /* Test */
        std::vector<std::shared_ptr<SeResponse>> responses =
            reader.processSeRequests(seRequests,
                                     MultiSeRequestProcessing::PROCESS_ALL,
                                     ChannelControl::CLOSE_AFTER);

        ASSERT_EQ(static_cast<int>(responses.size()), 3);
        ASSERT_EQ(static_cast<int>(responses[0]->getApduResponses().size()), 4);
        ASSERT_EQ(static_cast<int>(responses[1]->getApduResponses().size()), 4);
        ASSERT_EQ(responses[2], nullptr);

    } catch (const KeypleReaderException& ex) {
        (void)ex;
        FAIL();
    }
}

TEST(AbstractLocalReaderTransmitTest, transmitFirstMatch)
{
    ALRT_AbstractLocalReaderMock reader(PLUGIN_NAME, READER_NAME);

    configure(reader);

    const std::vector<std::shared_ptr<SeRequest>> seRequests =
        getPartialRequestList(3);

    try {
        /* Test */
        std::vector<std::shared_ptr<SeResponse>> responses =
            reader.processSeRequests(seRequests,
                                     MultiSeRequestProcessing::FIRST_MATCH,
                                     ChannelControl::CLOSE_AFTER);

        ASSERT_EQ(static_cast<int>(responses.size()), 1);
        ASSERT_EQ(static_cast<int>(responses[0]->getApduResponses().size()), 4);

    } catch (const KeypleReaderException& ex) {
        (void)ex;
        FAIL();
    }
}

TEST(AbstractLocalReaderTransmitTest, transmitPartialResponse0)
{
    ALRT_AbstractLocalReaderMock reader(PLUGIN_NAME, READER_NAME);

    configure(reader);

    std::shared_ptr<SeRequest> seRequest = getPartialRequest(0);

    try {
        /* Test */
        reader.processSeRequest(seRequest, ChannelControl::KEEP_OPEN);

        FAIL();

    } catch (const KeypleReaderIOException& ex) {
        ASSERT_EQ(static_cast<int>(ex.getSeResponse()->getApduResponses()
                                       .size()), 0);
    }
}

TEST(AbstractLocalReaderTransmitTest, transmitPartialResponse1)
{
    ALRT_AbstractLocalReaderMock reader(PLUGIN_NAME, READER_NAME);

    configure(reader);

    std::shared_ptr<SeRequest> seRequest = getPartialRequest(1);

    try {
        /* Test */
        reader.processSeRequest(seRequest, ChannelControl::CLOSE_AFTER);

        FAIL();

    } catch (const KeypleReaderIOException& ex) {
        ASSERT_EQ(static_cast<int>(ex.getSeResponse()->getApduResponses()
                                       .size()), 1);
    }
}

TEST(AbstractLocalReaderTransmitTest, transmitPartialResponse2)
{
    ALRT_AbstractLocalReaderMock reader(PLUGIN_NAME, READER_NAME);

    configure(reader);

    std::shared_ptr<SeRequest> seRequest = getPartialRequest(2);

    try {
        /* Test */
        reader.processSeRequest(seRequest, ChannelControl::CLOSE_AFTER);

        FAIL();

    } catch (const KeypleReaderIOException& ex) {
        ASSERT_EQ(static_cast<int>(ex.getSeResponse()->getApduResponses()
                                       .size()), 2);
    }
}

TEST(AbstractLocalReaderTransmitTest, transmitPartialResponse3)
{
    ALRT_AbstractLocalReaderMock reader(PLUGIN_NAME, READER_NAME);

    configure(reader);

    std::shared_ptr<SeRequest> seRequest = getPartialRequest(3);

    try {
        /* Test */
        std::shared_ptr<SeResponse> seResponse =
            reader.processSeRequest(seRequest, ChannelControl::CLOSE_AFTER);
        ASSERT_EQ(static_cast<int>(seResponse->getApduResponses().size()), 3);

    } catch (const KeypleReaderException& ex) {
        (void)ex;
        FAIL();
    }
}
