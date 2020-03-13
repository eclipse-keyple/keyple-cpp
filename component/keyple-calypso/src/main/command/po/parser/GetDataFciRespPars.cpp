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

/* Calypso.h */
#include "GetDataFciRespPars.h"
#include "ApduResponse.h"
#include "TLV.h"

/* Core */
#include "ByteArrayUtil.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using namespace keyple::calypso::command::po;
using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::util;
using namespace keyple::core::util::bertlv;

using StatusProperties = AbstractApduResponseParser::StatusProperties;

std::unordered_map<int, std::shared_ptr<StatusProperties>>
    GetDataFciRespPars::STATUS_TABLE;

GetDataFciRespPars::StaticConstructor::StaticConstructor()
{
    std::unordered_map<int, std::shared_ptr<StatusProperties>> m(
        AbstractApduResponseParser::STATUS_TABLE);

    m.emplace(
        0x6A88,
        std::make_shared<StatusProperties>(
            false, "Data object not found (optional mode not available)."));
    m.emplace(
        0x6B00,
        std::make_shared<StatusProperties>(
            false,
            "P1 or P2 value not supported (<>004fh, 0062h, 006Fh, 00C0h, "
            "00D0h, 0185h and 5F52h, according to availabl optional modes)."));
    m.emplace(
        0x6283,
        std::make_shared<StatusProperties>(
            true, "Successful execution, FCI request and DF is invalidated."));

    STATUS_TABLE = m;
}

GetDataFciRespPars::StaticConstructor GetDataFciRespPars::staticConstructor;

std::unordered_map<int, std::shared_ptr<StatusProperties>>
GetDataFciRespPars::getStatusTable() const
{
    return STATUS_TABLE;
}

std::vector<int> const
    GetDataFciRespPars::BUFFER_SIZE_INDICATOR_TO_BUFFER_SIZE = std::vector<int>{
        0,      0,      0,      0,      0,      0,      215,    256,
        304,    362,    430,    512,    608,    724,    861,    1024,
        1217,   1448,   1722,   2048,   2435,   2896,   3444,   4096,
        4870,   5792,   6888,   8192,   9741,   11585,  13777,  16384,
        19483,  23170,  27554,  32768,  38967,  46340,  55108,  65536,
        77935,  92681,  110217, 131072, 155871, 185363, 220435, 262144,
        311743, 370727, 440871, 524288, 623487, 741455, 881743, 1048576};

const std::shared_ptr<Tag> GetDataFciRespPars::TAG_FCI_TEMPLATE =
    std::make_shared<Tag>(0x0F, Tag::APPLICATION, Tag::TagType::CONSTRUCTED);
const std::shared_ptr<Tag> GetDataFciRespPars::TAG_DF_NAME =
    std::make_shared<Tag>(0x04, Tag::CONTEXT, Tag::TagType::PRIMITIVE);
const std::shared_ptr<Tag> GetDataFciRespPars::TAG_FCI_PROPRIETARY_TEMPLATE =
    std::make_shared<Tag>(0x05, Tag::CONTEXT, Tag::TagType::CONSTRUCTED);
const std::shared_ptr<Tag>
    GetDataFciRespPars::TAG_FCI_ISSUER_DISCRETIONARY_DATA =
        std::make_shared<Tag>(0x0C, Tag::CONTEXT, Tag::TagType::CONSTRUCTED);
const std::shared_ptr<Tag> GetDataFciRespPars::TAG_APPLICATION_SERIAL_NUMBER =
    std::make_shared<Tag>(0x07, Tag::PRIVATE, Tag::TagType::PRIMITIVE);
const std::shared_ptr<Tag> GetDataFciRespPars::TAG_DISCRETIONARY_DATA =
    std::make_shared<Tag>(0x13, Tag::APPLICATION, Tag::TagType::PRIMITIVE);

GetDataFciRespPars::GetDataFciRespPars(
    std::shared_ptr<ApduResponse> selectApplicationResponse)
: AbstractPoResponseParser(selectApplicationResponse)
{

    const std::vector<uint8_t> response = selectApplicationResponse->getBytes();
    std::shared_ptr<TLV> tlv;

    /* check the command status to determine if the DF has been invalidated */
    if (selectApplicationResponse->getStatusCode() == 0x6283) {
        logger->debug("The response to the select application command status"
                      "word indicates that the DF has been invalidated\n");
        isDfInvalidated_Renamed = true;
    }

    /* parse the raw data with the help of the TLV class */
    try {
        /* init TLV object with the raw data and extract the FCI Template */
        logger->debug("response: %s\n", ByteArrayUtil::toHex(response).c_str());
        std::vector<uint8_t> vec = response;
        tlv                      = std::make_shared<TLV>(vec);

        /* Get the FCI template */
        if (!tlv->parse(TAG_FCI_TEMPLATE, 0)) {
            logger->error("FCI parsing error: FCI template tag not found\n");
            return;
        }

        /* Get the DF Name */
        if (!tlv->parse(TAG_DF_NAME, tlv->getPosition())) {
            logger->error("FCI parsing error: DF name tag not found\n");
            return;
        }

        dfName = tlv->getValue();
        logger->debug("DF Name = %s\n", ByteArrayUtil::toHex(dfName).c_str());

        /* Get the FCI Proprietary Template */
        if (!tlv->parse(TAG_FCI_PROPRIETARY_TEMPLATE, tlv->getPosition())) {
            logger->error("FCI parsing error: FCI proprietary template tag "
                          "not found\n");
            return;
        }

        /* Get the FCI Issuer Discretionary Data */
        if (!tlv->parse(TAG_FCI_ISSUER_DISCRETIONARY_DATA,
                        tlv->getPosition())) {
            logger->error("FCI parsing error: FCI issuer discretionary data "
                          "tag not found\n");
            return;
        }

        /* Get the Application Serial Number */
        if (!tlv->parse(TAG_APPLICATION_SERIAL_NUMBER, tlv->getPosition())) {
            logger->error("FCI parsing error: serial number tag not found\n");
            return;
        }

        applicationSN = tlv->getValue();
        logger->debug("Application Serial Number = %s\n",
                      ByteArrayUtil::toHex(applicationSN).c_str());

        /* Get the Discretionary Data */
        if (!tlv->parse(TAG_DISCRETIONARY_DATA, tlv->getPosition())) {
            logger->error("FCI parsing error: discretionary data tag not "
                          "found\n");
            return;
        }

        std::vector<uint8_t> discretionaryData = tlv->getValue();

        if (logger->isDebugEnabled()) {
            logger->debug("Discretionary Data = %s\n",
                          ByteArrayUtil::toHex(discretionaryData).c_str());
        }

        /*
         * split discretionary data in as many individual startup information
         */
        siBufferSizeIndicator = discretionaryData[0];
        siPlatform            = discretionaryData[1];
        siApplicationType     = discretionaryData[2];
        siApplicationSubtype  = discretionaryData[3];
        siSoftwareIssuer      = discretionaryData[4];
        siSoftwareVersion     = discretionaryData[5];
        siSoftwareRevision    = discretionaryData[6];
        /* all 3 main fields were retrieved */
        isValidCalypsoFCI_Renamed = true;

    } catch (const std::runtime_error& e) {
        /* Silently ignore problems decoding TLV structure. Just log. */
        logger->debug("Error while parsing the FCI BER-TLV data structure "
                      "(%s)\n",
                      e.what());
    }
}

bool GetDataFciRespPars::isValidCalypsoFCI()
{
    return isValidCalypsoFCI_Renamed;
}

const std::vector<uint8_t>& GetDataFciRespPars::getDfName() const
{
    return dfName;
}

const std::vector<uint8_t>&
GetDataFciRespPars::getApplicationSerialNumber() const
{
    return applicationSN;
}

uint8_t GetDataFciRespPars::getBufferSizeIndicator()
{
    return siBufferSizeIndicator;
}

int GetDataFciRespPars::getBufferSizeValue()
{
    return BUFFER_SIZE_INDICATOR_TO_BUFFER_SIZE[getBufferSizeIndicator()];
}

uint8_t GetDataFciRespPars::getPlatformByte()
{
    return siPlatform;
}

uint8_t GetDataFciRespPars::getApplicationTypeByte()
{
    return siApplicationType;
}

bool GetDataFciRespPars::isRev3_2ModeAvailable()
{
    return (siApplicationType & APP_TYPE_CALYPSO_REV_32_MODE) != 0;
}

bool GetDataFciRespPars::isRatificationCommandRequired()
{
    return (siApplicationSubtype & APP_TYPE_RATIFICATION_COMMAND_REQUIRED) != 0;
}

bool GetDataFciRespPars::hasCalypsoStoredValue()
{
    return (siApplicationSubtype & APP_TYPE_WITH_CALYPSO_SV) != 0;
}

bool GetDataFciRespPars::hasCalypsoPin()
{
    return (siApplicationSubtype & APP_TYPE_WITH_CALYPSO_PIN) != 0;
}

uint8_t GetDataFciRespPars::getApplicationSubtypeByte()
{
    return siApplicationSubtype;
}

uint8_t GetDataFciRespPars::getSoftwareIssuerByte()
{
    return siSoftwareIssuer;
}

uint8_t GetDataFciRespPars::getSoftwareVersionByte()
{
    return siSoftwareVersion;
}

uint8_t GetDataFciRespPars::getSoftwareRevisionByte()
{
    return siSoftwareRevision;
}

bool GetDataFciRespPars::isDfInvalidated()
{
    return isDfInvalidated_Renamed;
}

}
}
}
}
}
