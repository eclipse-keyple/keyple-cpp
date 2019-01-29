#pragma once

#include "CardException.h"

class CardTerminals {
public:

	/**
	 * constructor
	 *
	 * Constructs a new CardTerminals object.
	 */
	CardTerminals()
	{

	}

	/**
	 * Returns an unmodifiable list of all available terminals.
	 *
	 * @return an unmodifiable list of all available terminals.
	 *
	 * @throws CardException if the card operation failed
	 */
	std::vector<CardTerminal> list() throw(CardException)
	{
		return terminals;
	}

private:
	/**
	 *
	 */
	std::vector<CardTerminal> terminals;
};
