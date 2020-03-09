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

#pragma once

#include <string>

/* Common */
#include "Export.h"

/* Core */
#include "ObservableReader_Import.h"

namespace keyple {
namespace plugin {
namespace pcsc {

using namespace keyple::core::seproxy::event;

class EXPORT PcscReader : public ObservableReader {
public:
    /**
     *
     */
    static const std::string SETTING_KEY_TRANSMISSION_MODE;
    static const std::string SETTING_TRANSMISSION_MODE_CONTACTS;
    static const std::string SETTING_TRANSMISSION_MODE_CONTACTLESS;
    static const std::string SETTING_KEY_PROTOCOL;
    static const std::string SETTING_PROTOCOL_T0;
    static const std::string SETTING_PROTOCOL_T1;
    static const std::string SETTING_PROTOCOL_T_CL;
    static const std::string SETTING_PROTOCOL_TX;
    static const std::string SETTING_KEY_MODE;
    static const std::string SETTING_MODE_EXCLUSIVE;
    static const std::string SETTING_MODE_SHARED;
    static const std::string SETTING_KEY_DISCONNECT;
    static const std::string SETTING_DISCONNECT_RESET;
    static const std::string SETTING_DISCONNECT_UNPOWER;
    static const std::string SETTING_DISCONNECT_LEAVE;
    static const std::string SETTING_DISCONNECT_EJECT;
};

}
}
}
