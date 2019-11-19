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

/* Plugin - Stub */
#include "StubSecureElement.h"

/* Core - Seproxy - Exception */
#include "KeypleChannelStateException.h"
#include "KeypleIOReaderException.h"

/* Core - Util */
#include "ByteArrayUtil.h"

/* Common */
#include "stringhelper.h"

namespace keyple {
namespace plugin {
namespace stub {

using namespace keyple::core::seproxy::exception;
using namespace keyple::core::util;

bool StubSecureElement::isPhysicalChannelOpen()
{
    return isPhysicalChannelOpen_Renamed;
}

void StubSecureElement::openPhysicalChannel()
{
    isPhysicalChannelOpen_Renamed = true;
}

void StubSecureElement::closePhysicalChannel()
{
    isPhysicalChannelOpen_Renamed = false;
}

void StubSecureElement::addHexCommand(const std::string &command,
                                      const std::string &response)
{
    if (!command.compare("") || !response.compare("")) {
        logger->debug("either command or response is empty\n");
        return;
    }

    /* Add commands without space */
    hexCommands.emplace(StringHelper::replace(command, " ", ""),
                        StringHelper::replace(response, " ", ""));
}

void StubSecureElement::removeHexCommand(const std::string &command)
{
    if (command.compare(""))
        //"command should not be null"));
        return;

    hexCommands.erase(StringHelper::trim(command));
}

std::vector<char> StubSecureElement::processApdu(std::vector<char> &apduIn)
{
    if (apduIn.empty())
        return apduIn;

    /* Convert apduIn to hexa */
    std::string hexApdu = ByteArrayUtil::toHex(apduIn);

    logger->debug("processApdu - looking for response to %s\n",
                  hexApdu.c_str());

    /* Return matching hexa response if found */
    if (hexCommands.find(hexApdu) != hexCommands.end()) {
        logger->debug("processApdu - response found: %s\n",
                      hexCommands[hexApdu].c_str());
        return ByteArrayUtil::fromHex(hexCommands[hexApdu]);
    }

    /* Throw a KeypleIOReaderException if not found */
    logger->debug("processApdu - response not found\n");
    throw KeypleIOReaderException("No response available for this request.");
}

}
}
}
