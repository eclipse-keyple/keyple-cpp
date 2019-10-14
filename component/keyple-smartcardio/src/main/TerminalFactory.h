#pragma once

/* Common */
#include "Export.h"

/* Smartcard I/O */
#include "CardTerminal.h"
#include "CardTerminals.h"

using CardTerminals = org::eclipse::keyple::smartcardio::CardTerminals;

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
