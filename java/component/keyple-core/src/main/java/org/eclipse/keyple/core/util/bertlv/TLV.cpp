#include "TLV.h"
#include "Tag.h"
#include "ByteArrayUtil.h"

/* Common */
#include "Arrays.h"
#include "stringhelper.h"
#include "System.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace util {
namespace bertlv {

using ByteArrayUtil = org::eclipse::keyple::core::util::ByteArrayUtil;

TLV::TLV(std::vector<char> &binary) : binary(binary)
{
    tag = std::make_shared<Tag>(0, static_cast<char>(0), Tag::TagType::PRIMITIVE); // This is a primitive TLV
    length = 0;
}

bool TLV::parse(std::shared_ptr<Tag> tag, int offset)
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
        if ((binary[offset] & static_cast<char>(0x80)) == static_cast<char>(0x00)) {
            /* short form: single octet length */
            length += static_cast<int>(binary[offset]);
            position++;
        } else {
            /* long form: first octet (b6-b0)) gives the number of following length octets */
            int following = (binary[offset] & static_cast<char>(0x7F));
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

std::vector<char> TLV::getValue()
{
    static std::vector<char> value;

    value.clear();
    value = Arrays::copyOfRange(binary, position, position + length);
    position += length;

    return value;
}

int TLV::getPosition() {
    return position;
}

std::string TLV::toString() {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    return StringHelper::formatSimple("TAG: %s, LENGTH: %d, VALUE: %s", tag->toString(), length, ByteArrayUtil::toHex(getValue()));
}

}
}
}
}
}
}
