/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

#include "TLV.h"
#include "Tag.h"
#include "ByteArrayUtil.h"

/* Common */
#include "Arrays.h"
#include "stringhelper.h"
#include "System.h"

namespace keyple {
namespace core {
namespace util {
namespace bertlv {

using namespace keyple::core::util;

TLV::TLV(std::vector<uint8_t> &binary) : binary(binary)
{
    tag = std::make_shared<Tag>(0, 0, Tag::TagType::PRIMITIVE);
    length = 0;
}

bool TLV::parse(const std::shared_ptr<Tag> tag, int offset)
{
    if (tag == nullptr) {
        throw std::invalid_argument("TLV parsing: tag can't be null.");
    }

    try {
        this->tag = std::make_shared<Tag>(binary, offset);
    } catch (const std::out_of_range &e) {
        (void)e;
        throw std::invalid_argument("TLV parsing: index is too large.");
    }

    length = 0;
    if (tag->equals(this->tag)) {
        offset += this->tag->getSize();
        position += this->tag->getSize();
        if ((binary[offset] & 0x80) == 0x00) {
            /* short form: single octet length */
            length += static_cast<int>(binary[offset]);
            position++;
        } else {
            /*
             * Long form: first octet (b6-b0)) gives the number of following
             * length octets
             */
            int following = (binary[offset] & 0x7F);
            position++;
            while (following > 0) {
                offset++;
                position++;
                length += (binary[offset] & 0xFF);
                if (following > 1) {
                    length <<= 8;
                }
                following--;
            }
        }
        return true;
    } else {
        return false;
    }
}

std::vector<uint8_t> TLV::getValue()
{
    std::vector<uint8_t> value;

    value = Arrays::copyOfRange(binary, position, position + length);
    position += length;

    return value;
}

int TLV::getPosition()
{
    return position;
}

std::string TLV::toString()
{
    return StringHelper::formatSimple("TAG: %s, LENGTH: %d, VALUE: %s",
                                      tag->toString(), length,
                                      ByteArrayUtil::toHex(getValue()));
}

}
}
}
}
