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

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "SamResourceManagerPool.h"

/* Calypso */
#include "CalypsoNoSamResourceAvailableException.h"
#include "SamResourceManagerFactory.h"

/* Common */
#include "System.h"

/* Core */
#include "KeypleAllocationReaderException.h"
#include "ProxyReader.h"
#include "ReaderPoolPlugin.h"

using namespace testing;

using namespace keyple::calypso::exception;
using namespace keyple::calypso::transaction;
using namespace keyple::common;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;

using AllocationMode = SamResourceManager::AllocationMode;
using ManagedSamResource = SamResourceManager::ManagedSamResource;

class MSRMP_ProxyReaderMock : public ProxyReader {
public:
    const std::string& getName() const override
    {
        return mName;
    }

    std::shared_ptr<SeResponse> transmitSeRequest(
        std::shared_ptr<SeRequest> seRequest,
        const ChannelControl& channelControl) override
    {
        (void)seRequest;
        (void)channelControl;

        return nullptr;
    }

    std::vector<std::shared_ptr<SeResponse>> transmitSeRequests(
        const std::vector<std::shared_ptr<SeRequest>>& seRequests,
        const MultiSeRequestProcessing& multiSeRequestProcessing,
        const ChannelControl& channelControl) override
    {
        (void)seRequests;
        (void)multiSeRequestProcessing;
        (void)channelControl;

        return std::vector<std::shared_ptr<SeResponse>>(
            {
                std::make_shared<SeResponse>(
                    true,
                    true,
                    nullptr,
                    std::vector<std::shared_ptr<ApduResponse>>{}
                )
            }
        );
    }

    const std::map<const std::string, const std::string>& getParameters() const override
    {
        return mParameters;
    }

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

    MOCK_METHOD(void,
                setParameter,
                (const std::string& key, const std::string&),
                (override));

    MOCK_METHOD(void,
                setParameters,
                ((const std::map<const std::string, const std::string>&)),
                (override));

    MOCK_METHOD(bool,
                isSePresent,
                (),
                (override));

private:
    const std::string mName = "seRader";
    const std::map<const std::string, const std::string> mParameters;
};

class MSRMP_ReaderPoolPluginMock : public ReaderPoolPlugin {
public:
    std::set<std::string> getReaderGroupReferences() override
    {
        return std::set<std::string>{};
    }

    MOCK_METHOD(std::shared_ptr<SeReader>,
                allocateReader,
                (const std::string&),
                (override));

    void releaseReader(std::shared_ptr<SeReader> seReader) override
    {
        (void)seReader;
    }

    const std::string& getName() const override
    {
        return mName;
    }

    const std::map<const std::string, const std::string>& getParameters() const override
    {
        return mParameters;
    }

    MOCK_METHOD(void,
                setParameter,
                (const std::string&, const std::string&),
                (override));

    MOCK_METHOD(void,
                setParameters,
                ((const std::map<const std::string, const std::string>&)),
                (override));

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

private:
    const std::string mName = "seRader";
    const std::map<const std::string, const std::string> mParameters;
};

class MSRMP_SamResourceManagerPoolMock : public SamResourceManagerPool {
public:
    MSRMP_SamResourceManagerPoolMock(
        ReaderPoolPlugin& samReaderPoolPlugin,
        const int maxBlockingTime,
        const int sleepTime)
    : SamResourceManagerPool(samReaderPoolPlugin, maxBlockingTime, sleepTime) {}

    MOCK_METHOD((std::shared_ptr<ManagedSamResource>),
                createSamResource,
                (std::shared_ptr<SeReader> samReader),
                (override));
};

TEST(ManagedSamResourceManagerPoolTest, waitResources)
{
    /* Init */
    MSRMP_ReaderPoolPluginMock poolPlugin;
    SamResourceManagerPool srmSpy(poolPlugin, 1000, 10);
    long long start = System::currentTimeMillis();
    bool exceptionThrown = false;

    EXPECT_CALL(poolPlugin, allocateReader(_)).WillRepeatedly(Return(nullptr));

    /* Test */
    std::shared_ptr<SeResource<CalypsoSam>> out = nullptr;
    try {
        auto samIdentifier = SamIdentifier::builder()->samRevision(SamRevision::AUTO)
                                                      .serialNumber("any")
                                                      .groupReference("any")
                                                      .build();
        out = srmSpy.allocateSamResource(AllocationMode::BLOCKING, samIdentifier);
    } catch (const CalypsoNoSamResourceAvailableException& e) {
        (void)e;
        exceptionThrown = true;
    } catch (const KeypleAllocationReaderException& e) {
        (void)e;
        exceptionThrown = true;
    }

    long long stop = System::currentTimeMillis();

    /* Assert an exception is thrown after MAX_BLOCKING_TIME */
    ASSERT_TRUE(exceptionThrown);
    ASSERT_GT(stop - start, 1000);
}

TEST(ManagedSamResourceManagerPoolTest, getResource)
{
    /* Init plugin */
    MSRMP_ReaderPoolPluginMock poolPlugin;

    EXPECT_CALL(poolPlugin, allocateReader(_))
        .WillRepeatedly(Return(std::make_shared<MSRMP_ProxyReaderMock>()));

    /* Init SamResourceManagerPool with custom pool plugin */
    MSRMP_SamResourceManagerPoolMock srmSpy(poolPlugin, 1000, 10);

    EXPECT_CALL(srmSpy, createSamResource(_))
        .WillRepeatedly(Return(std::make_shared<ManagedSamResource>(nullptr, nullptr)));

    long long start = System::currentTimeMillis();

    /* Test */
    std::shared_ptr<SeResource<CalypsoSam>> out =
        srmSpy.allocateSamResource(AllocationMode::BLOCKING,
                                   SamIdentifier::builder()
                                       ->samRevision(SamRevision::AUTO)
                                       .serialNumber("any")
                                       .groupReference("any")
                                       .build());

    long long stop = System::currentTimeMillis();

    /* Assert results */
    ASSERT_NE(out, nullptr);
    ASSERT_LT(stop - start, 1000);
}
