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

using namespace testing;
using namespace keyple::core::seproxy::message;

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using ByteArrayUtils = keyple::core::util::ByteArrayUtil;

void ApduRequestTest::setUp()
{
}

void ApduRequestTest::testSimpleAPDURequest()
{
    std::shared_ptr<ApduRequest> request =
        std::make_shared<ApduRequest>(getACommand(), true);
    ASSERT_NE(request, nullptr);
    ASSERT_EQ("", request->getName());
    ASSERT_TRUE(request->isCase4());
    ASSERT_EQ(getACommand(), request->getBytes());
    ASSERT_EQ(nullptr, request->getSuccessfulStatusCodes());
    ASSERT_EQ("ApduRequest: NAME = , RAWDATA = FEDCBA989005, case4",
                 request->toString());
}

void ApduRequestTest::testAPDURequest()
{
    std::shared_ptr<ApduRequest> request = getApduSample();
    ASSERT_NE(request, nullptr);
    ASSERT_TRUE(request->isCase4());
    ASSERT_EQ(getACommand(), request->getBytes());
    ASSERT_EQ(getAName(), request->getName());

    std::set<int> mySet1 = (*getASuccessFulStatusCode());
    std::set<int> mySet2 = (*request->getSuccessfulStatusCodes());

    for (std::set<int>::iterator it=mySet1.begin(), it_=mySet2.begin(); it!=mySet1.end(); ++it, ++it_)
    {
        EXPECT_EQ( *it, *it_);
    }

    ASSERT_EQ( "ApduRequest: NAME = " + getAName() +
                     ", RAWDATA = FEDCBA989005, case4, additional successful "
                     "status codes = 9000",
                 request->toString());
}

std::shared_ptr<ApduRequest> ApduRequestTest::getApduSample()
{
    std::shared_ptr<std::set<int>> successfulStatusCodes =
        getASuccessFulStatusCode();
    bool case4             = true;
    std::vector<uint8_t> command = getACommand();
    std::shared_ptr<ApduRequest> request =
        std::make_shared<ApduRequest>(command, case4, successfulStatusCodes);
    request->setName(getAName());
    return request;
}

std::vector<uint8_t> ApduRequestTest::getACommand()
{
    return ByteArrayUtils::fromHex("FEDCBA98 9005");
}

std::shared_ptr<std::set<int>> ApduRequestTest::getASuccessFulStatusCode()
{
    std::shared_ptr<std::set<int>> successfulStatusCodes = std::make_shared< std::set<int>>();
    int iData = std::stoi("9000", 0, 16);
    successfulStatusCodes->insert(iData);
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

TEST(ApduRequestTest, testA)
{
    std::shared_ptr<ApduRequestTest> LocalTest =
        std::make_shared<ApduRequestTest>();
    LocalTest->testSimpleAPDURequest();
}

TEST(ApduRequestTest, testB)
{
    std::shared_ptr<ApduRequestTest> LocalTest =
        std::make_shared<ApduRequestTest>();
    LocalTest->testAPDURequest();
}
