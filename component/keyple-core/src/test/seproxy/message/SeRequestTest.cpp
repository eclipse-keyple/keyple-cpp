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

#include "SeRequest.h"

#include "SeCommonProtocols.h"
#include "SeSelector.h"

using namespace testing;

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::message;

static const std::vector<std::shared_ptr<ApduRequest>> apdus;

static std::shared_ptr<std::set<int>> getSuccessFulStatusCode()
{
    std::shared_ptr<std::set<int>> successfulStatusCodes =
        std::make_shared< std::set<int>>();

    int iData = std::stoi("9000", 0, 16);
    successfulStatusCodes->insert(iData);

    return successfulStatusCodes;
}

static std::shared_ptr<SeSelector> getSeSelector(
    std::shared_ptr<std::set<int>> successfulStatusCodes)
{
    /*
     * We can use a fake AID here because it is not fully interpreted, the
     * purpose of this unit test is to verify the proper format of the request.
     */
    std::shared_ptr<SeSelector::AidSelector> aidSelector =
        SeSelector::AidSelector::builder()->aidToSelect("AABBCCDDEEFF")
            .build();

    if (successfulStatusCodes) {
        for (const auto& code : *successfulStatusCodes.get())
            aidSelector->addSuccessfulStatusCode(code);
    }

    return SeSelector::builder()
               ->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
               .aidSelector(aidSelector).build();
}

TEST(SeRequestTest, SeRequest)
{
    SeRequest request1(nullptr, apdus);
    SeRequest request2(getSeSelector(nullptr), apdus);
}

TEST(SeRequestTest, getSelector)
{
    std::shared_ptr<SeSelector> selector = getSeSelector(nullptr);

    SeRequest request1(nullptr, apdus);
    SeRequest request2(selector, apdus);

    ASSERT_EQ(request1.getSeSelector(), nullptr);
    ASSERT_EQ(request2.getSeSelector(), selector);
}

TEST(SeRequestTest, getApduRequests)
{
    std::shared_ptr<SeSelector> selector = getSeSelector(nullptr);

    SeRequest request1(nullptr, apdus);
    SeRequest request2(selector, apdus);

    ASSERT_EQ(request1.getApduRequests(), apdus);
    ASSERT_EQ(request2.getApduRequests(), apdus);
}

TEST(SeRequestTest, getSeProtocol)
{
    std::shared_ptr<SeSelector> selector = getSeSelector(nullptr);

    SeRequest request2(selector, apdus);

    ASSERT_EQ(request2.getSeSelector()->getSeProtocol(),
              SeCommonProtocols::PROTOCOL_ISO14443_4);
}

TEST(SeRequestTest, getSuccessfulSelectionStatusCodes)
{
    std::shared_ptr<SeSelector> selector =
        getSeSelector(getSuccessFulStatusCode());

    SeRequest request2(selector, apdus);

    std::set<int> statusCode = *getSuccessFulStatusCode().get();
    std::set<int> testCode =
        *(request2.getSeSelector()->getAidSelector()
            ->getSuccessfulSelectionStatusCodes().get());

    std::set<int>::iterator it, it_;

    for (it = testCode.begin(), it_ = statusCode.begin();
         it != testCode.end(); ++it, ++it_)
        EXPECT_EQ( *it, *it_);
}
