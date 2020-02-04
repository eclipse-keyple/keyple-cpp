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

const char Tag::UNIVERSAL = static_cast<char>(0x00);
const char Tag::APPLICATION = static_cast<char>(0x01);
const char Tag::CONTEXT = static_cast<char>(0x02);
const char Tag::PRIVATE = static_cast<char>(0x03);

Tag::Tag(int tagNumber, uint8_t tagClass, TagType tagType)
: tagNumber(tagNumber), tagClass(tagClass), tagType(tagType)
{
    //if (tagType == nullptr) {
    //    throw std::invalid_argument("TLV Tag: type is null.");
    //}
    if (tagClass > PRIVATE) {
        throw std::invalid_argument("TLV Tag: unknown tag class.");
    }
    if (tagNumber < 0x1F) {
        size = 1;
    }
    else if (tagNumber < 0x80) {
        size = 2;
    }
    else if (tagNumber < 0x4000) {
        size = 3;
    }
    else if (tagNumber < 0x200000) {
        size = 4;
    }
    else {
        size = 5;
    }
}

Tag::Tag(std::vector<uint8_t>& binary, int offset)
{
    /* the 2 first bits (b7b6) of the first byte defines the class */
    tagClass = (binary[offset] & 0xC0) >> 6;

    /* the type bit is the third bit (b5) */
    if ((binary[offset] & 0x20) == 0x20) {
        tagType = TagType::CONSTRUCTED;
    }
    else {
        tagType = TagType::PRIMITIVE;
    }

    /*
     * The tag number is defined in the following bits (b4-b0) and possibly
     * following octets.
     */
    int index = offset;
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
    size = index + 1 - offset;
}

int Tag::getTagNumber()
{
    return tagNumber;
}

char Tag::getTagClass()
{
    return tagClass;
}

Tag::TagType Tag::getTagType()
{
    return tagType;
}

int Tag::getSize()
{
    return size;
}

bool Tag::equals(std::shared_ptr<Object> obj)
{
    std::shared_ptr<Tag> tag = std::dynamic_pointer_cast<Tag>(obj);

    if (!tag)
        return false;

    if (tag.get() == this)
        return true;

    return ((this->tagNumber == tag->tagNumber) &&
            (this->tagClass == tag->tagClass)   &&
            (this->tagType == tag->tagType));
}

bool Tag::equals(std::shared_ptr<Tag> tag)
{
    if (!tag)
        return false;

    if (tag.get() == this)
        return true;

    return ((this->tagNumber == tag->tagNumber) &&
            (this->tagClass == tag->tagClass)   &&
            (this->tagType == tag->tagType));
}

std::string Tag::toString()
{
    std::string tagClassString;
    switch (tagClass) {
        case Tag::UNIVERSAL:
            tagClassString = "UNIVERSAL";
            break;
        case Tag::APPLICATION:
            tagClassString = "APPLICATION";
            break;
        case Tag::CONTEXT:
            tagClassString = "CONTEXT";
            break;
        case Tag::PRIVATE:
            tagClassString = "PRIVATE";
            break;
        default:
            tagClassString = "UNKWOWN";
            break;
    }

    return StringHelper::formatSimple("TAG: size=%d Class=%s, Type=%s, " \
                                     "Number=%X", size, tagClassString,
                                     "<tag type>", tagNumber);
}

}
}
}
}
