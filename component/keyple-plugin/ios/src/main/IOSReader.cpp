/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#include "IOSReader.h"

namespace keyple {
namespace plugin {
namespace ios {

const std::string IOSReader::SETTING_KEY_TRANSMISSION_MODE         = "transmission_mode";
const std::string IOSReader::SETTING_TRANSMISSION_MODE_CONTACTS    = "contacts";
const std::string IOSReader::SETTING_TRANSMISSION_MODE_CONTACTLESS = "contactless";
const std::string IOSReader::SETTING_KEY_PROTOCOL                  = "protocol";
const std::string IOSReader::SETTING_PROTOCOL_T0                   = "T0";
const std::string IOSReader::SETTING_PROTOCOL_T1                   = "T1";
const std::string IOSReader::SETTING_PROTOCOL_T_CL                 = "TCL";
const std::string IOSReader::SETTING_PROTOCOL_TX                   = "Tx";
const std::string IOSReader::SETTING_KEY_MODE                      = "mode";
const std::string IOSReader::SETTING_MODE_EXCLUSIVE                = "exclusive";
const std::string IOSReader::SETTING_MODE_SHARED                   = "shared";
const std::string IOSReader::SETTING_KEY_DISCONNECT                = "disconnect";
const std::string IOSReader::SETTING_DISCONNECT_RESET              = "reset";
const std::string IOSReader::SETTING_DISCONNECT_UNPOWER            = "unpower";
const std::string IOSReader::SETTING_DISCONNECT_LEAVE              = "leave";
const std::string IOSReader::SETTING_DISCONNECT_EJECT              = "eject";

}
}
}