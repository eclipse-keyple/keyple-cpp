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

/* Common */
#include "Export.h"

/* Smartcard I/O */
#include "CardTerminal.h"
#include "CardTerminals.h"

using namespace keyple::smartcardio;

namespace keyple {
namespace smartcardio {

class TerminalFactory {
public:
	/**
	 * Returns the default TerminalFactory instance. See getDefaultType() for
	 * more information.
	 *
	 * A default TerminalFactory is always available. However, depending on the
	 * implementation, it may not offer any terminals.
	 *
	 * @return the default TerminalFactory
	 */
	static std::shared_ptr<TerminalFactory> getDefault()
	{
		static TerminalFactory factory;
		return std::make_shared<TerminalFactory>(factory);
	}

	/**
	 * Returns a new CardTerminals object encapsulating the terminals supported
	 * by this factory. See the class comment of the CardTerminals class
	 * regarding how the returned objects can be shared and reused.
	 *
	 * @return a new CardTerminals object encapsulating the terminals supported
	 *         by this factory.
	 */
	std::shared_ptr<CardTerminals> terminals()
	{
		return std::shared_ptr<CardTerminals>(new CardTerminals());
	}
};

}
}
