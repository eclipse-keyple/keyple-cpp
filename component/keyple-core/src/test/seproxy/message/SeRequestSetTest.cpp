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

#include "SeRequestSetTest.h"
#include "SeRequestSet.h"
#include "SeRequest.h"
#include "SeRequestTest.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace seproxy {
namespace message {

void SeRequestSetTest::setUp() throw(std::runtime_error)
{
    // init
    sortedRequests = std::unordered_set<std::shared_ptr<SeRequest>>();

    firstRequest = SeRequestTest::getSeRequestSample();
    sortedRequests->add(firstRequest);

    // init
    seRequestSet = std::make_shared<SeRequestSet>(sortedRequests);
}

void SeRequestSetTest::getRequests()
{
    assertArrayEquals(sortedRequests->toArray(),
                      seRequestSet->getRequests()->toArray());
}

void SeRequestSetTest::getSingleRequest()
{
    seRequestSet = std::make_shared<SeRequestSet>(firstRequest);

    // with only one element it works
    assertEquals(firstRequest, seRequestSet->getSingleRequest());
}

void SeRequestSetTest::getSingleRequestFail()
{
    /* put a second element */
    sortedRequests->add(SeRequestTest::getSeRequestSample());
    seRequestSet = std::make_shared<SeRequestSet>(sortedRequests);
    /* raise exception because it works only with one element */
    seRequestSet->getSingleRequest();
}

void SeRequestSetTest::toStringNull()
{
    seRequestSet = std::make_shared<SeRequestSet>(
        std::unordered_set<std::shared_ptr<SeRequest>>());
    assertNotNull(seRequestSet->toString());
}
}
}
}
}
}
