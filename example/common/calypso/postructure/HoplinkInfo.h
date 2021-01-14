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
 * Helper class to provide specific information to handle Hoplink cards.
 * <ul>
 * <li>AID application selection</li>
 * <li>SAM_C1_ATR_REGEX regular expresion matching the expected C1 SAM ATR</li>
 * <li>Files infos (SFI, rec number, etc) for T2 Environment and T2 Usage</li>
 * </ul>
 */
class HoplinkInfo : public std::enable_shared_from_this<HoplinkInfo> {
public:
    /**
     * Hoplink card AID
     */
    static const std::string AID;

    /**
     * SAM C1 regular expression: platform, version and serial number values are ignored
     */
    static const std::string SAM_C1_ATR_REGEX;

    /**
     * Sample data for T2 usage update
     */
    static const std::string t2UsageRecord1_dataFill;

    /**
     *
     */
    static const uint8_t RECORD_NUMBER_1 = 1;
    static const uint8_t RECORD_NUMBER_2 = 2;
    static const uint8_t RECORD_NUMBER_3 = 3;
    static const uint8_t RECORD_NUMBER_4 = 4;

    /**
     *
     */
    static const uint8_t SFI_T2Usage       = 0x1A;
    static const uint8_t SFI_T2Environment = 0x14;

    /**
     *
     */
    static const std::string EXTRAINFO_ReadRecord_T2UsageRec1;
    static const std::string EXTRAINFO_ReadRecord_T2EnvironmentRec1;
};

}
}
}
}
}
