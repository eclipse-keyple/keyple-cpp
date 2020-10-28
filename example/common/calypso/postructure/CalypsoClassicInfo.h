/**************************************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                                                *
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

#pragma once

#include <string>
#include <memory>

namespace keyple {
namespace example {
namespace calypso {
namespace common {
namespace postructure {

/**
 * Helper class to provide specific elements to handle Calypso cards.
 * <ul>
 * <li>AID application selection (default Calypso AID)</li>
 * <li>SAM_C1_ATR_REGEX regular expression matching the expected C1 SAM ATR</li>
 * <li>Files infos (SFI, rec number, etc) for
 * <ul>
 * <li>Environment and Holder</li>
 * <li>Event Log</li>
 * <li>Contract List</li>
 * <li>Contracts</li>
 * </ul>
 * </li>
 * </ul>
 */
class CalypsoClassicInfo
: public std::enable_shared_from_this<CalypsoClassicInfo> {
    /** Calypso default AID */
public:
    static const std::string AID;
    /// ** 1TIC.ICA AID */
    // public final static String AID = "315449432E494341";
    /** SAM C1 regular expression: platform, version and serial number values are ignored */
    static const std::string SAM_C1_ATR_REGEX;

    static const std::string ATR_REV1_REGEX;

    static const uint8_t RECORD_NUMBER_1 = 1;
    static const uint8_t RECORD_NUMBER_2 = 2;
    static const uint8_t RECORD_NUMBER_3 = 3;
    static const uint8_t RECORD_NUMBER_4 = 4;

    static const uint8_t SFI_EnvironmentAndHolder = 0x07;
    static const uint8_t SFI_EventLog             = 0x08;
    static const uint8_t SFI_ContractList         = 0x1E;
    static const uint8_t SFI_Contracts            = 0x09;

    static const uint16_t LID_DF_RT = 0x2000;
    static const uint16_t LID_EventLog = 0x2010;

    static const std::string eventLog_dataFill;
};

}
}
}
}
}
