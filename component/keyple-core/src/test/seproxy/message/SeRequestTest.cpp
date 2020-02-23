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
#include "SeRequest.h"
#include "ApduRequest.h"
#include "SeProtocol.h"
#include "SeSelector.h"
#include "ApduRequestTest.h"
#include "Protocol.h"
#include "ContactlessProtocols.h"
#include "ByteArrayUtils.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace seproxy {
namespace message {
using ChannelState = keyple::core::seproxy::ChannelState;
using SeSelector   = keyple::core::seproxy::SeSelector;
using ContactlessProtocols =
    keyple::core::seproxy::protocol::ContactlessProtocols;
using Protocol       = keyple::core::seproxy::protocol::Protocol;
using SeProtocol     = keyple::core::seproxy::protocol::SeProtocol;
using ByteArrayUtils = keyple::core::util::ByteArrayUtil;

std::vector<std::shared_ptr<ApduRequest>> SeRequestTest::getApdus()
{
    return apdus;
}

void SeRequestTest::setUp()
{

    apdus               = getAapduLists();
    channelState        = ChannelState::KEEP_OPEN;
    seProtocol          = getASeProtocol();
    selectionStatusCode = ApduRequestTest::getASuccessFulStatusCode();
    selector            = getSelector(selectionStatusCode);
    seRequest =
        std::make_shared<SeRequest>(selector, apdus, channelState, seProtocol);
}

void SeRequestTest::testSERequest()
{
    assertNotNull(seRequest);
}

void SeRequestTest::getSelector()
{
    // test
    assertEquals(getSelector(selectionStatusCode)->toString(),
                 seRequest->getSeSelector()->toString());
}

void SeRequestTest::getApduRequests()
{
    // test
    seRequest = std::make_shared<SeRequest>(
        getSelector(nullptr), apdus, ChannelState::CLOSE_AFTER, Protocol::ANY);
    assertArrayEquals(apdus.toArray(), seRequest->getApduRequests().toArray());
}

void SeRequestTest::isKeepChannelOpen()
{
    assertTrue(seRequest->isKeepChannelOpen());
}

void SeRequestTest::getProtocolFlag()
{
    seRequest = std::make_shared<SeRequest>(
        getSelector(nullptr), std::vector<std::shared_ptr<ApduRequest>>(),
        ChannelState::KEEP_OPEN, seProtocol);
    assertEquals(seProtocol, seRequest->getProtocolFlag());
}

void SeRequestTest::getSuccessfulSelectionStatusCodes()
{
    seRequest = std::make_shared<SeRequest>(
        getSelector(selectionStatusCode),
        std::vector<std::shared_ptr<ApduRequest>>(), ChannelState::KEEP_OPEN,
        ContactlessProtocols::PROTOCOL_B_PRIME);
    assertArrayEquals(selectionStatusCode->toArray(),
                      seRequest->getSeSelector()
                          ->getAidSelector()
                          ->getSuccessfulSelectionStatusCodes()
                          ->toArray());
}

void SeRequestTest::toStringNull()
{
    seRequest = std::make_shared<SeRequest>(nullptr, nullptr, nullptr, nullptr);
    assertNotNull(seRequest->toString());
}

void SeRequestTest::constructor1()
{
    seRequest = std::make_shared<SeRequest>(getSelector(nullptr), apdus,
                                            channelState, Protocol::ANY);
    assertEquals(getSelector(nullptr)->toString(),
                 seRequest->getSeSelector()->toString());
    assertEquals(channelState == ChannelState::KEEP_OPEN,
                 seRequest->isKeepChannelOpen());
    assertArrayEquals(apdus.toArray(), seRequest->getApduRequests().toArray());
    //
    assertEquals(Protocol::ANY, seRequest->getProtocolFlag());
    assertNull(seRequest->getSeSelector()
                   ->getAidSelector()
                   ->getSuccessfulSelectionStatusCodes());
}

void SeRequestTest::constructor2()
{
    seRequest = std::make_shared<SeRequest>(getSelector(nullptr), apdus,
                                            channelState, seProtocol);
    assertEquals(getSelector(nullptr)->toString(),
                 seRequest->getSeSelector()->toString());
    assertEquals(channelState == ChannelState::KEEP_OPEN,
                 seRequest->isKeepChannelOpen());
    assertArrayEquals(apdus.toArray(), seRequest->getApduRequests().toArray());
    assertEquals(seProtocol, seRequest->getProtocolFlag());
    //
    assertNull(seRequest->getSeSelector()
                   ->getAidSelector()
                   ->getSuccessfulSelectionStatusCodes());
}

void SeRequestTest::constructor2b()
{
    seRequest = std::make_shared<SeRequest>(getSelector(selectionStatusCode),
                                            apdus, channelState, Protocol::ANY);
    assertEquals(getSelector(selectionStatusCode)->toString(),
                 seRequest->getSeSelector()->toString());
    assertEquals(channelState == ChannelState::KEEP_OPEN,
                 seRequest->isKeepChannelOpen());
    assertArrayEquals(apdus.toArray(), seRequest->getApduRequests().toArray());
    assertEquals(Protocol::ANY, seRequest->getProtocolFlag());
    //
    assertArrayEquals(selectionStatusCode->toArray(),
                      seRequest->getSeSelector()
                          ->getAidSelector()
                          ->getSuccessfulSelectionStatusCodes()
                          ->toArray());
}

void SeRequestTest::constructor3()
{
    seRequest = std::make_shared<SeRequest>(getSelector(selectionStatusCode),
                                            apdus, channelState, seProtocol);
    assertEquals(getSelector(selectionStatusCode)->toString(),
                 seRequest->getSeSelector()->toString());
    assertEquals(channelState == ChannelState::KEEP_OPEN,
                 seRequest->isKeepChannelOpen());
    assertArrayEquals(apdus.toArray(), seRequest->getApduRequests().toArray());
    assertEquals(seProtocol, seRequest->getProtocolFlag());
    assertArrayEquals(selectionStatusCode->toArray(),
                      seRequest->getSeSelector()
                          ->getAidSelector()
                          ->getSuccessfulSelectionStatusCodes()
                          ->toArray());
}

std::shared_ptr<SeRequest> SeRequestTest::getSeRequestSample()
{

    std::vector<std::shared_ptr<ApduRequest>> apdus = getAapduLists();
    ChannelState channelState                       = ChannelState::KEEP_OPEN;
    std::shared_ptr<SeProtocol> seProtocol          = getASeProtocol();
    std::shared_ptr<std::set<Integer>> selectionStatusCode =
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
    return ContactlessProtocols::PROTOCOL_B_PRIME;
}

std::shared_ptr<SeSelector> SeRequestTest::getSelector(
    std::shared_ptr<std::set<Integer>> selectionStatusCode)
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
}
