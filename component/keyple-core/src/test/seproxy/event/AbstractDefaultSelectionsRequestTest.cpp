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

#include "AbstractDefaultSelectionsRequest.h"

using namespace testing;

using namespace keyple::core::seproxy::event;

class ADSR_AbstractDefaultSelectionsRequestMock
: public AbstractDefaultSelectionsRequest {
public:
    ADSR_AbstractDefaultSelectionsRequestMock(
      std::vector<std::shared_ptr<SeRequest>> selectionSeRequests,
      const MultiSeRequestProcessing& multiSeRequestProcessing,
      const ChannelControl& channelControl)
    : AbstractDefaultSelectionsRequest(selectionSeRequests,
      multiSeRequestProcessing, channelControl) {}
};

TEST(AbstractDefaultSelectionsRequestTest, AbstractDefaultSelectionsRequest)
{
    std::vector<std::shared_ptr<SeRequest>> selectionSeRequests;

    ADSR_AbstractDefaultSelectionsRequestMock mock(
        selectionSeRequests, MultiSeRequestProcessing::FIRST_MATCH,
        ChannelControl::CLOSE_AFTER
    );
}
