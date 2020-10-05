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

#include "GetDataFciRespPars.h"

/* Common */
#include "ClassNotFoundException.h"

/* Core */
#include "ApduResponse.h"
#include "TLV.h"
#include "ByteArrayUtil.h"

/* Calypso */
#include "CalypsoPoDataAccessException.h"
#include "CalypsoPoIllegalParameterException.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::exception;
using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::util;
using namespace keyple::core::util::bertlv;

using StatusProperties = AbstractApduResponseParser::StatusProperties;

const std::map<int, std::shared_ptr<StatusProperties>>
    GetDataFciRespPars::STATUS_TABLE = {
    {
        0x6A88,
        std::make_shared<StatusProperties>(
            "Data object not found (optional mode not available).",
            typeid(CalypsoPoDataAccessException))
    }, {
        0x6B00,
        std::make_shared<StatusProperties>(
            "P1 or P2 value not supported (<>004fh, 0062h, 006Fh, 00C0h, "
            "00D0h, 0185h and 5F52h, according to availabl optional modes).",
            typeid(CalypsoPoIllegalParameterException))
    }, {
        0x6283,
        std::make_shared<StatusProperties>(
            "Successful execution, FCI request and DF is invalidated.",
            typeid(ClassNotFoundException))
    }, {
        0x9000,
        std::make_shared<StatusProperties>("Success")
    }
};

const std::shared_ptr<Tag> GetDataFciRespPars::TAG_FCI_TEMPLATE =
    std::make_shared<Tag>(0x0F, Tag::TagClass::APPLICATION,
                          Tag::TagType::CONSTRUCTED, 1);
const std::shared_ptr<Tag> GetDataFciRespPars::TAG_DF_NAME =
    std::make_shared<Tag>(0x04, Tag::TagClass::CONTEXT,
                          Tag::TagType::PRIMITIVE, 1);
const std::shared_ptr<Tag> GetDataFciRespPars::TAG_FCI_PROPRIETARY_TEMPLATE =
    std::make_shared<Tag>(0x05, Tag::TagClass::CONTEXT,
                          Tag::TagType::CONSTRUCTED, 1);
const std::shared_ptr<Tag>
    GetDataFciRespPars::TAG_FCI_ISSUER_DISCRETIONARY_DATA =
        std::make_shared<Tag>(0x0C, Tag::TagClass::CONTEXT,
                              Tag::TagType::CONSTRUCTED, 2);
const std::shared_ptr<Tag> GetDataFciRespPars::TAG_APPLICATION_SERIAL_NUMBER =
    std::make_shared<Tag>(0x07, Tag::TagClass::PRIVATE,
                          Tag::TagType::PRIMITIVE, 1);
const std::shared_ptr<Tag> GetDataFciRespPars::TAG_DISCRETIONARY_DATA =
    std::make_shared<Tag>(0x13, Tag::TagClass::APPLICATION,
                          Tag::TagType::PRIMITIVE, 1);

const std::map<int, std::shared_ptr<StatusProperties>>&
    GetDataFciRespPars::getStatusTable() const
{
    return STATUS_TABLE;
}

GetDataFciRespPars::GetDataFciRespPars(
  std::shared_ptr<ApduResponse> response, GetDataFciCmdBuild* builder)
: AbstractPoResponseParser(
    response,
    dynamic_cast<AbstractPoCommandBuilder<AbstractPoResponseParser>*>(builder))
{
    std::shared_ptr<TLV> tlv = nullptr;

    /* check the command status to determine if the DF has been invalidated */
    if (response->getStatusCode() == 0x6283) {
        logger->debug("The response to the select application command status"
                      "word indicates that the DF has been invalidated\n");
        mIsDfInvalidated = true;
    }

    /* parse the raw data with the help of the TLV class */
    try {
        /* init TLV object with the raw data and extract the FCI Template */
        std::vector<uint8_t> responseData = response->getBytes();
        tlv = std::make_shared<TLV>(responseData);

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

        mDfName = tlv->getValue();

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

        mApplicationSN = tlv->getValue();

        /* Get the Discretionary Data */
        if (!tlv->parse(TAG_DISCRETIONARY_DATA, tlv->getPosition())) {
            logger->error("FCI parsing error: discretionary data tag not "
                          "found\n");
            return;
        }

        mDiscretionaryData = tlv->getValue();

        logger->debug("Discretionary Data = %\n", mDiscretionaryData);

        /* all 3 main fields were retrieved */
        mIsValidCalypsoFCI = true;

    } catch (const std::runtime_error& e) {
        /* Silently ignore problems decoding TLV structure. Just log. */
        logger->debug("Error while parsing the FCI BER-TLV data structure "
                      "(%)\n", e.what());
    }
}

bool GetDataFciRespPars::isValidCalypsoFCI() const
{
    return mIsValidCalypsoFCI;
}

const std::vector<uint8_t>& GetDataFciRespPars::getDfName() const
{
    return mDfName;
}

const std::vector<uint8_t>& GetDataFciRespPars::getApplicationSerialNumber()
    const
{
    return mApplicationSN;
}

const std::vector<uint8_t>& GetDataFciRespPars::getDiscretionaryData() const
{
    return mDiscretionaryData;
}

bool GetDataFciRespPars::isDfInvalidated() const
{
    return mIsDfInvalidated;
}

}
}
}
}
}
