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

#include "PcscReader.h"

namespace keyple {
namespace plugin {
namespace pcsc {

const std::string PcscReader::SETTING_KEY_TRANSMISSION_MODE =
    "transmission_mode";
const std::string PcscReader::SETTING_TRANSMISSION_MODE_CONTACTS = "contacts";
const std::string PcscReader::SETTING_TRANSMISSION_MODE_CONTACTLESS =
    "contactless";
const std::string PcscReader::SETTING_KEY_PROTOCOL = "protocol";
const std::string PcscReader::SETTING_PROTOCOL_T0 = "TO";
const std::string PcscReader::SETTING_PROTOCOL_T1 = "T1";
const std::string PcscReader::SETTING_PROTOCOL_T_CL = "TCL";
const std::string PcscReader::SETTING_PROTOCOL_TX = "Tx";
const std::string PcscReader::SETTING_KEY_MODE = "mode";
const std::string PcscReader::SETTING_MODE_EXCLUSIVE = "exclusive";
const std::string PcscReader::SETTING_MODE_SHARED = "shared";
const std::string PcscReader::SETTING_KEY_DISCONNECT = "disconnect";
const std::string PcscReader::SETTING_DISCONNECT_RESET = "reset";
const std::string PcscReader::SETTING_DISCONNECT_UNPOWER = "unpower";
const std::string PcscReader::SETTING_DISCONNECT_LEAVE = "leave";
const std::string PcscReader::SETTING_DISCONNECT_EJECT = "eject";
const std::string PcscReader::SETTING_KEY_THREAD_TIMEOUT =
    "thread_wait_timeout";

}
}
}
