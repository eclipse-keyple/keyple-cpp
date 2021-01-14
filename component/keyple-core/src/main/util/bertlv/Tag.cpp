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

#include <iostream>

#include "Tag.h"

#include "stringhelper.h"

namespace keyple {
namespace core {
namespace util {
namespace bertlv {

using namespace keyple::common;

Tag::Tag(
  int tagNumber, const TagClass tagClass, const TagType tagType, int tagSize)
: mTagNumber(tagNumber), mTagClass(tagClass), mTagType(tagType),
  mTagSize(tagSize)
{
    if (tagClass > TagClass::PRIVATE)
        throw std::invalid_argument("TLV Tag: unknown tag class.");
}

Tag::Tag(const std::vector<uint8_t>& binary, int offset)
{
    if (offset >= static_cast<int>(binary.size()))
        throw std::invalid_argument("offset out of bound");

    /* the 2 first bits (b7b6) of the first byte defines the class */
    mTagClass = static_cast<TagClass>((binary[offset] & 0xC0) >> 6);

    /* the type bit is the third bit (b5) */
    if ((binary[offset] & 0x20) == 0x20) {
        mTagType = TagType::CONSTRUCTED;
    } else {
        mTagType = TagType::PRIMITIVE;
    }

    /* */
    int number = binary[offset] & 0x1F;
    if (number == 0x1F) {
        /* two-byte tag */
        number = binary[offset + 1];
        mTagSize = 2;
    } else {
        /* one-byte tag */
        mTagSize = 1;
    }

    mTagNumber = number;
}

int Tag::getTagNumber() const
{
    return mTagNumber;
}

Tag::TagClass Tag::getTagClass() const
{
    return mTagClass;
}

Tag::TagType Tag::getTagType() const
{
    return mTagType;
}

int Tag::getTagSize() const
{
    return mTagSize;
}


bool Tag::operator==(const Tag& o) const
{
    return this->mTagNumber == o.mTagNumber &&
           this->mTagClass == o.mTagClass &&
           this->mTagType == o.mTagType;
           /* Do not compare size for now - buggy */
           //this->size == o.size;
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
       << "SIZE = " << t.mTagSize << ", "
       << t.mTagClass << ", "
       << t.mTagType << ", "
       << "NUMBER = " << t.mTagNumber
       << "}";

    return os;
}

}
}
}
}
