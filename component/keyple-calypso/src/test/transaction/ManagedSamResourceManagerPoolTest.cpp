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

    MOCK_METHOD((const std::map<const std::string, const std::string>&),
                getParameters,
                (),
                (const, override));

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
};

class MSRMP_ReaderPoolPluginMock : public ReaderPoolPlugin {
public:
    std::set<std::string> getReaderGroupReferences() override
    {
        return std::set<std::string>{};
    }

    std::shared_ptr<SeReader> allocateReader(const std::string& groupReference)
        override
    {
        (void)groupReference;

        return mSeReader;
    }

    void releaseReader(std::shared_ptr<SeReader> seReader) override
    {
        (void)seReader;
    }

    const std::string& getName() const override
    {
        return mName;
    }

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
    std::shared_ptr<MSRMP_ProxyReaderMock> mSeReader =
        std::make_shared<MSRMP_ProxyReaderMock>();

    const std::string mName = "seRader";
};

TEST(ManagedSamResourceManagerPoolTest, waitResources)
{
    /* Init */
    MSRMP_ReaderPoolPluginMock poolPlugin;
    SamResourceManagerPool srmSpy(poolPlugin, 1000, 10);
    long long start = System::currentTimeMillis();
    bool exceptionThrown = false;

    /* Test */
    std::shared_ptr<SeResource<CalypsoSam>> out = nullptr;
    try {
        out = srmSpy.allocateSamResource(AllocationMode::BLOCKING,
                                         SamIdentifier::builder()
                                             ->samRevision(SamRevision::AUTO)
                                             .serialNumber("any")
                                             .groupReference("any")
                                             .build());
    } catch (const CalypsoNoSamResourceAvailableException& e) {
        exceptionThrown = true;
    } catch (const KeypleAllocationReaderException& e) {
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

    /* Init SamResourceManagerPool with custom pool plugin */
    SamResourceManagerPool srmSpy(poolPlugin, 1000, 10);
    //doReturn(samResourceMock()).when(srmSpy).createSamResource(any(SeReader.class));

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
