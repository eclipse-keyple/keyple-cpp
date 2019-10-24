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

#include "PcscReadersSettings.h"

namespace keyple {
namespace example {
namespace calypso {
namespace pc {

const std::string PcscReadersSettings::PO_READER_NAME_REGEX =
    ".*(ASK|ACS|5421\\(2\\)|00\\ 01|5x21-CL\\ ).*";
const std::string PcscReadersSettings::SAM_READER_NAME_REGEX =
    ".*(Cherry TC|SCM Microsystems|Identive|HID|Generic|00\\ 00" \
    "|5421\\(1\\)|5x21\\ ).*";

}
}
}
}
