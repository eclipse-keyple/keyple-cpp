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

/* Core */
#include "SeCommonProtocols.h"

/* Calypso */
#include "SamSelector.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command::sam;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::protocol;

SamSelector::SamSelector(const SamRevision& samRevision,
                         const std::string& serialNumber,
                         const std::string& extraInfo)
: SeSelector(SeCommonProtocols::PROTOCOL_ISO7816_3,
             std::make_shared<SeSelector::AtrFilter>(""), nullptr, extraInfo)
{
    std::string atrRegex;
    std::string snRegex;

    /* check if serialNumber is defined */
    if (serialNumber == "" || serialNumber.empty()) {
        /* match all serial numbers */
        snRegex = ".{8}";
    } else {
        /* match the provided serial number (could be a regex substring) */
        snRegex = serialNumber;
    }

    /*
     * build the final Atr regex according to the SAM subtype and serial number
     * if any.
     *
     * The header is starting with 3B, its total length is 4 or 6 bytes (8 or 10
     * hex digits)
     */
    switch (samRevision.innerEnumValue) {
    case SamRevision::InnerEnum::C1:
    case SamRevision::InnerEnum::S1D:
    case SamRevision::InnerEnum::S1E:
        atrRegex = "3B(.{6}|.{10})805A..80" +
                   samRevision.getApplicationTypeMask() + "20.{4}" + snRegex +
                   "829000";
        break;
    case SamRevision::InnerEnum::AUTO:
        /* match any ATR */
        atrRegex = ".*";
        break;
    default:
        throw std::invalid_argument("Unknown SAM subtype.");
    }

    this->getAtrFilter()->setAtrRegex(atrRegex);
}

SamSelector::SamSelector(const SamIdentifier& samIdentifier,
                         const std::string& extraInfo)
: SamSelector(samIdentifier.getSamRevision(), samIdentifier.getSerialNumber(),
              extraInfo)
{
}

}
}
}
