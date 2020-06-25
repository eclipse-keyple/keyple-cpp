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

#include "SeResource.h"

#include "AbstractMatchingSe.h"
#include "SeResponse.h"

using namespace keyple::core::selection;
using namespace keyple::core::seproxy::message;

using namespace testing;

class MatchingSeMock : public AbstractMatchingSe {
public:
    MatchingSeMock(std::shared_ptr<SeResponse> selectionResponse,
                   TransmissionMode transmissionMode,
                   const std::string& extraInfo)
    : AbstractMatchingSe(selectionResponse, transmissionMode, extraInfo)
    {
    }
};

class LocalSeResourceMock : public SeResource<MatchingSeMock> {
public:
    LocalSeResourceMock(
      std::shared_ptr<SeReader> seReader,
      std::shared_ptr<MatchingSeMock> matchingSe)
    : SeResource(seReader, matchingSe)
    {
    }
};

TEST(SeResourceTest, SeResource)
{
    std::vector<std::shared_ptr<ApduResponse>> empty;
    std::shared_ptr<MatchingSeMock> matchingSe =
        std::make_shared<MatchingSeMock>(
            std::make_shared<SeResponse>(true, true, nullptr, empty),
        TransmissionMode::CONTACTLESS, "extrainfo");

    std::shared_ptr<SeReader> seReader = nullptr;
    std::shared_ptr<LocalSeResourceMock> localSeResource =
        std::make_shared<LocalSeResourceMock>(seReader, matchingSe);

    ASSERT_EQ(localSeResource->getMatchingSe(), matchingSe);
    ASSERT_EQ(localSeResource->getSeReader(), nullptr);
}
