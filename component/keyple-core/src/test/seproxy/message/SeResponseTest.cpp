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

#include "SeResponseTest.h"
#include "SelectionStatus.h"
#include "SeResponse.h"
#include "ApduResponseTest.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace seproxy {
namespace message {

void SeResponseTest::constructorSuccessfullResponseMatch() throw(
    std::invalid_argument)
{

    std::shared_ptr<SeResponse> response = std::make_shared<SeResponse>(
        true,
        std::make_shared<SelectionStatus>(ApduResponseTest::getAAtr(),
                                          ApduResponseTest::getAFCI(), true),
        ApduResponseTest::getAListOfAPDUs());
    Assert::assertNotNull(response);
    Assert::assertArrayEquals(ApduResponseTest::getAListOfAPDUs().toArray(),
                              response->getApduResponses().toArray());
    Assert::assertEquals(true, response->wasChannelPreviouslyOpen());
    Assert::assertEquals(ApduResponseTest::getAAtr(),
                         response->getSelectionStatus()->getAtr());
    Assert::assertEquals(ApduResponseTest::getAFCI(),
                         response->getSelectionStatus()->getFci());
    Assert::assertEquals(response->getSelectionStatus()->hasMatched(), true);
}

void SeResponseTest::constructorSuccessfullResponseNoMatch() throw(
    std::invalid_argument)
{

    std::shared_ptr<SeResponse> response = std::make_shared<SeResponse>(
        true,
        std::make_shared<SelectionStatus>(ApduResponseTest::getAAtr(),
                                          ApduResponseTest::getAFCI(), false),
        ApduResponseTest::getAListOfAPDUs());
    Assert::assertNotNull(response);
    Assert::assertArrayEquals(ApduResponseTest::getAListOfAPDUs().toArray(),
                              response->getApduResponses().toArray());
    Assert::assertEquals(true, response->wasChannelPreviouslyOpen());
    Assert::assertEquals(ApduResponseTest::getAAtr(),
                         response->getSelectionStatus()->getAtr());
    Assert::assertEquals(ApduResponseTest::getAFCI(),
                         response->getSelectionStatus()->getFci());
    Assert::assertEquals(response->getSelectionStatus()->hasMatched(), false);
}

void SeResponseTest::constructorATRNull() throw(std::invalid_argument)
{
    std::shared_ptr<SeResponse> response = std::make_shared<SeResponse>(
        true,
        std::make_shared<SelectionStatus>(nullptr, ApduResponseTest::getAFCI(),
                                          true),
        ApduResponseTest::getAListOfAPDUs());
    Assert::assertNotNull(response);
}

void SeResponseTest::constructorFCINull() throw(std::invalid_argument)
{
    std::shared_ptr<SeResponse> response = std::make_shared<SeResponse>(
        true,
        std::make_shared<SelectionStatus>(ApduResponseTest::getAAtr(), nullptr,
                                          true),
        ApduResponseTest::getAListOfAPDUs());
    Assert::assertNotNull(response);
}

void SeResponseTest::constructorFCIAndATRNull() throw(std::invalid_argument)
{
    std::shared_ptr<SeResponse> response = std::make_shared<SeResponse>(
        true, std::make_shared<SelectionStatus>(nullptr, nullptr, true),
        ApduResponseTest::getAListOfAPDUs());
    Assert::assertNull(response);
}

void SeResponseTest::testEquals() throw(std::runtime_error)
{
    Assert::assertTrue(getASeResponse()->equals(getASeResponse()));
}

void SeResponseTest::testThisEquals() throw(std::runtime_error)
{
    std::shared_ptr<SeResponse> resp = getASeResponse();
    Assert::assertTrue(resp->equals(resp));
}

void SeResponseTest::testNotEquals() throw(std::runtime_error)
{
    std::shared_ptr<SeResponse> resp = getASeResponse();
    std::shared_ptr<void> any;
    Assert::assertFalse(resp->equals(any));
}

void SeResponseTest::testNotEqualsNull() throw(std::runtime_error)
{
    std::shared_ptr<SeResponse> resp     = getASeResponse();
    std::shared_ptr<SeResponse> respNull = std::make_shared<SeResponse>(
        true,
        std::make_shared<SelectionStatus>(nullptr, ApduResponseTest::getAFCI(),
                                          true),
        nullptr);
    std::shared_ptr<SeResponse> respNull2 = std::make_shared<SeResponse>(
        true,
        std::make_shared<SelectionStatus>(ApduResponseTest::getAAtr(), nullptr,
                                          true),
        nullptr);
    std::shared_ptr<SeResponse> respNull3 = std::make_shared<SeResponse>(
        true,
        std::make_shared<SelectionStatus>(ApduResponseTest::getAAtr(),
                                          ApduResponseTest::getAFCI(), true),
        nullptr);
    Assert::assertFalse(resp->equals(respNull));
    Assert::assertFalse(resp->equals(respNull2));
    Assert::assertFalse(resp->equals(respNull3));
}

void SeResponseTest::hashcode() throw(std::runtime_error)
{
    std::shared_ptr<SeResponse> resp  = getASeResponse();
    std::shared_ptr<SeResponse> resp2 = getASeResponse();
    Assert::assertTrue(resp->hashCode() == resp2->hashCode());
}

void SeResponseTest::hashcodeNull() throw(std::runtime_error)
{
    std::shared_ptr<SeResponse> resp = std::make_shared<SeResponse>(
        true,
        std::make_shared<SelectionStatus>(nullptr, ApduResponseTest::getAFCI(),
                                          true),
        nullptr);
    Assert::assertNotNull(resp->hashCode());
}

std::shared_ptr<SeResponse>
SeResponseTest::getASeResponse() throw(std::invalid_argument)
{
    return std::make_shared<SeResponse>(
        true,
        std::make_shared<SelectionStatus>(ApduResponseTest::getAAtr(),
                                          ApduResponseTest::getAFCI(), true),
        ApduResponseTest::getAListOfAPDUs());
}
}
}
}
}
}
