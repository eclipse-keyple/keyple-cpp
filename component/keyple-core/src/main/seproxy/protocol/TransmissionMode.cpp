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

#include "TransmissionMode.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace protocol {

std::ostream& operator<<(std::ostream& os, const TransmissionMode& tm)
{
    os << "TRANMISSIONMODE = ";

    if (tm == TransmissionMode::CONTACTLESS)
        os << "CONTACTLESS";
    else if (tm == TransmissionMode::CONTACTS)
        os << "CONTACTS";
    else if (tm == TransmissionMode::NONE)
        os << "NONE";
    else
        os << "UNEXPECTED VALUE";

    return os;
}

}
}
}
}
