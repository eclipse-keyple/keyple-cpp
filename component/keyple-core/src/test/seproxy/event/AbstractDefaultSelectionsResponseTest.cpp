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

#include "AbstractDefaultSelectionsResponse.h"

using namespace testing;

using namespace keyple::core::seproxy::event;

class ADSR_AbstractDefaultSelectionsResponseMock :
public AbstractDefaultSelectionsResponse {
public:
    ADSR_AbstractDefaultSelectionsResponseMock(
      const std::vector<std::shared_ptr<SeResponse>>& selectionSeResponses)
    : AbstractDefaultSelectionsResponse(selectionSeResponses) {}
};

TEST(AbstractDefaultSelectionsResponseTest, AbstractDefaultSelectionsResponse)
{
    const std::vector<std::shared_ptr<SeResponse>> selectionSeResponses;

    ADSR_AbstractDefaultSelectionsResponseMock response(selectionSeResponses);
}
