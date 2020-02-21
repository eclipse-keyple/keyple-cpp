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

#include "System.h"

namespace keyple {
namespace common {

class BerDecoder {
private:
    /*
     * The start point in buf to decode
     */
    int origOffset;

    std::vector<char> buf;

    int bufsize;

    int offset;

    /**
     * Resets this decode to start parsing from the initial offset
     * (ie., same state as after calling the constructor).
     */
    void reset() {
        offset = origOffset;
    }

public:
    /**
     * Creates a BER decoder that reads bytes from the specified buffer.
     */
    BerDecoder(std::vector<char> buf, int offset, int bufsize) : origOffset(offset), buf(buf), bufsize(bufsize)
    {
        reset();
    }

    /**
     * Parses the next byte in this BER buffer.
     * @return The byte parsed.
     */
    int parseByte()
    {
        if (bufsize - offset < 1) {
            throw std::runtime_error("Insufficient data");
        }
        return buf[offset++] & 0xff;
    }

    /**
     * Returns the next byte in this BER buffer without consuming it.
     * @return The next byte.
     */
    int peekByte()
    {
        if (bufsize - offset < 1) {
            throw std::runtime_error("Insufficient data");
        }
        return buf[offset] & 0xff;
    }
    
    /**
      * Parses a possibly variable length field.
      */
    int parseLength() 
    {

        int lengthbyte = parseByte();

        if ((lengthbyte & 0x80) == 0x80) {

            lengthbyte &= 0x7f;

            if (lengthbyte == 0) {
                throw std::runtime_error("Indefinite length not supported");
            }

            if (lengthbyte > 4) {
                throw std::runtime_error("encoding too long");
            }

            if (bufsize - offset < lengthbyte) {
                throw std::runtime_error("Insufficient data");
            }

            int retval = 0;

            for( int i = 0; i < lengthbyte; i++) {
                retval = (retval << 8) + (buf[offset++] & 0xff);
            }
            if (retval < 0) {
              throw std::runtime_error("Invalid length bytes");
            }
            return retval;
        } else {
            return lengthbyte;
        }
    }

    /**
     * Parses an octet string of a given type(tag) from this BER buffer.
     * <blockquote><pre>
     * BER Binary Data of type "tag" ::= tag length {byte}*
     *</pre></blockquote>
     *
     * @param tag The tag to look for.
     * @param rlen An array for returning the relative parsed position. If null,
     *          the relative parsed position is not returned.
     * @return A non-null array containing the octet string.
     * @throws DecodeException If the next byte in the BER buffer is not
     * <tt>tag</tt>, or if length specified in the BER buffer exceeds the
     * number of bytes left in the buffer.
     */
    std::vector<char> parseOctetString(int tag, std::vector<int> rlen)
    {

        int origOffset = offset;
        int st;

        if ((st = parseByte()) != tag) {

            throw std::runtime_error(StringHelper::formatSimple("Encountered ASN.1 tag %x (expected tag %x)", st, tag));
        }

        int len = parseLength();

        if (len > bufsize - offset) {
            throw std::runtime_error("Insufficient data");
        }

        std::vector<char> retarr;
        if (len > 0) {
            System::arraycopy(buf, offset, retarr, 0, len);
            offset += len;
        }

        if (!rlen.empty()) {
            rlen[0] = offset - origOffset;
        }
        
        return retarr;
    }
};

}
}

