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

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace keyple::core::seproxy::message;

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

void SeResponseTest::constructorSuccessfullResponseMatch() 
{
    std::vector<std::shared_ptr<ApduResponse>> apduResponses = ApduResponseTest::getAListOfAPDUs();
    std::shared_ptr<AnswerToReset> lAtr = ApduResponseTest::getAAtr();
    std::shared_ptr<ApduResponse> lFci = ApduResponseTest::getAFCI();

    std::shared_ptr<SeResponse> response = std::make_shared<SeResponse>(
        true,
        true,
        std::make_shared<SelectionStatus>(lAtr,
                                          lFci, true),
        apduResponses);

    ASSERT_NE(response, nullptr);

    ASSERT_EQ(apduResponses, response->getApduResponses());

    ASSERT_EQ(true, response->wasChannelPreviouslyOpen());
    ASSERT_EQ(lAtr, response->getSelectionStatus()->getAtr());
    ASSERT_EQ(lFci, response->getSelectionStatus()->getFci());
    ASSERT_EQ(response->getSelectionStatus()->hasMatched(), true);
}

void SeResponseTest::constructorSuccessfullResponseNoMatch() 
{
    std::vector<std::shared_ptr<ApduResponse>> apduResponses = ApduResponseTest::getAListOfAPDUs();
    std::shared_ptr<AnswerToReset> lAtr = ApduResponseTest::getAAtr();
    std::shared_ptr<ApduResponse> lFci = ApduResponseTest::getAFCI();
    std::shared_ptr<SeResponse> response = std::make_shared<SeResponse>(
        true,
        true,
        std::make_shared<SelectionStatus>(lAtr, lFci, false),
        apduResponses);

    ASSERT_NE(response, nullptr);

    ASSERT_EQ(apduResponses, response->getApduResponses());

    ASSERT_EQ(true, response->wasChannelPreviouslyOpen());
    ASSERT_EQ(lAtr, response->getSelectionStatus()->getAtr());
    ASSERT_EQ(lFci, response->getSelectionStatus()->getFci());
    ASSERT_EQ(response->getSelectionStatus()->hasMatched(), false);
}

void SeResponseTest::constructorATRNull() 
{
    std::vector<std::shared_ptr<ApduResponse>> apduResponses = ApduResponseTest::getAListOfAPDUs();
    std::shared_ptr<SeResponse> response = std::make_shared<SeResponse>(
        true,
        true,
        std::make_shared<SelectionStatus>(nullptr, ApduResponseTest::getAFCI(),
                                          true),
        apduResponses);
    ASSERT_NE(response, nullptr);
}

void SeResponseTest::constructorFCINull() 
{
    std::vector<std::shared_ptr<ApduResponse>> apduResponses = ApduResponseTest::getAListOfAPDUs();
    std::shared_ptr<SeResponse> response = std::make_shared<SeResponse>(
        true,
        true,
        std::make_shared<SelectionStatus>(ApduResponseTest::getAAtr(), nullptr,
                                          true),
        apduResponses);
    ASSERT_NE(response, nullptr);
}

void SeResponseTest::constructorFCIAndATRNull() 
{
    std::vector<std::shared_ptr<ApduResponse>> apduResponses = ApduResponseTest::getAListOfAPDUs();
    std::shared_ptr<SeResponse> response;
    try{
        response = std::make_shared<SeResponse>(
            true, 
            true,
            std::make_shared<SelectionStatus>(nullptr, nullptr, true), //Exception ici => atr et fci non null simultannément
            apduResponses);
    }
    catch(...)
    {}
    ASSERT_EQ(response, nullptr);
}

void SeResponseTest::testEquals() 
{
    ASSERT_FALSE(getASeResponse()->equals(getASeResponse()));
}

void SeResponseTest::testThisEquals() 
{
    std::shared_ptr<SeResponse> resp = getASeResponse();
    ASSERT_TRUE(resp->equals(resp));
}

void SeResponseTest::testNotEquals() 
{
    std::shared_ptr<SeResponse> resp = getASeResponse();
    std::shared_ptr<void> any;
    ASSERT_FALSE(resp->equals(any));
}

void SeResponseTest::testNotEqualsNull()
{
    std::shared_ptr<SeResponse> resp     = getASeResponse();
    std::vector<std::shared_ptr<ApduResponse>> apduResponses = {};
    std::shared_ptr<SeResponse> respNull = std::make_shared<SeResponse>(
        true,
        true,
        std::make_shared<SelectionStatus>(nullptr, ApduResponseTest::getAFCI(),
                                          true),
        apduResponses);
    std::shared_ptr<SeResponse> respNull2 = std::make_shared<SeResponse>(
        true,
        true,
        std::make_shared<SelectionStatus>(ApduResponseTest::getAAtr(), nullptr,
                                          true),
        apduResponses);
    std::shared_ptr<SeResponse> respNull3 = std::make_shared<SeResponse>(
        true,
        true,
        std::make_shared<SelectionStatus>(ApduResponseTest::getAAtr(),
                                          ApduResponseTest::getAFCI(), true),
        apduResponses);
    ASSERT_FALSE(resp->equals(respNull));
    ASSERT_FALSE(resp->equals(respNull2));
    ASSERT_FALSE(resp->equals(respNull3));
}

void SeResponseTest::hashcode() 
{
    std::shared_ptr<SeResponse> resp  = getASeResponse();
    std::shared_ptr<SeResponse> resp2 = getASeResponse();
    ASSERT_TRUE(resp->hashCode() == resp2->hashCode());
}

void SeResponseTest::hashcodeNull() 
{
    std::vector<std::shared_ptr<ApduResponse>> apduResponses = {};
    std::shared_ptr<SeResponse> resp = std::make_shared<SeResponse>(
        true,
        true,
        std::make_shared<SelectionStatus>(nullptr, ApduResponseTest::getAFCI(),
                                          true),
        apduResponses);
    ASSERT_NE(resp->hashCode(), 0);
}

std::shared_ptr<SeResponse>
SeResponseTest::getASeResponse() 
{
    std::vector<std::shared_ptr<ApduResponse>> apduResponses = ApduResponseTest::getAListOfAPDUs();
    return std::make_shared<SeResponse>(
        true,
        true,
        std::make_shared<SelectionStatus>(ApduResponseTest::getAAtr(),
                                          ApduResponseTest::getAFCI(), true),
        apduResponses);
}
}
}
}
}

TEST(SeResponseTest, testA)
{
    std::shared_ptr<SeResponseTest> LocalTest =
        std::make_shared<SeResponseTest>();
    LocalTest->constructorSuccessfullResponseMatch();
}

TEST(SeResponseTest, testB)
{
    std::shared_ptr<SeResponseTest> LocalTest =
        std::make_shared<SeResponseTest>();
    LocalTest->constructorSuccessfullResponseNoMatch();
}

TEST(SeResponseTest, testC)
{
    std::shared_ptr<SeResponseTest> LocalTest =
        std::make_shared<SeResponseTest>();
    LocalTest->constructorATRNull();
}

TEST(SeResponseTest, testD)
{
    std::shared_ptr<SeResponseTest> LocalTest =
        std::make_shared<SeResponseTest>();
    LocalTest->constructorFCINull();
}

TEST(SeResponseTest, testE)
{
    std::shared_ptr<SeResponseTest> LocalTest =
        std::make_shared<SeResponseTest>();
    LocalTest->constructorFCIAndATRNull();
}

TEST(SeResponseTest, testF)
{
    std::shared_ptr<SeResponseTest> LocalTest =
        std::make_shared<SeResponseTest>();
    LocalTest->testEquals();
}

TEST(SeResponseTest, testG)
{
    std::shared_ptr<SeResponseTest> LocalTest =
        std::make_shared<SeResponseTest>();
    LocalTest->testThisEquals();
}

TEST(SeResponseTest, testH)
{
    std::shared_ptr<SeResponseTest> LocalTest =
        std::make_shared<SeResponseTest>();
    LocalTest->testNotEquals();
}

TEST(SeResponseTest, testI)
{
    std::shared_ptr<SeResponseTest> LocalTest =
        std::make_shared<SeResponseTest>();
    LocalTest->testNotEqualsNull();
}

TEST(SeResponseTest, testJ)
{
    std::shared_ptr<SeResponseTest> LocalTest =
        std::make_shared<SeResponseTest>();
    LocalTest->hashcode();
}

TEST(SeResponseTest, testK)
{
    std::shared_ptr<SeResponseTest> LocalTest =
        std::make_shared<SeResponseTest>();
    LocalTest->hashcodeNull();
}
