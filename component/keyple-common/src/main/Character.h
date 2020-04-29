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

namespace keyple {
namespace common {

class Character {
public:
    static char digit(char ch, int radix)
    {
        (void)radix;

        if (ch >= '0' && ch <= '9')
            return ch - '0';
        if (ch >= 'A' && ch <= 'Z')
            return ch - 'A' + 10;
        if (ch >= 'a' && ch <= 'z')
            return ch - 'a' + 10;

        return 0;
    }

    /**
     * See {@link #isWhitespace(int)}.
     */
    static bool isWhitespace(char c)
    {
        return isWhitespace((int)c);
    }

    /**
     * Returns true if the given code point is a Unicode whitespace character.
     * The exact set of characters considered as whitespace varies with Unicode version.
     * Note that non-breaking spaces are not considered whitespace.
     * Note also that line separators are considered whitespace; see {@link #isSpaceChar}
     * for an alternative.
     */
    static bool isWhitespace(int codePoint)
    {

        if ((codePoint >= 0x1c && codePoint <= 0x20) ||
            (codePoint >= 0x09 && codePoint <= 0x0d)) {
            return true;
        }
        if (codePoint < 0x1000) {
            return false;
        }

        /* OGHAM SPACE MARK or MONGOLIAN VOWEL SEPARATOR? */
        if (codePoint == 0x1680 || codePoint == 0x180e) {
            return true;
        }
        if (codePoint < 0x2000) {
            return false;
        }

        /* Exclude General Punctuation's non-breaking spaces (which includes FIGURE SPACE). */
        if (codePoint == 0x2007 || codePoint == 0x202f) {
            return false;
        }
        if (codePoint <= 0xffff) {
            /* Other whitespace from General Punctuation... */
            return codePoint <= 0x200a || codePoint == 0x2028 ||
                   codePoint == 0x2029 || codePoint == 0x205f ||
                   codePoint == 0x3000; /* ...or CJK Symbols and Punctuation? */
        }
        /* Let icu4c worry about non-BMP code points. */
        //return isWhitespaceImpl(codePoint);
        return false;
    }
};

}
}
