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

#include "SelectFileCmdBuild.h"
#include "SelectFileRespPars.h"
#include "ApduResponse.h"

/* Common */
#include "stringhelper.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::core::seproxy::message;
using namespace keyple::calypso::command::po::builder;

SelectFileCmdBuild::SelectFileCmdBuild(PoClass poClass,
                                       SelectControl selectControl)
: AbstractPoCommandBuilder<SelectFileRespPars>(CalypsoPoCommands::SELECT_FILE,
                                               nullptr)
{
    uint8_t p1;
    uint8_t p2;

    std::vector<uint8_t> selectData = {0x00, 0x00};
    switch (selectControl) {
    case SelectControl::FIRST:
        p1 = 0x02;
        p2 = 0x00;
        break;
    case SelectControl::NEXT:
        p1 = 0x02;
        p2 = 0x02;
        break;
    case SelectControl::CURRENT_DF:
        p1 = 0x09;
        p2 = 0x00;
        break;
    default:
        throw IllegalStateException(StringHelper::formatSimple(
            "Unsupported selectControl parameter %d (see "
            "SelectFileCmdBuild::SelectionControl)",
            selectControl));
    }

    request =
        setApduRequest(poClass.getValue(), command, p1, p2, selectData, 0x00);
}

SelectFileCmdBuild::SelectFileCmdBuild(
    PoClass poClass, const std::vector<uint8_t>& selectionPath)
: AbstractPoCommandBuilder<SelectFileRespPars>(CalypsoPoCommands::SELECT_FILE,
                                               nullptr)
{
    request = setApduRequest(poClass.getValue(), command, 0x09, 0x00,
                             selectionPath, 0x00);
}

std::shared_ptr<SelectFileRespPars> SelectFileCmdBuild::createResponseParser(
    std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<SelectFileRespPars>(apduResponse);
}

std::ostream& operator<<(std::ostream& os,
                         const SelectFileCmdBuild::SelectControl& sc)
{
    if (sc == SelectFileCmdBuild::SelectControl::FIRST)
        os << "FIRST";
    else if (sc == SelectFileCmdBuild::SelectControl::NEXT)
        os << "NEXT";
    else if (sc == SelectFileCmdBuild::SelectControl::CURRENT_DF)
		os << "CURRENT_DF";

	return os;
}

}
}
}
}
}
