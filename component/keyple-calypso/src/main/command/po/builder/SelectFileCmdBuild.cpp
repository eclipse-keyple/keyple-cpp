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

#include "SelectFileCmdBuild.h"

/* Calypso */
#include "SelectFileRespPars.h"

/* Core */
#include "ApduResponse.h"
#include "ByteArrayUtil.h"

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
using namespace keyple::core::util;

SelectFileCmdBuild::SelectFileCmdBuild(
  PoClass poClass, SelectFileControl selectFileControl)
: AbstractPoCommandBuilder<SelectFileRespPars>(
      CalypsoPoCommand::SELECT_FILE, nullptr),
  mPath(path), mSelectFileControl(selectFileControl)
{
    const uint8_t cla = poClass.getValue();
    uint8_t p1;
    uint8_t p2;

    const std::vector<uint8_t> selectData = {0x00, 0x00};
    switch (selectFileControl) {
    case SelectFileControl::FIRST_EF:
        p1 = 0x02;
        p2 = 0x00;
        break;
    case SelectFileControl::NEXT_EF:
        p1 = 0x02;
        p2 = 0x02;
        break;
    case SelectFileControl::CURRENT_DF:
        p1 = 0x09;
        p2 = 0x00;
        break;
    default:
        throw IllegalStateException(StringHelper::formatSimple(
                  "Unsupported selectFileControl parameter %d",
                  selectFileControl));
    }

    request = setApduRequest(cla command, p1, p2, selectData, 0x00);

    const std::string extraInfo =
        StringHelper::formatSimple("SELECTIONCONTROL=%s", selectFileControl);
    addSubName(extraInfo);
}

SelectFileCmdBuild::SelectFileCmdBuild(
  PoClass poClass, const std::vector<uint8_t>& selectionPath)
: AbstractPoCommandBuilder<SelectFileRespPars>(
      CalypsoPoCommands::SELECT_FILE, nullptr),
  mPath(selectionPath), mSelectFileControl(SelectFileControl::NONE)
{
    /* Handle the REV1 case */
    const uint8_t p1 = poClass == PoClass::LEGACY ? 0x08 : 0x09;

    request = setApduRequest(poClass.getValue(), command, p1, 0x00,
                             selectionPath, 0x00);

    const std::string extraInfo =
        StringHelper::formatSimple("SELECTIONPATH=%s",
                                   ByteArrayUtil::toHex(selectionPath));
    addSubName(extraInfo);
}

std::shared_ptr<SelectFileRespPars> SelectFileCmdBuild::createResponseParser(
    std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<SelectFileRespPars>(apduResponse, this);
}

bool SelectFileCmdBuild::isSessionBufferUsed() const
{
    return false;
}

const std::vector<uint8_t>& SelectFileCmdBuild::getPath() const
{
    return mPath;
}

SelectFileControl SelectFileCmdBuild::getSelectFileControl() const
{
    return mSelectFileControl;
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
