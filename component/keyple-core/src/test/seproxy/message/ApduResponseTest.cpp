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

#include "ApduResponseTest.h"
#include "ApduResponse.h"
#include "ByteArrayUtils.h"
#include "AnswerToReset.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace seproxy {
namespace message {

using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
using org::junit::Before;
using org::junit::Test;
using org::junit::runner::RunWith;
using org::mockito::junit::MockitoJUnitRunner;

void ApduResponseTest::setUp()
{
}

void ApduResponseTest::constructorSuccessFullResponse()
{
    std::shared_ptr<ApduResponse> response = std::make_shared<ApduResponse>(
        ByteArrayUtils::fromHex("FEDCBA98 9000h"), nullptr);
    assertNotNull(response);
    assertEquals(0x9000, response->getStatusCode());
    assertEquals("FEDCBA989000", ByteArrayUtils::toHex(response->getBytes()));
    assertArrayEquals(ByteArrayUtils::fromHex("FEDCBA98"),
                      response->getDataOut());
    assertTrue(response->isSuccessful());
}

void ApduResponseTest::constructorSuccessFullResponseWithCustomCode()
{
    std::shared_ptr<ApduResponse> response = std::make_shared<ApduResponse>(
        ByteArrayUtils::fromHex("FEDCBA98 9005h"), getA9005CustomCode());
    assertNotNull(response);
    assertEquals(0x9005, response->getStatusCode());
    assertEquals("FEDCBA989005", ByteArrayUtils::toHex(response->getBytes()));
    assertArrayEquals(ByteArrayUtils::fromHex("FEDCBA98"),
                      response->getDataOut());
    assertTrue(response->isSuccessful());
}

void ApduResponseTest::constructorFailResponse()
{
    std::shared_ptr<ApduResponse> response = std::make_shared<ApduResponse>(
        ByteArrayUtils::fromHex("FEDCBA98 9004h"), nullptr);
    assertNotNull(response);
    assertEquals("FEDCBA989004", ByteArrayUtils::toHex(response->getBytes()));
    assertArrayEquals(ByteArrayUtils::fromHex("FEDCBA98"),
                      response->getDataOut());
    assertEquals(0x9004, response->getStatusCode());
    assertFalse(response->isSuccessful());
}

void ApduResponseTest::constructorFailResponseWithCustomCode()
{
    std::shared_ptr<ApduResponse> response = std::make_shared<ApduResponse>(
        ByteArrayUtils::fromHex("FEDCBA98 9004h"), getA9005CustomCode());
    assertNotNull(response);
    assertEquals("FEDCBA989004", ByteArrayUtils::toHex(response->getBytes()));
    assertArrayEquals(ByteArrayUtils::fromHex("FEDCBA98"),
                      response->getDataOut());
    assertEquals(0x9004, response->getStatusCode());
    assertFalse(response->isSuccessful());
}

void ApduResponseTest::isEqualsTest()
{
    assertTrue(getAFCI()->equals(getAFCI()));
}

void ApduResponseTest::isThisEquals()
{
    std::shared_ptr<ApduResponse> resp = getAFCI();
    assertTrue(resp->equals(resp));
}

void ApduResponseTest::isNotEquals()
{
    std::shared_ptr<ApduResponse> resp = getAFCI();
    std::shared_ptr<void> obj;
    assertFalse(resp->equals(obj));
}

void ApduResponseTest::isNotEqualsNull()
{
    std::shared_ptr<ApduResponse> resp = getAFCI();
    std::shared_ptr<ApduResponse> respNull =
        std::make_shared<ApduResponse>(nullptr, nullptr);
    assertFalse(resp->equals(respNull));
}

void ApduResponseTest::hashcodeTest()
{
    std::shared_ptr<ApduResponse> resp  = getAFCI();
    std::shared_ptr<ApduResponse> resp2 = getAFCI();
    assertTrue(resp->hashCode() == resp2->hashCode());
}

void ApduResponseTest::hashcodeNull()
{
    std::shared_ptr<ApduResponse> resp =
        std::make_shared<ApduResponse>(nullptr, nullptr);
    assertNotNull(resp->hashCode());
}

void ApduResponseTest::testToStringNull()
{
    std::shared_ptr<ApduResponse> resp =
        std::make_shared<ApduResponse>(nullptr, nullptr);
    assertNotNull(resp->toString());
}

std::shared_ptr<std::set<Integer>> ApduResponseTest::getA9005CustomCode()
{
    std::shared_ptr<std::set<Integer>> successfulStatusCodes =
        std::unordered_set<Integer>();
    successfulStatusCodes->add(0x9005);
    return successfulStatusCodes;
}

std::shared_ptr<AnswerToReset> ApduResponseTest::getAAtr()
{
    return std::make_shared<AnswerToReset>(
        ByteArrayUtils::fromHex("3B8F8001804F0CA000000306030001000000006A"));
}

std::shared_ptr<ApduResponse> ApduResponseTest::getAFCI()
{
    return std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("9000"),
                                          nullptr);
}

std::shared_ptr<ApduResponse> ApduResponseTest::getSuccessfullResponse()
{
    return std::make_shared<ApduResponse>(
        ByteArrayUtils::fromHex("FEDCBA98 9000h"), nullptr);
}

std::vector<std::shared_ptr<ApduResponse>> ApduResponseTest::getAListOfAPDUs()
{
    std::vector<std::shared_ptr<ApduResponse>> apdus;
    apdus.push_back(getSuccessfullResponse());
    return apdus;
}
}
}
}
}
}
