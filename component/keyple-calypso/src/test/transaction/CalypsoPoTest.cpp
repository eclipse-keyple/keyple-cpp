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

#include "ByteArrayUtil.h"
#include "CalypsoPo.h"
#include "CalypsoPoTest.h"
#include "SeSelector.h"
#include "SeResponse.h"

#include "ApduResponse.h"
#include "SelectionStatus.h"
#include "SeCommonProtocols.h"

using namespace keyple::calypso::transaction;

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command::po;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;

using IsoAid = keyple::core::seproxy::SeSelector::AidSelector::IsoAid;

//std::shared_ptr<CalypsoPo>
PoRevision CalypsoPoTest::getPoApplicationByte(uint8_t applicationByte)
{
    char cBuffer[256];

    snprintf(cBuffer, sizeof(cBuffer),
             "6F 22 84 08 315449432E494341 A5 16 BF0C 13 C7 08 "
             "0000000011223344 53 07 060A %02X 02200311 9000",
             applicationByte);
    std::string szResp      = cBuffer;
    std::vector<uint8_t> cResp = ByteArrayUtil::fromHex(szResp);
    std::shared_ptr<ApduResponse> fciData =
        std::make_shared<ApduResponse>(cResp, nullptr);
    std::shared_ptr<AnswerToReset> atrData =
        std::make_shared<AnswerToReset>(cResp);
    std::vector<std::shared_ptr<ApduResponse>> apduVecteurReponseVide = {
        nullptr};
    SeCommonProtocols seCommonProtocols =
        SeCommonProtocols::PROTOCOL_ISO14443_4;
    std::shared_ptr<SeResponse> selectionData = std::make_shared<SeResponse>(
        true, false, std::make_shared<SelectionStatus>(atrData, fciData, true),
        apduVecteurReponseVide);

    TransmissionMode transm = TransmissionMode::NO_MODE;
    std::string extrainfo   = "";
    try {
        /* code */
        // Ca plante parce qu'il n'y a pas d'ATR dans CalypsoPO / SelectionStatus !!!
        std::shared_ptr<CalypsoPo> calypsoPo =
            std::make_shared<CalypsoPo>(selectionData, transm, extrainfo);
        return calypsoPo->getRevision();
    } catch (...) {
    }
    return PoRevision::NO_REV;
}

void CalypsoPoTest::computePoRevision()
{
    ASSERT_EQ(getPoApplicationByte(0x01), PoRevision::REV2_4);
    ASSERT_EQ(getPoApplicationByte(0x04), PoRevision::REV2_4);
    ASSERT_EQ(getPoApplicationByte(0x06), PoRevision::REV2_4);
    ASSERT_EQ(getPoApplicationByte(0x1F), PoRevision::REV2_4);
    ASSERT_EQ(getPoApplicationByte(0x20), PoRevision::REV3_1);
    ASSERT_EQ(getPoApplicationByte(0x27), PoRevision::REV3_1);
    ASSERT_EQ(getPoApplicationByte(0x28), PoRevision::REV3_2);
    ASSERT_EQ(getPoApplicationByte(0x2F), PoRevision::REV3_2);
}

}
}
}

TEST(CalypsoPoTest, testA)
{
    std::shared_ptr<CalypsoPoTest> LocalTest =
        std::make_shared<CalypsoPoTest>();
    LocalTest->computePoRevision();
}
