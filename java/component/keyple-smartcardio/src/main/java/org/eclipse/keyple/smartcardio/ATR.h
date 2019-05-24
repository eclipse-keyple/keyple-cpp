/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

 #pragma once

#include <algorithm>
#include <string>

/* Common */
#include "Arrays.h"
#include "Logger.h"
#include "LoggerFactory.h"
#include "stringhelper.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace smartcardio {

class ATR : public Object {
public:
    /**
     * Constructs an ATR from a byte array.
     *
     * @param atr the byte array containing the answer-to-reset bytes
     * @throws NullPointerException if <code>atr</code> is null
     */
    ATR(std::vector<char> atr) : atr(atr)
    {
        parse();
    }

    /**
     * Returns a copy of the bytes in this ATR.
     *
     * @return a copy of the bytes in this ATR.
     */
    std::vector<char> getBytes()
    {
        std::vector<char> copy(atr.begin(), atr.end());

        return copy;
    }

    /**
     * Returns a copy of the historical bytes in this ATR.
     * If this ATR does not contain historical bytes, an array of length
     * zero is returned.
     *
     * @return a copy of the historical bytes in this ATR.
     */
    std::vector<char> getHistoricalBytes()
    {
        std::vector<char> copy;
        std::copy(atr.begin() + startHistorical,
                  atr.begin() + startHistorical + nHistorical,
                  std::back_inserter(copy));

        return copy;
    }

    /**
     * Returns a string representation of this ATR.
     *
     * @return a String representation of this ATR.
     */
    std::string toString()
    {
        return StringHelper::formatSimple("ATR: %d bytes", atr.size());
    }

    /**
     * Compares the specified object with this ATR for equality.
     * Returns true if the given object is also an ATR and its bytes are
     * identical to the bytes in this ATR.
     *
     * @param obj the object to be compared for equality with this ATR
     * @return true if the specified object is equal to this ATR
     */
    bool equals(Object& obj)
    {
        if (this == &obj)
            return true;

        ATR& other = dynamic_cast<ATR&>(obj);
        return Arrays::equals(this->atr, other.atr);
    }

    /**
     * Returns the hash code value for this ATR.
     *
     * @return the hash code value for this ATR.
     */
    int hashCode()
    {
        return Arrays::hashCode(atr);
    }

    bool equals(std::shared_ptr<void> o)
    {
        if (this == o.get())
            return true;

        return false;
    }

protected:
    void finalize()
    {

    }

private:
    std::vector<char> atr;

    int startHistorical, nHistorical;

    void parse()
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

        while ((t0 != 0) && (i < atr.size())) {
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
                if (i >= atr.size()) {
                    return;
                }
                t0 = (atr[i++] & 0xf0) >> 4;
            } else {
                t0 = 0;
            }
        }

        int k = i + n;
        if ((k == atr.size()) || (k == atr.size() - 1)) {
            startHistorical = i;
            nHistorical = n;
        }
    }
};

}
}
}
}
