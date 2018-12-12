/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#include "SelectionStatus.hpp"

SelectionStatus::SelectionStatus(AnswerToReset *atr, ApduResponse *fci, bool hasMatched)
: atr(atr),
  fci(fci),
  hasMatched(hasMatched)
{
    if (atr == nullptr && fci == nullptr)
    {
        throw std::invalid_argument("Atr and Fci can't be null at the same time.");
    }
}