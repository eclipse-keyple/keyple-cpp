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

#include "SeResponseSetTest.h"
#include "SeResponseSet.h"
#include "SeResponseTest.h"
#include "SeResponse.h"
#include "AnswerToReset.h"
#include "SelectionStatus.h"
#include "ByteArrayUtils.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace seproxy {
namespace message {

using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;

void SeResponseSetTest::getSingleResponse() throw(std::runtime_error)
{
    std::shared_ptr<SeResponseSet> set =
        std::make_shared<SeResponseSet>(SeResponseTest::getASeResponse());
    // assertArrayEquals(SeResponseTest.getASeResponse().getApduResponses()
    //                       .toArray(),
    // set.getSingleResponse().getApduResponses().toArray());
    assertEquals(SeResponseTest::getASeResponse(), set->getSingleResponse());
}

void SeResponseSetTest::getSingleResponseFail() throw(std::runtime_error)
{
    std::vector<std::shared_ptr<SeResponse>> seResponses;
    seResponses.push_back(SeResponseTest::getASeResponse());
    seResponses.push_back(SeResponseTest::getASeResponse());
    std::shared_ptr<SeResponseSet> set =
        std::make_shared<SeResponseSet>(seResponses);

    set->getSingleResponse(); // throw exception
}

void SeResponseSetTest::getResponses() throw(std::runtime_error)
{
    std::vector<std::shared_ptr<SeResponse>> seResponses;
    seResponses.push_back(SeResponseTest::getASeResponse());
    seResponses.push_back(SeResponseTest::getASeResponse());
    std::shared_ptr<SeResponseSet> set =
        std::make_shared<SeResponseSet>(seResponses);
    assertArrayEquals(seResponses.toArray(), set->getResponses().toArray());
}

void SeResponseSetTest::toStringTest() throw(std::runtime_error)
{
    std::shared_ptr<SeResponse> emptySeR = std::make_shared<SeResponse>(
        true,
        std::make_shared<SelectionStatus>(
            std::make_shared<AnswerToReset>(ByteArrayUtils::fromHex("3BFF00")),
            nullptr, true),
        nullptr);
    std::shared_ptr<SeResponseSet> set =
        std::make_shared<SeResponseSet>(emptySeR);
    assertNotNull(set->toString());
}
}
}
}
}
}
