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

#include "SamResourceManagerDefault.h"

/* Calypso */
#include "CalypsoNoSamResourceAvailableException.h"
#include "SamIdentifier.h"
#include "SamResourceManagerFactory.h"

/* Common */
#include "ConcurrentMap.h"
#include "System.h"

/* Core */
#include "ByteArrayUtil.h"
#include "ProxyReader.h"
#include "SelectionStatus.h"
#include "SeResponse.h"

using namespace testing;

using namespace keyple::calypso::exception;
using namespace keyple::calypso::transaction;
using namespace keyple::common;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::util;

using AllocationMode = SamResourceManager::AllocationMode;

static const std::string SAM_READER_NAME = "sam-reader-name";
static const std::string SAM_PLUGIN_NAME = "sam-plugin-name";
static const std::string ATR1 = "3B001122805A0180D002030411223344829000";

class MSRMD_ProxyReaderMock : public ProxyReader {
public:
    ~MSRMD_ProxyReaderMock() = default;

    MOCK_METHOD((const std::string&),
                getName,
                (),
                (const, override));

    MOCK_METHOD(bool,
                isSePresent,
                (),
                (override));

    MOCK_METHOD((std::vector<std::shared_ptr<SeResponse>>),
                transmitSeRequests,
                (const std::vector<std::shared_ptr<SeRequest>>& seRequests,
                 const MultiSeRequestProcessing& multiSeRequestProcessing,
                 const ChannelControl& channelControl),
                (override));

    MOCK_METHOD((const std::map<const std::string, const std::string>&),
                getParameters,
                (),
                (const, override));

    MOCK_METHOD(void,
                setParameter,
                (const std::string& key, const std::string&),
                (override));

    MOCK_METHOD(void,
                setParameters,
                ((const std::map<const std::string, const std::string>&)),
                (override));

    MOCK_METHOD(void,
                addSeProtocolSetting,
                (std::shared_ptr<SeProtocol>, const std::string&),
                (override));

    MOCK_METHOD(void,
                setSeProtocolSetting,
                ((const std::map<std::shared_ptr<SeProtocol>, std::string>&)),
                (override));

    MOCK_METHOD((const TransmissionMode&),
                getTransmissionMode,
                (),
                (const, override));

    MOCK_METHOD((std::shared_ptr<SeResponse>),
                transmitSeRequest,
                (std::shared_ptr<SeRequest>, ChannelControl),
                (override));
};

class MSRMD_ReaderPluginMock : public ReaderPlugin {
public:
    ~MSRMD_ReaderPluginMock() = default;

    MOCK_METHOD((const std::set<std::string>),
                getReaderNames,
                (),
                (const, override));

    MOCK_METHOD((ConcurrentMap<const std::string, std::shared_ptr<SeReader>>&),
                getReaders,
                (),
                (override));

    MOCK_METHOD((const std::shared_ptr<SeReader>),
                getReader,
                (const std::string&),
                (override));

    MOCK_METHOD((const std::map<const std::string, const std::string>&),
                getParameters,
                (),
                (const, override));

    MOCK_METHOD(void,
                setParameter,
                (const std::string&, const std::string&),
                (override));

    MOCK_METHOD(void,
                setParameters,
                ((const std::map<const std::string, const std::string>&)),
                (override));

    MOCK_METHOD(const std::string&,
                getName,
                (),
                (const, override));
};

std::shared_ptr<SelectionStatus> selectionStatus =
    std::make_shared<SelectionStatus>(
        std::make_shared<AnswerToReset>(ByteArrayUtil::fromHex(ATR1)),
        nullptr,
        true);

std::shared_ptr<SeResponse> seResponse =
    std::make_shared<SeResponse>(true,
                                 true,
                                 selectionStatus,
                                 std::vector<std::shared_ptr<ApduResponse>>{});

static std::shared_ptr<SeResponse> samSelectionSuccess()
{
    return seResponse;
}

/* Get a sam manager spy with a selectable sam */
static ConcurrentMap<const std::string, std::shared_ptr<SeReader>> readers;
static std::shared_ptr<MSRMD_ProxyReaderMock> reader;
static std::shared_ptr<MSRMD_ReaderPluginMock> plugin;

static std::shared_ptr<SamResourceManagerDefault> srmSpy(
    const std::string& samFilter)
{
    std::vector<std::shared_ptr<SeResponse>> selectionResponses;
    selectionResponses.push_back(samSelectionSuccess());

    /* Create a mock reader */
    reader = std::make_shared<MSRMD_ProxyReaderMock>();
    Mock::AllowLeak(reader.get());

    EXPECT_CALL(*reader, getName())
        .WillRepeatedly(ReturnRef(SAM_READER_NAME));
    EXPECT_CALL(*reader, isSePresent())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*reader, transmitSeRequests(_,_,_))
        .WillRepeatedly(Return(selectionResponses));
    if (samFilter == ".*") {
        EXPECT_CALL(*reader, addSeProtocolSetting(_,_))
            .WillRepeatedly(Return());
        EXPECT_CALL(*reader, setParameter(_,_))
            .WillRepeatedly(Return());
    }

    /* Create a list of mock readers */
    readers.clear();
    readers.insert({reader->getName(), reader});

    /* Create the mock plugin */
    plugin = std::make_shared<MSRMD_ReaderPluginMock>();
    Mock::AllowLeak(plugin.get());

    EXPECT_CALL(*plugin, getReaders())
        .WillRepeatedly(ReturnRef(readers));
    EXPECT_CALL(*plugin, getReader(SAM_READER_NAME))
        .WillRepeatedly(Return(reader));
    EXPECT_CALL(*plugin, getName())
        .WillRepeatedly(ReturnRef(SAM_PLUGIN_NAME));

    return std::make_shared<SamResourceManagerDefault>(
               plugin, samFilter, 1000, 10);
}

TEST(ManagedSamResourceManagerDefaultTest, waitResources)
{
    /* Init SamResourceManager with a not mathching filter */
    std::shared_ptr<SamResourceManagerDefault> spy =
        srmSpy("notMatchingFilter");
    unsigned long long start = System::currentTimeMillis();
    bool exceptionThrown = false;

    /* Test */
    std::shared_ptr<SeResource<CalypsoSam>> out = nullptr;
    try {
        out = spy->allocateSamResource(AllocationMode::BLOCKING,
                                       SamIdentifier::builder()
                                           ->samRevision(SamRevision::AUTO)
                                           .serialNumber("any")
                                           .groupReference("any")
                                           .build());

    } catch (const CalypsoNoSamResourceAvailableException& e) {
        exceptionThrown = true;
    }

    long long stop = System::currentTimeMillis();

    /* Assert an exception is thrown after MAX_BLOCKING_TIME */
    ASSERT_EQ(out, nullptr);
    ASSERT_TRUE(exceptionThrown);
    ASSERT_GT(stop - start, 1000);
}

TEST(ManagedSamResourceManagerDefaultTest, getSamResource)
{
    /* Init SamResourceManager with a mathching filter */
    std::shared_ptr<SamResourceManagerDefault> spy = srmSpy(".*");

    long long start = System::currentTimeMillis();

    /* Test */
    std::shared_ptr<SeResource<CalypsoSam>> out =
        spy->allocateSamResource(AllocationMode::BLOCKING,
                                 SamIdentifier::builder()
                                     ->samRevision(SamRevision::AUTO)
                                     .build());

    long long stop = System::currentTimeMillis();

    /* Assert results */
    ASSERT_NE(out, nullptr);
    ASSERT_LT(stop - start, 1000);
}
