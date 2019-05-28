/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#include <algorithm>
#include <string>

/* Smartcard I/O */
#include "ATR.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace smartcardio {

ATR::ATR(std::vector<char> atr) : atr(atr)
{
    parse();
}


std::vector<char> ATR::getBytes()
{
    std::vector<char> copy(atr.begin(), atr.end());

    return copy;
}

std::vector<char> ATR::getHistoricalBytes()
{
    std::vector<char> copy;
    std::copy(atr.begin() + startHistorical,
              atr.begin() + startHistorical + nHistorical,
              std::back_inserter(copy));

    return copy;
}

std::string ATR::toString()
{
    return StringHelper::formatSimple("ATR: %d bytes", atr.size());
}

bool ATR::equals(Object& obj)
{
    if (this == &obj)
        return true;

    ATR& other = dynamic_cast<ATR&>(obj);
    return Arrays::equals(this->atr, other.atr);
}

int ATR::hashCode()
{
    return Arrays::hashCode(atr);
}

bool ATR::equals(std::shared_ptr<void> o)
{
    if (this == o.get())
        return true;

    return false;
}

void ATR::finalize()
{

}

void ATR::parse()
{
    if (atr.size() < 2) {
        return;
    }

    if ((atr[0] != 0x3b) && (atr[0] != 0x3f)) {
        return;
    }

    int t0 = (atr[1] & 0xf0) >> 4;
    int n = atr[1] & 0xf;
    int i = 2;

    while ((t0 != 0) && (i < (int)atr.size())) {
        if ((t0 & 1) != 0) {
            i++;
        }
        if ((t0 & 2) != 0) {
            i++;
        }
        if ((t0 & 4) != 0) {
            i++;
        }
        if ((t0 & 8) != 0) {
            if (i >= (int)atr.size()) {
                return;
            }
            t0 = (atr[i++] & 0xf0) >> 4;
        } else {
            t0 = 0;
        }
    }

    int k = i + n;
    if ((k == (int)atr.size()) || (k == (int)atr.size() - 1)) {
        startHistorical = i;
        nHistorical = n;
    }
}

}
}
}
}
