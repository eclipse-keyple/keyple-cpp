/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#pragma once

#include <string.h>
#include <vector>

#if defined(WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include <winscard.h>
#else
#include <PCSC/winscard.h>
#include <PCSC/wintypes.h>
#endif

/* Smartcard I/O */
#include "CardException.h"
#include "CardTerminal.h"

/* Common */
#include "Export.h"
#include "Logger.h"
#include "LoggerFactory.h"

namespace keyple {
namespace smartcardio {

using namespace keyple::common;

class EXPORT CardTerminals {
public:
    /**
	 * Returns an unmodifiable list of all available terminals.
	 *
	 * @return an unmodifiable list of all available terminals.
	 *
	 * @throws CardException if the card operation failed
	 */
    std::vector<CardTerminal> list();

    /**
	 * Constructor
	 *
	 * Constructs a new CardTerminals object.
	 *
	 * <p>This constructor is called by subclasses only. Application should
     * call {@linkplain TerminalFactory#terminals} to obtain a CardTerminals
	 * object.
     */
    CardTerminals();

    /**
     * Destructor
     */
    ~CardTerminals();

protected:
    /**
	 *
	 */
    SCARDCONTEXT ctx;

private:
    /**
	 *
	 */
    std::vector<CardTerminal> terminals;

    /**
	 *
	 */
    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(CardTerminals));

};

}
}
