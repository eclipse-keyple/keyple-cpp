/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
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

/* Common */
#include "LoggerFactory.h"

/* Plugin */
#include "KeyplePluginIOSExport.h"
#include "IOSTerminalImpl.h"

namespace keyple {
namespace plugin {
namespace ios {

using namespace keyple::common;

class KEYPLEPLUGINIOS_API IOSTerminal {
public:
    /**
     *
     */
    explicit IOSTerminal(const std::string& name);

    /**
     *
     */
    const std::string& getName() const;

    /**
     *
     */
    bool isCardPresent();

    /**
     *
     */
    bool didUserCancelOrTimeout();

    /**
     *
     */
    void waitForCardPresent();

    /**
     *
     */
    void stopWaitForCard();

    /**
     *
     */
    void waitForCardAbsent();

    /**
     *
     */
    void openAndConnect(const std::string& protocol);

    /**
     *
     */
    void closeAndDisconnect(bool reset);

    /**
     *
     */
    static const std::vector<std::string>& listTerminals();

    /**
     *
     */
    const std::vector<uint8_t>& getATR();

    /**
     *
     */
    std::vector<uint8_t> transmitApdu(const std::vector<uint8_t>& apduIn);

    /**
     *
     */
    void beginExclusive();

    /**
     *
     */
    void endExclusive();

    /**
     *
     */
    friend std::ostream& operator<<(std::ostream& os, const IOSTerminal& t);

    /**
     *
     */
    friend std::ostream& operator<<(std::ostream& os, const std::vector<IOSTerminal>& vt);

    /**
     *
     */
    bool operator==(const IOSTerminal& o) const;

    /**
     *
     */
    bool operator!=(const IOSTerminal& o) const;

private:
    /**
     *
     */
    const std::shared_ptr<Logger> mLogger = LoggerFactory::getLogger(typeid(IOSTerminal));

    /**
     *
     */
    const std::string mName;

    /**
     *
     */
    std::vector<uint8_t> mAtr;

    /**
     *
     */
    IOSTerminalImpl *mIOSTerminalImpl;
};

}
}
}
