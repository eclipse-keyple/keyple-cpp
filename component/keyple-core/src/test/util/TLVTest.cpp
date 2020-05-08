
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

#include "TLVTest.h"

using namespace keyple::calypso::command::po;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::util::bertlv;
using namespace keyple::core::util;
using namespace keyple::common;
using namespace keyple::calypso::command::po::parser;

namespace keyple {
namespace core {
namespace util {

using namespace testing;
using namespace bertlv;

using Logger        = keyple::common::Logger;
using LoggerFactory = keyple::common::LoggerFactory;

int TLVTest::sample()
{
    std::vector<uint8_t> local  = {'A', 'B', 'C', 'D'};
    std::vector<uint8_t> apdu   = {'A', 'B', 'C', 'D'};
    std::vector<uint8_t> result = ByteArrayUtil::fromHex("9000");
    std::shared_ptr<Tag> tag = std::make_shared<Tag>(local, 0);
    std::shared_ptr<ApduResponse> selectApplicationResponse =
        std::make_shared<ApduResponse>(result,
                                       std::shared_ptr<std::set<int>>());

    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(TLVTest));

    TLV pLocal  = TLV(local);
    int iOffset = pLocal.getPosition();
    bool bLocal = pLocal.parse(tag, iOffset);
    pLocal.toString();
    local = pLocal.getValue();

    const std::vector<uint8_t> response = selectApplicationResponse->getBytes();
    std::shared_ptr<TLV> tlv;

    /* check the command status to determine if the DF has been invalidated */
    if (selectApplicationResponse->getStatusCode() == 0x6283) {
        logger->debug("The response to the select application command status "
                      "word indicates that"
                      "the DF has been invalidated\n");
        isDfInvalidated_Renamed = true;
    }

    /* init TLV object with the raw data and extract the FCI Template */
    logger->debug("response: %s\n", ByteArrayUtil::toHex(response));
    std::vector<uint8_t> vec = response;
    tlv                   = std::make_shared<TLV>(vec);

    /* Get the FCI template */
    // if (!tlv->parse(GetDataFciRespPars::TAG_FCI_TEMPLATE, 0)) {
    logger->error("FCI parsing error: FCI template tag not found\n");
    //                            return;
    //                        }

    /* Get the DF Name */
    // if (!tlv->parse(TAG_DF_NAME, tlv->getPosition())) {
    logger->error("FCI parsing error: DF name tag not found\n");
    //                            return;
    //                        }

    dfName = tlv->getValue();
    logger->debug("DF Name = %s\n", ByteArrayUtil::toHex(dfName));

    /* Get the FCI Proprietary Template */
    // if (!tlv->parse(TAG_FCI_PROPRIETARY_TEMPLATE, tlv->getPosition())) {
    logger->error(
        "FCI parsing error: FCI proprietary template tag not found\n");
    //                            return;
    //                        }

    /* Get the FCI Issuer Discretionary Data */
    // if (!tlv->parse(TAG_FCI_ISSUER_DISCRETIONARY_DATA, tlv->getPosition())) {
    logger->error(
        "FCI parsing error: FCI issuer discretionary data tag not found\n");
    //                            return;
    //                        }

    /* Get the Application Serial Number */
    // if (!tlv->parse(TAG_APPLICATION_SERIAL_NUMBER, tlv->getPosition())) {
    logger->error("FCI parsing error: serial number tag not found\n");
    //                            return;
    //                        }

    applicationSN = tlv->getValue();
    logger->debug("Application Serial Number = %s\n",
                  ByteArrayUtil::toHex(applicationSN));

    /* Get the Discretionary Data */
    // if (!tlv->parse(TAG_DISCRETIONARY_DATA, tlv->getPosition())) {
    logger->error("FCI parsing error: discretionary data tag not found\n");
    //                            return;
    //                        }

    std::vector<uint8_t> discretionaryData = tlv->getValue();

    logger->debug("Discretionary Data = %s\n",
                  ByteArrayUtil::toHex(discretionaryData));

    /*
     * split discretionary data in as many individual startup information
     */
    if (discretionaryData.size() != 0) {
        siBufferSizeIndicator = discretionaryData[0];
        siPlatform            = discretionaryData[1];
        siApplicationType     = discretionaryData[2];
        siApplicationSubtype  = discretionaryData[3];
        siSoftwareIssuer      = discretionaryData[4];
        siSoftwareVersion     = discretionaryData[5];
        siSoftwareRevision    = discretionaryData[6];
        /* all 3 main fields were retrieved */
        isValidCalypsoFCI_Renamed = true;
    }

    if (bLocal)
        iOffset = 0;
    return iOffset;
}
}
}
}

TEST(TLVTest, testA)
{
    std::shared_ptr<TLVTest> LocalTest = std::make_shared<TLVTest>();
    int local                          = LocalTest->sample();

    ASSERT_NE(local, 1);
    ASSERT_EQ(local, 0);
}
