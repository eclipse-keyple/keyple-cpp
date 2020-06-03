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


#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "ApduRequest.h"

#include "ByteArrayUtil.h"

using namespace testing;

using namespace keyple::core::seproxy::message;
using namespace keyple::core::util;

static const std::vector<uint8_t> req = {0xFE, 0xDC, 0xBA, 0x98, 0x90, 0x05};

static std::shared_ptr<std::set<int>> getASuccessFulStatusCode()
{
    std::shared_ptr<std::set<int>> successfulStatusCodes =
        std::make_shared< std::set<int>>();

    int iData = std::stoi("9000", 0, 16);
    successfulStatusCodes->insert(iData);

    return successfulStatusCodes;
}

static std::shared_ptr<ApduRequest> getApduSample()
{
    std::shared_ptr<std::set<int>> successfulStatusCodes =
        getASuccessFulStatusCode();

    std::shared_ptr<ApduRequest> request =
        std::make_shared<ApduRequest>(req, true, successfulStatusCodes);

    request->setName("TEST");

    return request;
}

TEST(ApduRequestTest, ApduRequest)
{
    ApduRequest request1(req, true);
    ApduRequest request2("request2", req, true, getASuccessFulStatusCode());
    ApduRequest request3(req, true, getASuccessFulStatusCode());
}

TEST(ApduRequestTest, isCase4)
{
    ApduRequest request1(req, true);
    ApduRequest request2("request2", req, false, getASuccessFulStatusCode());
    ApduRequest request3(req, true, getASuccessFulStatusCode());

    ASSERT_TRUE(request1.isCase4());
    ASSERT_FALSE(request2.isCase4());
    ASSERT_TRUE(request3.isCase4());
}

TEST(ApduRequestTest, getName)
{
    ApduRequest request1(req, true);
    ApduRequest request2("request2", req, false, getASuccessFulStatusCode());
    ApduRequest request3(req, true, getASuccessFulStatusCode());

    ASSERT_EQ(request1.getName(), "");
    ASSERT_EQ(request2.getName(), "request2");
    ASSERT_EQ(request3.getName(), "");
}

TEST(ApduRequestTest, setName)
{
    ApduRequest request1(req, true);
    ApduRequest request2("requestX", req, false, getASuccessFulStatusCode());
    ApduRequest request3(req, true, getASuccessFulStatusCode());

    request1.setName("request1");
    request2.setName("request2");
    request3.setName("request3");

    ASSERT_EQ(request1.getName(), "request1");
    ASSERT_EQ(request2.getName(), "request2");
    ASSERT_EQ(request3.getName(), "request3");
}

TEST(ApduRequestTest, getSuccessfulStatusCodes)
{
    ApduRequest request1(req, true);
    ApduRequest request2("request2", req, false, getASuccessFulStatusCode());
    ApduRequest request3(req, true, getASuccessFulStatusCode());

    std::set<int> refSc = (*getASuccessFulStatusCode());
    std::set<int> sc2 = (*request2.getSuccessfulStatusCodes());
    std::set<int> sc3 = (*request3.getSuccessfulStatusCodes());

    std::set<int>::iterator it, it_;

    for (it = sc2.begin(), it_ = refSc.begin(); it != sc2.end(); ++it, ++it_)
        EXPECT_EQ( *it, *it_);

    for (it = sc3.begin(), it_ = refSc.begin(); it != sc3.end(); ++it, ++it_)
        EXPECT_EQ( *it, *it_);
}

TEST(ApduRequestTest, getBytes)
{
    ApduRequest request1(req, true);
    ApduRequest request2("request2", req, false, getASuccessFulStatusCode());
    ApduRequest request3(req, true, getASuccessFulStatusCode());

    ASSERT_EQ(request1.getBytes(), req);
    ASSERT_EQ(request2.getBytes(), req);
    ASSERT_EQ(request3.getBytes(), req);
}

TEST(ApduRequestTest, testSimpleAPDURequest)
{

    ApduRequest request(req, true);

    ASSERT_EQ(request.getName(), "");
    ASSERT_TRUE(request.isCase4());
    ASSERT_EQ(request.getBytes(), req);
    ASSERT_EQ(request.getSuccessfulStatusCodes(), nullptr);
}

TEST(ApduRequestTest, testAPDURequest)
{
    std::shared_ptr<ApduRequest> request = getApduSample();

    ASSERT_NE(request, nullptr);
    ASSERT_TRUE(request->isCase4());
    ASSERT_EQ(request->getBytes(), req);
    ASSERT_EQ(request->getName(), "TEST");

    std::set<int> mySet1 = (*getASuccessFulStatusCode());
    std::set<int> mySet2 = (*request->getSuccessfulStatusCodes());

    for (std::set<int>::iterator it=mySet1.begin(), it_=mySet2.begin();
         it!=mySet1.end(); ++it, ++it_) {
        EXPECT_EQ( *it, *it_);
    }
}

