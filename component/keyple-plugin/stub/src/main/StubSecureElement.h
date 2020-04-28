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
#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/* Common */
#include "stringhelper.h"
#include "LoggerFactory.h"

/* Core - Seproxy - Exception */
#include "KeypleChannelControlException.h"
#include "KeypleIOReaderException.h"

/* Plugin */
#include "KeyplePluginStubExport.h"

namespace keyple {
namespace plugin {
namespace stub {

using namespace keyple::core::seproxy::exception;
using namespace keyple::common;

class KEYPLEPLUGINSTUB_API StubSecureElement
: public std::enable_shared_from_this<StubSecureElement> {
public:
    /**
     *
     */
    virtual ~StubSecureElement()
    {
    }

    /**
     * Getter for ATR
     *
     * @return Secured Element ATR
     */
    virtual const std::vector<uint8_t>& getATR() = 0;

    /**
     *
     */
    bool isPhysicalChannelOpen_Renamed = false;

    /**
     *
     */
    virtual bool isPhysicalChannelOpen();

    /**
     *
     */
    virtual void openPhysicalChannel();

    /**
     *
     */
    virtual void closePhysicalChannel();

    /**
     * @return SE protocol supported by the SE
     */
    virtual std::string getSeProcotol() = 0;

    /**
     *
     */
    std::unordered_map<std::string, std::string> hexCommands;

    /**
     * Add more simulated commands to the Stub SE
     *
     * @param command : hexadecimal command to react to
     * @param response : hexadecimal response to be sent in reaction to command
     */
    virtual void addHexCommand(const std::string& command,
                               const std::string& response);

    /**
     * Remove simulated commands from the Stub SE
     *
     * @param command : hexadecimal command to be removed
     */
    virtual void removeHexCommand(const std::string& command);

    /**
     * Return APDU Response to APDU Request
     *
     * @param apduIn : commands to be processed
     * @return APDU response
     * @throws KeypleIOReaderException if the transmission fails
     */
    virtual std::vector<uint8_t> processApdu(
        const std::vector<uint8_t>& apduIn);

	/**
	 *
	 */
	friend std::ostream& operator<<(std::ostream& os,
		                            const StubSecureElement& s);

	/**
	 *
	 */
    friend std::ostream& operator<<(
		std::ostream& os, const std::shared_ptr<StubSecureElement>& s);

private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(StubSecureElement));
};

}
}
}
