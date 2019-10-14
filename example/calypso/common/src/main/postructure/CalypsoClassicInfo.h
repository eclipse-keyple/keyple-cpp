/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#pragma once

#include <string>
#include <memory>


namespace org {
namespace eclipse {
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
class CalypsoClassicInfo : public std::enable_shared_from_this<CalypsoClassicInfo> {
    /** Calypso default AID */
public:
    static const std::string AID;
    /// ** 1TIC.ICA AID */
    // public final static String AID = "315449432E494341";
    /** SAM C1 regular expression: platform, version and serial number values are ignored */
    static const std::string SAM_C1_ATR_REGEX;

    static const std::string ATR_REV1_REGEX;

    static constexpr char RECORD_NUMBER_1 = 1;
    static constexpr char RECORD_NUMBER_2 = 2;
    static constexpr char RECORD_NUMBER_3 = 3;
    static constexpr char RECORD_NUMBER_4 = 4;

    static const char SFI_EnvironmentAndHolder = static_cast<char>(0x07);
    static const char SFI_EventLog = static_cast<char>(0x08);
    static const char SFI_ContractList = static_cast<char>(0x1E);
    static const char SFI_Contracts = static_cast<char>(0x09);

    static const std::string eventLog_dataFill;
};

}
}
}
}
}
}
}
