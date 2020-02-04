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

#include <iostream>

#include "AbstractIso7816CommandBuilder.h"
#include "ApduRequest.h"

/* Core */
#include "CommandsTable.h"

/* Common */
#include "System.h"

namespace keyple {
namespace core {
namespace command {

using namespace keyple::core::seproxy::message;
using namespace keyple::core::command;

AbstractIso7816CommandBuilder::AbstractIso7816CommandBuilder(
  CommandsTable& commandReference, std::shared_ptr<ApduRequest> request)
: AbstractApduCommandBuilder(commandReference, request)
{
}

AbstractIso7816CommandBuilder::AbstractIso7816CommandBuilder(
  const std::string &name, std::shared_ptr<ApduRequest> request)
: AbstractApduCommandBuilder(name, request)
{
}

std::shared_ptr<ApduRequest> AbstractIso7816CommandBuilder::setApduRequest(
    const uint8_t cla, const CommandsTable& command, const uint8_t p1,
    const uint8_t p2, const std::vector<uint8_t> &dataIn)
{
    bool case4;

    /* sanity check */
    if (dataIn.size() == 0) {
        throw std::invalid_argument("There must be incoming data when Le is " \
                                    "not present");
    }

    /* Buffer allocation */
    int length = 4; // header
    if (dataIn.size() > 0) {
        length += dataIn.size() + 1; // Lc + data
    }

    std::vector<uint8_t> apdu(length);

    /* Build APDU buffer from provided arguments */
    apdu[0] = cla;
    apdu[1] = command.getInstructionByte();
    apdu[2] = p1;
    apdu[3] = p2;

    /* append Lc and ingoing data */
    apdu[4] = dataIn.size();
    System::arraycopy(dataIn, 0, apdu, 5, dataIn.size());

    /*
     * ISO7618 case determination and Le management
     *
     * Only one case possible here:
     * - case3: ingoing data only, no Le
     */
    case4 = false;

    return std::make_shared<ApduRequest>(command.getName(), apdu, case4);
}

std::shared_ptr<ApduRequest> AbstractIso7816CommandBuilder::setApduRequest(
    const uint8_t cla, const CommandsTable& command, const uint8_t p1,
    const uint8_t p2, const uint8_t le)
{
    bool case4;

    /* sanity check */
    if (le == 0) {
        throw std::invalid_argument("Le cannot equal 0 when there is no "\
                                    "incoming data");
    }

    /* Buffer allocation */
    int length = 4; // header

    /* Le */
    length += 1;

    std::vector<uint8_t> apdu(length);

    /* Build APDU buffer from provided arguments */
    apdu[0] = cla;
    apdu[1] = command.getInstructionByte();
    apdu[2] = p1;
    apdu[3] = p2;

    /*
     * ISO7618 case determination and Le management
     *
     * Only one case possible here:
     * - case2: outgoing data only
     */
    apdu[4] = le;
    case4 = false;

    return std::make_shared<ApduRequest>(command.getName(), apdu, case4);
}

std::shared_ptr<ApduRequest> AbstractIso7816CommandBuilder::setApduRequest(
    const uint8_t cla, const CommandsTable& command, const uint8_t p1,
    const uint8_t p2, const std::vector<uint8_t> &dataIn, const uint8_t le)
{
    bool case4;

    /* sanity check */
    if (dataIn.size() == 0 || le != 0) {
        throw std::invalid_argument("Le must be equal to 0 when not null and " \
                                    "ingoing data are present.");
    }

    /* Buffer allocation */
    int length = 4; // header
    if (dataIn.size() > 0) {
        length += dataIn.size() + 1; // Lc + data
    }


    length += 1; // Le

    std::vector<uint8_t> apdu(length);

    /* Build APDU buffer from provided arguments */
    apdu[0] = cla;
    apdu[1] = command.getInstructionByte();
    apdu[2] = p1;
    apdu[3] = p2;

    /* ISO7618 case determination and Le management */
    if (dataIn.size() > 0) {
        /* append Lc and ingoing data */
        apdu[4] = dataIn.size();
        System::arraycopy(dataIn, 0, apdu, 5, dataIn.size());
        /*
         * case4: ingoing and outgoing data, Le is always set to 0 (see
         * Calypso Reader Recommendations - T84)
         */
        case4 = true;
        apdu[length - 1] = le;

    } else {
        /* case2: outgoing data only */
        apdu[4] = le;
        case4 = false;
    }

    return std::make_shared<ApduRequest>(command.getName(), apdu, case4);
}

}
}
}
