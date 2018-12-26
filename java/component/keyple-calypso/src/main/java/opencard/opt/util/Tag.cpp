#include "Tag.h"

namespace opencard {
    namespace opt {
        namespace util {

            Tag::Tag() {
                tag = 0;
                tagclass = 0;
                constructed = false;
            }

            Tag::Tag(std::shared_ptr<Tag> t) {
                this->tag = t->tag;
                this->tagclass = t->tagclass;
                this->constructed = t->constructed;
            }

            Tag::Tag(int tag, char tagClass, bool constructed) {
                this->tag = tag;
                this->tagclass = tagClass;
                this->constructed = constructed;
            }

            Tag::Tag(std::vector<char> &binary, std::vector<int> &offset) {
                fromBinary(binary, offset);
            }

            Tag::Tag(std::vector<char> &binary) {
                std::vector<int> offset(1);
                offset[0] = 0;
                fromBinary(binary, offset);
            }

            int Tag::size() {
                if (tag < 0x1F) {
                    return 1;
                }
                else if (tag < 0x80) {
                    return 2;
                }
                else if (tag < 0x4000) {
                    return 3;
                }
                else if (tag < 0x200000) {
                    return 4;
                }
                return 5;
            }

            void Tag::fromBinary(std::vector<char> &binary, std::vector<int> &offset) {
                // Get class of tag (encoded in bits 7,6 of first byte)
                tagclass = static_cast<char>(static_cast<int>(static_cast<unsigned int>((binary[offset[0]] & 0xC0)) >> 6));

                // Get constructed flag (encoded in bit 5 of first byte)
                if ((binary[offset[0]] & static_cast<char>(0x20)) == static_cast<char>(0x20)) {
                    constructed = true; // This is a constructed TLV
                }
                else {
                    constructed = false; // This is a primitive TLV
                }

                // Get tag number (encoded in bits 4-0 of first byte and optionally
                // several following bytes.
                tag = 0;
                if ((binary[offset[0]] & static_cast<char>(0x1F)) == static_cast<char>(0x1F))
                {
                // it's a multi byte tag
                    do {
                        offset[0]++;
                        tag *= 128;
                        tag += binary[offset[0]] & 0x7F;
                    } while ((binary[offset[0]] & 0x80) == 0x80);
                }
                else
                {
                // it's a one byte tag
                    tag = binary[offset[0]] & static_cast<char>(0x1F);
                }
                offset[0]++;
            }

            std::vector<char> Tag::getBytes() {
                std::vector<int> offset(1);
                offset[0] = 0;
                std::vector<char> result(size());
                toBinary(result, offset);
                return result;
            }

            void Tag::toBinary(std::vector<char> &binary, std::vector<int> &offset) {
                std::vector<char> classes = {static_cast<char>(0x00), static_cast<char>(0x40), static_cast<char>(0x80), static_cast<char>(0xC0)};
                int count;

                binary[offset[0]] |= classes[tagclass]; // encode class
                if (constructed) { // encode constructed bit
                    binary[offset[0]] |= 0x20;
                }

                if (tag < 31) { // encode tag number
                    binary[offset[0]] |= static_cast<char>(tag);
                }
                else {
                    binary[offset[0]] |= 0x1F;
                    for (count = this->size() - 2; count > 0; count--) {
                        offset[0]++;
                        binary[offset[0]] = static_cast<char>(0x80 | ((tag >> (count * 7)) & 0x7f));
                    }
                    offset[0]++;
                    binary[offset[0]] = static_cast<char>(tag & 0x7f);
                }
                offset[0]++;
            }

            void Tag::set(int tag, char tagclass, bool constructed) {
                this->tag = tag;
                this->tagclass = tagclass;
                this->constructed = constructed;
            }

            void Tag::setConstructed(bool constructed) {
                this->constructed = constructed;
            }

            int Tag::code() {
                return tag;
            }

            bool Tag::isConstructed() {
                return constructed;
            }

            int Tag::hashCode() {
                return tag + tagclass;
            }

            bool Tag::equals(std::shared_ptr<void> o) {
                return ((this->tag == (std::static_pointer_cast<Tag>(o))->tag) && (this->tagclass == (std::static_pointer_cast<Tag>(o))->tagclass));
            }

            std::string Tag::toString() {
                return "(" + std::to_string(tag) + "," + std::to_string(tagclass) + "," + StringHelper::toString(constructed) + ")";
            }
        }
    }
}
