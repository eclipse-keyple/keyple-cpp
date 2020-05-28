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

#include "AbstractMatchingSe.h"

#include "ApduResponse.h"
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

static std::vector<std::shared_ptr<ApduResponse>> emptyApduVector;

static std::shared_ptr<SeResponse> seResponse =
    std::make_shared<SeResponse>(true, true, nullptr, emptyApduVector);

TEST(AbstractMatchingSeTest, AbstractMatchingSe_Nullptr)
{
    MatchingSeMock matchingSe(nullptr, TransmissionMode::CONTACTLESS, "");
}

TEST(AbstractMatchingSeTest, AbstractMatchingSe)
{
    MatchingSeMock matchingSe(seResponse, TransmissionMode::CONTACTLESS, "");
}

TEST(AbstractMatchingSeTest, isSelected)
{
    MatchingSeMock matchingSe(seResponse, TransmissionMode::CONTACTLESS, "");

    ASSERT_FALSE(matchingSe.isSelected());
}