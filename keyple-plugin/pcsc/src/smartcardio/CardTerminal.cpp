    /*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#include <string>

#include "CardTerminal.hpp"

CardTerminal::CardTerminal()
{

}

CardTerminal::~CardTerminal()
{

}

Card& CardTerminal::connect(std::string protocol)
{
    LONG status = SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &context);
    if( status != SCARD_S_SUCCESS )
        throw CardException("could not establish context");
    
    return card;
}

std::string &CardTerminal::getName()
{

}

bool CardTerminal::isCardPresent()
{

}

bool CardTerminal::waitForCardAbsent(long timeout)
{

}

bool CardTerminal::waitForCardPresent(long timeout)
{

}
