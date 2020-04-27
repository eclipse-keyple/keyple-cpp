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

#include "ApduRequestTest.h"
#include "ApduRequest.h"
#include "ByteArrayUtils.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace seproxy {
namespace message {

using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;

void ApduRequestTest::setUp() throw(std::runtime_error)
{
}

void ApduRequestTest::testSimpleAPDURequest()
{
    std::shared_ptr<ApduRequest> request =
        std::make_shared<ApduRequest>(getACommand(), true);
    assertNotNull(request);
    assertEquals(nullptr, request->getName());
    assertTrue(request->isCase4());
    assertArrayEquals(getACommand(), request->getBytes());
    assertEquals(nullptr, request->getSuccessfulStatusCodes());
    assertEquals("ApduRequest: NAME = \"null\", RAWDATA = FEDCBA989005, case4",
                 request->toString());
}

void ApduRequestTest::testAPDURequest()
{
    std::shared_ptr<ApduRequest> request = getApduSample();
    assertNotNull(request);
    assertTrue(request->isCase4());
    assertArrayEquals(getACommand(), request->getBytes());
    assertEquals(getAName(), request->getName());
    assertEquals(getASuccessFulStatusCode(),
                 request->getSuccessfulStatusCodes());
    assertEquals("ApduRequest: NAME = \"" + getAName() +
                     "\", RAWDATA = FEDCBA989005, case4, additional successful "
                     "status codes = 2328",
                 request->toString());
}

std::shared_ptr<ApduRequest> ApduRequestTest::getApduSample()
{
    std::shared_ptr<std::set<Integer>> successfulStatusCodes =
        getASuccessFulStatusCode();
    Boolean case4             = true;
    std::vector<char> command = getACommand();
    std::shared_ptr<ApduRequest> request =
        std::make_shared<ApduRequest>(command, case4, successfulStatusCodes);
    request->setName(getAName());
    return request;
}

std::vector<char> ApduRequestTest::getACommand()
{
    return ByteArrayUtils::fromHex("FEDCBA98 9005h");
}

std::shared_ptr<std::set<int>> ApduRequestTest::getASuccessFulStatusCode()
{
    std::shared_ptr<std::set<Integer>> successfulStatusCodes =
        std::unordered_set<Integer>();
    successfulStatusCodes->add(std::stoi("9000"));
    return successfulStatusCodes;
}

std::string ApduRequestTest::getAName()
{
    return "TEST";
}
}
}
}
}
}
