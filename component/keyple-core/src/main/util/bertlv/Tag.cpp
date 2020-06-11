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

#include <iostream>

#include "Tag.h"

#include "stringhelper.h"

namespace keyple {
namespace core {
namespace util {
namespace bertlv {

using namespace keyple::common;

Tag::Tag(int tagNumber, TagClass tagClass, TagType tagType)
: tagNumber(tagNumber), tagClass(tagClass), tagType(tagType)
{
 
    if (tagClass > Tag::TagClass::PRIVATE) {
        throw std::invalid_argument("TLV Tag: unknown tag class.");
    }

    if (tagNumber < 0x1F) {
        size = 1;
    } else if (tagNumber < 0x80) {
        size = 2;
    } else if (tagNumber < 0x4000) {
        size = 3;
    } else if (tagNumber < 0x200000) {
        size = 4;
    } else {
        size = 5;
    }
}

Tag::Tag(const std::vector<uint8_t>& binary, int offset)
{
    if (offset >= binary.size())
        throw std::invalid_argument("offset out of bound");

    /* the 2 first bits (b7b6) of the first byte defines the class */
    tagClass = static_cast<TagClass>((binary[offset] & 0xC0) >> 6);

    /* the type bit is the third bit (b5) */
    if ((binary[offset] & 0x20) == 0x20) {
        tagType = TagType::CONSTRUCTED;
    } else {
        tagType = TagType::PRIMITIVE;
    }

    /*
     * The tag number is defined in the following bits (b4-b0) and possibly
     * following octets.
     */
    int index  = offset;
    int number = 0;
    if ((binary[index] & 0x1F) == 0x1F) {
        /* all bits of tag number are set: multi-octet tag */
        do {
            index++;
            number <<= 7;
            number += binary[index] & 0x7F;
            /* loop while the "more bit" (b7) is set */
        } while ((binary[index] & 0x80) == 0x80);
    } else {
        /* single octet tag */
        number = binary[index] & 0x1F;
    }

    tagNumber = number;
    size      = index + 1 - offset;
}

int Tag::getTagNumber() const
{
    return tagNumber;
}

Tag::TagClass Tag::getTagClass() const
{
    return tagClass;
}

Tag::TagType Tag::getTagType() const
{
    return tagType;
}

int Tag::getSize() const
{
    return size;
}


bool Tag::operator==(const Tag& o) const
{
    return this->tagNumber == o.tagNumber &&
           this->tagClass == o.tagClass &&
           this->tagType == o.tagType &&
           this->size == o.size;
}

bool Tag::operator!=(const Tag& o) const
{
    return !(*this == o);
}

std::ostream& operator<<(std::ostream& os, const Tag::TagClass& tc)
{
    os << "TAGCLASS = ";

    switch (tc) {
    case Tag::TagClass::UNIVERSAL:
        os << "UNIVERSAL";
        break;
    case Tag::TagClass::APPLICATION:
        os << "APPLICATION";
        break;
    case Tag::TagClass::CONTEXT:
        os << "CONTEXT";
        break;
    case Tag::TagClass::PRIVATE:
        os << "PRIVATE";
        break;
    default:
        os << "UNKWOWN";
        break;
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const Tag::TagType& tt)
{
    os << "TAGTYPE = ";

    switch (tt) {
    case Tag::TagType::CONSTRUCTED:
        os << "CONSTRUCTED";
        break;
    case Tag::TagType::PRIMITIVE:
        os << "PRIMITIVE";
        break;
    default:
        os << "UNKNOWN";
        break;
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const Tag& t)
{
    os << "TAG: {"
       << "SIZE = " << t.size << ", "
       << t.tagClass << ", "
       << t.tagType << ", "
       << "NUMBER = " << t.tagNumber
       << "}";

    return os;
}

}
}
}
}
