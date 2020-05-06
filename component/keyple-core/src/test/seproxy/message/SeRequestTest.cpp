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

#include "SeRequestTest.h"

using namespace keyple::core::seproxy::message;

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using ChannelControl = keyple::core::seproxy::ChannelControl;
using SeSelector   = keyple::core::seproxy::SeSelector;
using SeProtocol     = keyple::core::seproxy::protocol::SeProtocol;
using ByteArrayUtils = keyple::core::util::ByteArrayUtil;

std::vector<std::shared_ptr<ApduRequest>> SeRequestTest::getApdus()
{
    return apdus;
}

void SeRequestTest::setUp()
{
    apdus               = getAapduLists();
    channelState        = ChannelControl::KEEP_OPEN;
    seProtocol          = getASeProtocol();
    selectionStatusCode = ApduRequestTest::getASuccessFulStatusCode();
    selector            = getSelector(selectionStatusCode);
    seRequest =
        std::make_shared<SeRequest>(selector, apdus, channelState, seProtocol);
}

void SeRequestTest::testSERequest()
{
    ASSERT_TRUE( seRequest == nullptr );
}

void SeRequestTest::getSelector()
{
    // test
    ASSERT_EQ(getSelector(selectionStatusCode)->toString(),
                 seRequest->getSeSelector()->toString());
}

void SeRequestTest::getApduRequests()
{
    // test
    seRequest = std::make_shared<SeRequest>(
        getSelector(nullptr), apdus, ChannelControl::CLOSE_AFTER );

    ASSERT_EQ(apdus, seRequest->getApduRequests());
}

void SeRequestTest::isKeepChannelOpen()
{
    ASSERT_TRUE(channelState);
}

void SeRequestTest::getProtocolFlag()
{
    seRequest = std::make_shared<SeRequest>(
        getSelector(nullptr), std::vector<std::shared_ptr<ApduRequest>>(),
        ChannelControl::KEEP_OPEN, seProtocol);
    ASSERT_EQ(seProtocol, seRequest->getProtocolFlag());
}

void SeRequestTest::getSuccessfulSelectionStatusCodes()
{
    seRequest = std::make_shared<SeRequest>(
        getSelector(selectionStatusCode),
        std::vector<std::shared_ptr<ApduRequest>>(), ChannelControl::KEEP_OPEN,
        SeCommonProtocols::PROTOCOL_B_PRIME);

    ASSERT_EQ(selectionStatusCode,
                      seRequest->getSeSelector()
                          ->getAidSelector()
                          ->getSuccessfulSelectionStatusCodes() );
}

void SeRequestTest::toStringNull()
{
    seRequest = std::make_shared<SeRequest>(nullptr, nullptr, nullptr, nullptr);
    ASSERT_TRUE(seRequest->toString() == "");
}

void SeRequestTest::constructor1()
{
    seRequest = std::make_shared<SeRequest>(getSelector(nullptr), apdus,
                                            channelState);
    ASSERT_EQ(getSelector(nullptr)->toString(),
                 seRequest->getSeSelector()->toString());
    ASSERT_EQ(channelState, ChannelControl::KEEP_OPEN );

    ASSERT_EQ(apdus, seRequest->getApduRequests());
    //
    ASSERT_EQ(Protocol::ANY, seRequest->getProtocolFlag());
    ASSERT_TRUE(seRequest->getSeSelector()
                   ->getAidSelector()
                   ->getSuccessfulSelectionStatusCodes());
}

void SeRequestTest::constructor2()
{
    seRequest = std::make_shared<SeRequest>(getSelector(nullptr), apdus,
                                            channelState, seProtocol);
    ASSERT_EQ(getSelector(nullptr)->toString(),
                 seRequest->getSeSelector()->toString());
    ASSERT_EQ(channelState == ChannelControl::KEEP_OPEN,
                 isKeepChannelOpen());

    ASSERT_EQ(apdus, seRequest->getApduRequests());

    ASSERT_EQ(seProtocol, getProtocolFlag());
    //
    ASSERT_TRUE(seRequest->getSeSelector()
                   ->getAidSelector()
                   ->getSuccessfulSelectionStatusCodes() == nullptr);
}

void SeRequestTest::constructor2b()
{
    seRequest = std::make_shared<SeRequest>(getSelector(selectionStatusCode),
                                            apdus, channelState);
    ASSERT_EQ(getSelector(selectionStatusCode)->toString(),
                 seRequest->getSeSelector()->toString());
    ASSERT_EQ(channelState == ChannelControl::KEEP_OPEN,
                 isKeepChannelOpen());

    ASSERT_EQ(apdus, seRequest->getApduRequests());

    ASSERT_EQ(Protocol::ANY, getProtocolFlag());
    //
    ASSERT_EQ(selectionStatusCode,
                      seRequest->getSeSelector()
                          ->getAidSelector()
                          ->getSuccessfulSelectionStatusCodes());
}

void SeRequestTest::constructor3()
{
    seRequest = std::make_shared<SeRequest>(getSelector(selectionStatusCode),
                                            apdus, channelState, seProtocol);
    ASSERT_EQ(getSelector(selectionStatusCode)->toString(),
                 seRequest->getSeSelector()->toString());
    ASSERT_EQ(ChannelControl == ChannelControl::KEEP_OPEN,
                 isKeepChannelOpen());

    ASSERT_EQ(apdus, seRequest->getApduRequests());

    ASSERT_EQ(seProtocol, seRequest->getProtocolFlag());

    ASSERT_EQ(selectionStatusCode,
                      seRequest->getSeSelector()
                          ->getAidSelector()
                          ->getSuccessfulSelectionStatusCodes());
}

std::shared_ptr<SeRequest> SeRequestTest::getSeRequestSample()
{

    std::vector<std::shared_ptr<ApduRequest>> apdus = getAapduLists();
    ChannelControl channelState                     = ChannelControl::KEEP_OPEN;
    std::shared_ptr<SeProtocol> seProtocol          = getASeProtocol();
    std::shared_ptr<std::set<int>> selectionStatusCode =
        ApduRequestTest::getASuccessFulStatusCode();

    return std::make_shared<SeRequest>(getSelector(selectionStatusCode), apdus,
                                       channelState, seProtocol);
}

std::vector<std::shared_ptr<ApduRequest>> SeRequestTest::getAapduLists()
{
    std::vector<std::shared_ptr<ApduRequest>> apdus;
    apdus = std::vector<std::shared_ptr<ApduRequest>>();
    apdus.push_back(ApduRequestTest::getApduSample());
    apdus.push_back(ApduRequestTest::getApduSample());
    return apdus;
}

std::shared_ptr<SeProtocol> SeRequestTest::getASeProtocol()
{
    return std::make_shared<SeProtocol>(SeCommonProtocols::PROTOCOL_B_PRIME);
}

std::shared_ptr<SeSelector> SeRequestTest::getSelector(
    std::shared_ptr<std::set<int>> selectionStatusCode)
{
    /*
     * We can use a fake AID here because it is not fully interpreted, the
     * purpose of this unit test is to verify the proper format of the request.
     */
    std::shared_ptr<SeSelector::AidSelector> aidSelector =
        std::make_shared<SeSelector::AidSelector>(
            ByteArrayUtils::fromHex("AABBCCDDEEFF"), selectionStatusCode);
    std::shared_ptr<SeSelector> seSelector =
        std::make_shared<SeSelector>(aidSelector, nullptr, nullptr);
    return seSelector;
}
}
}
}
}

TEST(SeRequestTest, testA)
{
    std::shared_ptr<SeRequestTest> LocalTest =
        std::make_shared<SeRequestTest>();
    LocalTest->setUp();
    LocalTest->testSERequest();
}

TEST(SeRequestTest, testB)
{
    std::shared_ptr<SeRequestTest> LocalTest =
        std::make_shared<SeRequestTest>();
    LocalTest->setUp();
    LocalTest->getSelector();
}

TEST(SeRequestTest, testC)
{
    std::shared_ptr<SeRequestTest> LocalTest =
        std::make_shared<SeRequestTest>();
    LocalTest->setUp();
    LocalTest->getApduRequests();
}

TEST(SeRequestTest, testD)
{
    std::shared_ptr<SeRequestTest> LocalTest =
        std::make_shared<SeRequestTest>();
    LocalTest->setUp();
    LocalTest->isKeepChannelOpen();
}

TEST(SeRequestTest, testE)
{
    std::shared_ptr<SeRequestTest> LocalTest =
        std::make_shared<SeRequestTest>();
    LocalTest->setUp();
    LocalTest->getProtocolFlag();
}

TEST(SeRequestTest, testF)
{
    std::shared_ptr<SeRequestTest> LocalTest =
        std::make_shared<SeRequestTest>();
    LocalTest->setUp();
    LocalTest->getSuccessfulSelectionStatusCodes();
}

TEST(SeRequestTest, testG)
{
    std::shared_ptr<SeRequestTest> LocalTest =
        std::make_shared<SeRequestTest>();
    LocalTest->setUp();
    LocalTest->toStringNull();
}

TEST(SeRequestTest, testH)
{
    std::shared_ptr<SeRequestTest> LocalTest =
        std::make_shared<SeRequestTest>();
    LocalTest->setUp();
    LocalTest->constructor1();
}

TEST(SeRequestTest, testI)
{
    std::shared_ptr<SeRequestTest> LocalTest =
        std::make_shared<SeRequestTest>();
    LocalTest->setUp();
    LocalTest->constructor2();
}

TEST(SeRequestTest, testJ)
{
    std::shared_ptr<SeRequestTest> LocalTest =
        std::make_shared<SeRequestTest>();
    LocalTest->setUp();
    LocalTest->constructor2b();
}

TEST(SeRequestTest, testK)
{
    std::shared_ptr<SeRequestTest> LocalTest =
        std::make_shared<SeRequestTest>();
    LocalTest->setUp();
    LocalTest->constructor3();
}
