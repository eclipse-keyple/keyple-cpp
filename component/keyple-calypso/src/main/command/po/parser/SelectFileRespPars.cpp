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

#include "SelectFileRespPars.h"

/* Common */
#include "ClassNotFoundException.h"
#include "IllegalArgumentException.h"
#include "IllegalStateException.h"
#include "stringhelper.h"
#include "System.h"

/* Calypso */
#include "CalypsoPoDataAccessException.h"
#include "CalypsoPoIllegalParameterException.h"

/* Core */
#include "KeypleAssert.h"
#include "TLV.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using namespace keyple::calypso::command::po::exception;
using namespace keyple::common;
using namespace keyple::core::command;
using namespace keyple::core::util;

using StatusProperties = AbstractApduResponseParser::StatusProperties;

const Tag SelectFileRespPars::TAG_PROPRIETARY_INFORMATION(
     0x05, Tag::TagClass::CONTEXT, Tag::TagType::PRIMITIVE, 1);

const std::map<int, std::shared_ptr<StatusProperties>>
    SelectFileRespPars::STATUS_TABLE = {
    {
        0x6700,
        std::make_shared<StatusProperties>(
            "Lc value not supported.",
            typeid(CalypsoPoIllegalParameterException))
    }, {
        0x6A82,
        std::make_shared<StatusProperties>(
            "File not found.",
            typeid(CalypsoPoDataAccessException))
    }, {
        0x6119,
        std::make_shared<StatusProperties>(
            "Correct execution (ISO7816 T=0).",
            typeid(ClassNotFoundException))
    }, {
        0x9000,
        std::make_shared<StatusProperties>("Success")
    }
};

SelectFileRespPars::SelectFileRespPars(
  std::shared_ptr<ApduResponse> response, SelectFileCmdBuild* builder)
: AbstractPoResponseParser(
 response,
 reinterpret_cast<AbstractPoCommandBuilder<AbstractPoResponseParser>*>(builder))
{
    mProprietaryInformation.clear();
}


const std::map<int, std::shared_ptr<StatusProperties>>&
    SelectFileRespPars::getStatusTable() const
{
    return STATUS_TABLE;
}

const std::vector<uint8_t>& SelectFileRespPars::getProprietaryInformation()
{
    if (mProprietaryInformation.size() == 0) {
        TLV tlv(mResponse->getDataOut());

        if (!tlv.parse(std::make_shared<Tag>(TAG_PROPRIETARY_INFORMATION), 0))
            throw IllegalStateException(
                      "Proprietary information: tag not found.");

        mProprietaryInformation = tlv.getValue();

        KeypleAssert::getInstance().isEqual(mProprietaryInformation.size(),
                                            23,
                                            "proprietaryInformation");
    }

    return mProprietaryInformation;
}

}
}
}
}
}
