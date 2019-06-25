#include "Tag.h"

#include "stringhelper.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace util {
namespace bertlv {

Tag::Tag(int tagNumber, char tagClass, TagType tagType) : tagNumber(tagNumber), tagClass(tagClass), tagType(tagType) {
    //if (tagType == nullptr) {
    //    throw std::invalid_argument("TLV Tag: type is null.");
    //}
    if (tagClass < 0 || tagClass > PRIVATE) {
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

Tag::Tag(std::vector<char> &binary, int offset)
: tagClass(static_cast<char>(static_cast<int>(static_cast<unsigned int>((binary[offset] & 0xC0)) >> 6)))
{
    /* the 2 first bits (b7b6) of the first byte defines the class */

    /* the type bit is the third bit (b5) */
    if ((binary[offset] & static_cast<char>(0x20)) == static_cast<char>(0x20)) {
        tagType = TagType::CONSTRUCTED;
    }
    else {
        tagType = TagType::PRIMITIVE;
    }

    /* the tag number is defined in the following bits (b4-b0) and possibly following octets */
    int index = offset;
    int number = 0;
    if ((binary[index] & static_cast<char>(0x1F)) == static_cast<char>(0x1F)) {
        /* all bits of tag number are set: multi-octet tag */
        do {
            index++;
            number <<= 7;
            number += binary[index] & 0x7F;
            /* loop while the "more bit" (b7) is set */
        } while ((binary[index] & 0x80) == 0x80);
    }
    else {
        /* single octet tag */
        number = binary[index] & static_cast<char>(0x1F);
    }

    tagNumber = number;
    size = index + 1 - offset;
}

int Tag::getTagNumber() {
    return tagNumber;
}

char Tag::getTagClass() {
    return tagClass;
}

Tag::TagType Tag::getTagType() {
    return tagType;
}

int Tag::getSize() {
    return size;
}

bool Tag::equals(std::shared_ptr<Tag> tag) {
    return ((this->tagNumber == tag->tagNumber) && (this->tagClass == tag->tagClass) && (this->tagType == tag->tagType));
}

std::string Tag::toString() {
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
    return StringHelper::formatSimple("TAG: size=%d Class=%s, Type=%s, Number=%X", size, tagClassString, "<tag type>", tagNumber);
}

}
}
}
}
}
}
