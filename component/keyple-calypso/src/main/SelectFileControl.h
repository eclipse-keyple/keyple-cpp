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

#pragma once

namespace keyple {
namespace calypso {

/**
 * Definitions governing the expected behavior of the selection command (see the
 * specifics of this command in the ISO7816-4 standard and the Calypso
 * specification)
 */
enum class SelectFileControl {
    NONE,
    /** The first EF of the current Calypso DF */
    FIRST_EF,
    /** The next EF of the current Calypso DF */
    NEXT_EF,
    /** The Calypso DF */
    CURRENT_DF
};

std::ostream& operator<<(std::ostream& os, const SelectFileControl& sfc)
{
    switch (sfc) {
    case SelectFileControl::NONE:
        os << "NONE";
        break;
    case SelectFileControl::FIRST_EF:
        os << "FIRST EF";
        break;
    case SelectFileControl::NEXT_EF:
        os << "NEXT EF";
        break;
    case SelectFileControl::CURRENT_DF:
        os << "CURRENT EF";
        break;
    default:
        os << "UNKNOWN (" << static_cast<int>(sfc) << ")";
        break;
    }

    return os;
}

}
}
