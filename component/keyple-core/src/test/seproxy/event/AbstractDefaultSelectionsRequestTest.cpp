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

class AbstractDefaultSelectionsRequestMock
: public AbstractDefaultSelectionsRequest {
protected:
    const std::vector<std::shared_ptr<SeRequest>>& getSelectionSeRequestSet()
        const
    {
        return requests;
    }

    const MultiSeRequestProcessing& getMultiSeRequestProcessing() const
    {
        return processing;
    }

    const ChannelControl& getChannelControl() const
    {
	    return channelControl;
    }

private:
    std::vector<std::shared_ptr<SeRequest>> requests;

    MultiSeRequestProcessing processing;

    ChannelControl channelControl;
};

TEST(AbstractDefaultSelectionsRequestTest, AbstractDefaultSelectionsRequest)
{
	AbstractDefaultSelectionsRequestMock mock;
}

