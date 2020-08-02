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
#include "IllegalArgumentException.h"
#include "SeCommonProtocols.h"
#include "SmartSelectionReader.h"

using namespace testing;

using namespace keyple::core::util;
using namespace keyple::core::seproxy::plugin::local;
using namespace keyple::core::seproxy::protocol;

static const std::string PLUGIN_NAME = "AbstractLocalReaderTestP";
static const std::string READER_NAME = "AbstractLocalReaderTest";

static const std::string AID = "A000000291A000000191";
static const std::string ATR = "0000";

static const int STATUS_CODE_1 = 1;
static const int STATUS_CODE_2 = 2;

static const std::vector<uint8_t> RESP_SUCCESS = ByteArrayUtil::fromHex("90 00");
static const std::vector<uint8_t> RESP_FAIL = ByteArrayUtil::fromHex("00 00");

class ALRS_AbstractLocalReaderMock : public AbstractLocalReader {
public:
    ALRS_AbstractLocalReaderMock(
      const std::string& pluginName, const std::string& readerName)
    : AbstractLocalReader(pluginName, readerName) {}

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

    MOCK_METHOD((std::shared_ptr<SelectionStatus>),
                openLogicalChannel,
                (std::shared_ptr<SeSelector> seSelector),
                (override));

    MOCK_METHOD(bool,
                protocolFlagMatches,
                (const std::shared_ptr<SeProtocol>),
                (override));

    MOCK_METHOD(std::vector<uint8_t>,
                transmitApdu,
                (const std::vector<uint8_t>&),
                (override));

    std::map<std::shared_ptr<SeProtocol>, std::string>& getProtocolsMap()
    {
        return this->mProtocolsMap;
    }
};

class ALRS_SmartSelectionReaderMock
: public AbstractLocalReader, public SmartSelectionReader {
public:
    ALRS_SmartSelectionReaderMock(
      const std::string& pluginName, const std::string& readerName)
    : AbstractLocalReader(pluginName, readerName) {}

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

    MOCK_METHOD((std::shared_ptr<SelectionStatus>),
                openLogicalChannel,
                (std::shared_ptr<SeSelector> seSelector),
                (override));

    MOCK_METHOD(bool,
                protocolFlagMatches,
                (const std::shared_ptr<SeProtocol>),
                (override));

    MOCK_METHOD(std::vector<uint8_t>,
                transmitApdu,
                (const std::vector<uint8_t>&),
                (override));

    MOCK_METHOD((std::shared_ptr<ApduResponse>),
                openChannelForAid,
                (SeSelector::AidSelector&),
                (override));
};

static std::shared_ptr<SeSelector> getAtrSelector()
{
    std::shared_ptr<SeSelector::AtrFilter> atrFilter =
        std::make_shared<SeSelector::AtrFilter>(ATR);

    return SeSelector::builder()->atrFilter(atrFilter).build();
}

static std::shared_ptr<SeSelector> getAidSelector()
{
    std::shared_ptr<SeSelector::AidSelector> aidSelector =
        SeSelector::AidSelector::builder()->aidToSelect(AID).build();

    aidSelector->addSuccessfulStatusCode(STATUS_CODE_1);
    aidSelector->addSuccessfulStatusCode(STATUS_CODE_2);

    return SeSelector::builder()->aidSelector(aidSelector).build();
}

/** ==== Card presence management =========================================== */

TEST(AbstractLocalReaderSelectionTest, isSePresent_false)
{
    ALRS_AbstractLocalReaderMock r(PLUGIN_NAME, READER_NAME);

    EXPECT_CALL(r, checkSePresence())
        .Times(1)
        .WillOnce(Return(false));

    /* Test */
    ASSERT_FALSE(r.isSePresent());
}

TEST(AbstractLocalReaderSelectionTest, isSePresent_true)
{
    ALRS_AbstractLocalReaderMock r(PLUGIN_NAME, READER_NAME);

    EXPECT_CALL(r, checkSePresence())
        .Times(1)
        .WillOnce(Return(true));

    /* Test */
    ASSERT_TRUE(r.isSePresent());
}

/* Select by ATR */
TEST(AbstractLocalReaderSelectionTest, select_byAtr_success)
{
    ALRS_AbstractLocalReaderMock r(PLUGIN_NAME, READER_NAME);

    static const std::vector<uint8_t> atr = {0x00, 0x00};

    /* Mock ATR */
    EXPECT_CALL(r, getATR())
        .Times(1)
        .WillOnce(ReturnRef(atr));

    std::shared_ptr<SeSelector> seSelector = getAtrSelector();

    EXPECT_CALL(r, openLogicalChannel(seSelector))
        .Times(1)
        .WillOnce(Return(r.AbstractLocalReader::openLogicalChannel(seSelector)));

    std::shared_ptr<SelectionStatus> status = r.openLogicalChannel(seSelector);

    ASSERT_TRUE(status->hasMatched());

    /*
     * TODO OD : hard to understand why isLogicalChannelOpen is false after
     * openLogicalChannel. Channel is open only when processSeRequest.
     */
    ASSERT_FALSE(r.isLogicalChannelOpen());
}

TEST(AbstractLocalReaderSelectionTest, select_byAtr_fail)
{
    ALRS_AbstractLocalReaderMock r(PLUGIN_NAME, READER_NAME);

    static const std::vector<uint8_t> atr = {0x10, 0x00};

    /* Mock ATR to fail */
    EXPECT_CALL(r, getATR())
        .Times(1)
        .WillOnce(ReturnRef(atr));

    std::shared_ptr<SeSelector> seSelector = getAtrSelector();

    EXPECT_CALL(r, openLogicalChannel(seSelector))
        .Times(1)
        .WillOnce(
            Return(r.AbstractLocalReader::openLogicalChannel(seSelector)));

    std::shared_ptr<SelectionStatus> status = r.openLogicalChannel(seSelector);

    ASSERT_FALSE(status->hasMatched());
}

TEST(AbstractLocalReaderSelectionTest, select_byAtr_null)
{
    ALRS_AbstractLocalReaderMock r(PLUGIN_NAME, READER_NAME);

    static const std::vector<uint8_t> atr;

    /* Mock ATR to fail */
    EXPECT_CALL(r, getATR())
        .Times(1)
        .WillOnce(ReturnRef(atr));

    std::shared_ptr<SeSelector> seSelector = getAtrSelector();

    EXPECT_THROW(r.AbstractLocalReader::openLogicalChannel(seSelector),
                 KeypleReaderIOException);
}

/* Select by AID */
TEST(AbstractLocalReaderSelectionTest, select_byAid_success)
{
    ALRS_AbstractLocalReaderMock r(PLUGIN_NAME, READER_NAME);

    static const std::vector<uint8_t> atr;

    /* Mock ATR to fail */
    EXPECT_CALL(r, getATR())
        .Times(1)
        .WillOnce(ReturnRef(atr));

    EXPECT_CALL(r, transmitApdu(_))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(RESP_SUCCESS));

    std::shared_ptr<SeSelector> seSelector = getAidSelector();

    EXPECT_CALL(r, openLogicalChannel(seSelector))
        .Times(1)
        .WillOnce(
            Return(r.AbstractLocalReader::openLogicalChannel(seSelector)));

    std::shared_ptr<SelectionStatus> status = r.openLogicalChannel(seSelector);

    ASSERT_TRUE(status->hasMatched());
}

TEST(AbstractLocalReaderSelectionTest, select_byAid_fail)
{
    ALRS_AbstractLocalReaderMock r(PLUGIN_NAME, READER_NAME);

    static const std::vector<uint8_t> atr;

    /* Mock ATR to fail */
    EXPECT_CALL(r, getATR())
        .Times(1)
        .WillOnce(ReturnRef(atr));

    EXPECT_CALL(r, transmitApdu(_))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(RESP_FAIL));

    std::shared_ptr<SeSelector> seSelector = getAidSelector();

    EXPECT_CALL(r, openLogicalChannel(seSelector))
        .Times(1)
        .WillOnce(
            Return(r.AbstractLocalReader::openLogicalChannel(seSelector)));

    std::shared_ptr<SelectionStatus> status = r.openLogicalChannel(seSelector);

    ASSERT_FALSE(status->hasMatched());
}

/* Select by AID -- Smart Selection interface */
TEST(AbstractLocalReaderSelectionTest, select_bySmartAid_success)
{
    /* Use a SmartSelectionReader object */
    ALRS_SmartSelectionReaderMock r(PLUGIN_NAME, READER_NAME);

    static const std::vector<uint8_t> atr = {0x00, 0x00};

    EXPECT_CALL(r, openChannelForAid(_))
        .Times(1)
        .WillOnce(Return(std::make_shared<ApduResponse>(RESP_SUCCESS,
                                                        nullptr)));

    EXPECT_CALL(r, getATR())
        .Times(1)
        .WillOnce(ReturnRef(atr));

    EXPECT_CALL(r, transmitApdu(_))
        .Times(AtLeast(1))
        .WillOnce(Return(RESP_SUCCESS));

    std::shared_ptr<SeSelector> seSelector = getAidSelector();

    EXPECT_CALL(r, openLogicalChannel(seSelector))
        .Times(1)
        .WillOnce(
            Return(r.AbstractLocalReader::openLogicalChannel(seSelector)));

    std::shared_ptr<SelectionStatus> status = r.openLogicalChannel(seSelector);

    ASSERT_TRUE(status->hasMatched());
}

/*
 * Select by Atr and Aid
 * atr fail, aid success
 * TODO OD : check this, it seems that this case is no treated
 */
// TEST(AbstractLocalReaderSelectionTest, select_byAtrAndAid_success)
// {
//     ALRS_AbstractLocalReaderMock r(PLUGIN_NAME, READER_NAME);

//     static const std::vector<uint8_t> atr = {0x10, 0x00};

//     /* Mock ATR to fail */
//     EXPECT_CALL(r, getATR())
//         .Times(1)
//         .WillOnce(ReturnRef(atr));

//     /* Mock aid to success */
//     EXPECT_CALL(r, transmitApdu(_))
//         .Times(1)
//         .WillOnce(Return(RESP_SUCCESS));

//     std::shared_ptr<SeSelector::AtrFilter> atrFilter =
//         std::make_shared<SeSelector::AtrFilter>(ATR);

//     std::shared_ptr<SeSelector::AidSelector> aidSelector =
//         SeSelector::AidSelector::builder()->aidToSelect(AID).build();

//     aidSelector->addSuccessfulStatusCode(STATUS_CODE_1);
//     aidSelector->addSuccessfulStatusCode(STATUS_CODE_2);

//     /* Select both */
//     std::shared_ptr<SeSelector> seSelector =
//         SeSelector::builder()
//             ->atrFilter(atrFilter)
//             .aidSelector(aidSelector)
//             .build();

//     EXPECT_CALL(r, openLogicalChannel(seSelector))
//         .Times(1)
//         .WillOnce(
//             Return(r.AbstractLocalReader::openLogicalChannel(seSelector)));

//     std::shared_ptr<SelectionStatus> status = r.openLogicalChannel(seSelector);

//     ASSERT_TRUE(status->hasMatched());
// }

/*
 * Select by null null
 * TODO OD:is this normal ? check this
 */
TEST(AbstractLocalReaderSelectionTest, select_no_param)
{
    ALRS_AbstractLocalReaderMock r(PLUGIN_NAME, READER_NAME);

    static const std::vector<uint8_t> atr;

    std::shared_ptr<SeSelector> seSelector = SeSelector::builder()->build();

    /* Mock ATR to fail */
    EXPECT_CALL(r, getATR())
        .Times(1)
        .WillOnce(ReturnRef(atr));

    EXPECT_CALL(r, openLogicalChannel(seSelector))
        .Times(1)
        .WillOnce(
            Return(r.AbstractLocalReader::openLogicalChannel(seSelector)));

    std::shared_ptr<SelectionStatus> status = r.openLogicalChannel(seSelector);

    ASSERT_TRUE(status->hasMatched());
}

/* Open logical channel */
TEST(AbstractLocalReaderSelectionTest, open_channel_null)
{
    ALRS_AbstractLocalReaderMock r(PLUGIN_NAME, READER_NAME);

    EXPECT_THROW(r.openLogicalChannelAndSelect(nullptr),
                 IllegalArgumentException);
}

TEST(AbstractLocalReaderSelectionTest, open_logical_channel_success)
{
    ALRS_AbstractLocalReaderMock r(PLUGIN_NAME, READER_NAME);

    static const std::vector<uint8_t> atr = {0x00, 0x00};

    EXPECT_CALL(r, getATR())
        .Times(1)
        .WillOnce(ReturnRef(atr));

    EXPECT_CALL(r, isPhysicalChannelOpen())
        .Times(2)
        .WillRepeatedly(Return(true));

    std::shared_ptr<SeSelector> seSelector = getAtrSelector();

    EXPECT_CALL(r, openLogicalChannel(seSelector))
        .Times(1)
        .WillOnce(
            Return(r.AbstractLocalReader::openLogicalChannel(seSelector)));

    r.openLogicalChannelAndSelect(seSelector);
}

TEST(AbstractLocalReaderSelectionTest, open_channel_fail)
{
    ALRS_AbstractLocalReaderMock r(PLUGIN_NAME, READER_NAME);

    EXPECT_CALL(r, openPhysicalChannel())
        .Times(1);

    EXPECT_CALL(r, isPhysicalChannelOpen())
        .Times(2)
        .WillRepeatedly(Return(false)); // does not open

    std::shared_ptr<SeSelector> seSelector = getAtrSelector();

    EXPECT_CALL(r, openLogicalChannel(seSelector))
        .Times(0);

    EXPECT_THROW(r.openLogicalChannelAndSelect(seSelector),
                 KeypleReaderIOException);
}

/* Add Se Protocol Setting */
TEST(AbstractLocalReaderSelectionTest, add_SeProtocolSetting)
{
    ALRS_AbstractLocalReaderMock r(PLUGIN_NAME, READER_NAME);

    const std::string protocolRule = "any";

    r.addSeProtocolSetting(SeCommonProtocols::PROTOCOL_ISO14443_4,
                           protocolRule);

    ASSERT_EQ(r.getProtocolsMap()[SeCommonProtocols::PROTOCOL_ISO14443_4],
              protocolRule);
}

TEST(AbstractLocalReaderSelectionTest, set_SeProtocolSetting)
{
    ALRS_AbstractLocalReaderMock r(PLUGIN_NAME, READER_NAME);

    const std::string protocolRule = "any";

    std::map<std::shared_ptr<SeProtocol>, std::string> protocols;
    protocols.insert({SeCommonProtocols::PROTOCOL_ISO14443_4, protocolRule});
    r.setSeProtocolSetting(protocols);

    ASSERT_EQ(r.getProtocolsMap()[SeCommonProtocols::PROTOCOL_ISO14443_4],
              protocolRule);
}
