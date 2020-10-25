/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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

#include "SamSelectionRequest.h"

/* Calypso */
#include "CalypsoSam.h"
#include "SamRevision.h"
#include "SamSelector.h"

/* Core */
#include "ByteArrayUtil.h"
#include "SeCommonProtocols.h"
#include "SeResponse.h"

using namespace testing;

using namespace keyple::calypso::transaction;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;

static const std::string REVISION = "C1";
static const std::string SN = "11223344";
static const std::string ATR = "3B3F9600805A4880" +
                               REVISION +
                               "205017" +
                               SN +
                               "829000";

TEST(SamSelectionRequestTest, samSelectionRequest_parse)
{
    std::shared_ptr<SamSelector> samSelector =
        std::dynamic_pointer_cast<SamSelector>(
            SamSelector::builder()
                ->seProtocol(SeCommonProtocols::PROTOCOL_ISO7816_3)
                .samRevision(SamRevision::AUTO)
                .build());

    auto samSelectionRequest =
        std::make_shared<SamSelectionRequest>(samSelector);

    auto selectionStatus =
        std::make_shared<SelectionStatus>(
            std::make_shared<AnswerToReset>(ByteArrayUtil::fromHex(ATR)),
            nullptr,
            true);

    std::shared_ptr<CalypsoSam> calypsoSam =
        std::dynamic_pointer_cast<CalypsoSam>(
            samSelectionRequest->parse(
                std::make_shared<SeResponse>(
                    true,
                    true,
                    selectionStatus,
                    std::vector<std::shared_ptr<ApduResponse>>{})));

    /* Minimal checks on the CalypsoSam result */
    ASSERT_EQ(calypsoSam->getSamRevision(), SamRevision::C1);
    ASSERT_EQ(calypsoSam->getSerialNumber(), ByteArrayUtil::fromHex(SN));
}
