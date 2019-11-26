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

#include "CalypsoClassicInfo.h"

namespace keyple {
namespace example {
namespace calypso {
namespace common {
namespace postructure {

const std::string CalypsoClassicInfo::AID = "A0000004040125090101";
//const std::string CalypsoClassicInfo::AID = "315449432E49434131";
const std::string CalypsoClassicInfo::SAM_C1_ATR_REGEX =
    "3B3F9600805A[0-9a-fA-F]{2}80C1[0-9a-fA-F]{14}829000";
const std::string CalypsoClassicInfo::ATR_REV1_REGEX =
    "3B8F8001805A0A0103200311........829000..";
const std::string CalypsoClassicInfo::eventLog_dataFill =
    "00112233445566778899AABBCCDDEEFF00112233445566778899AABBCC";

}
}
}
}
}
