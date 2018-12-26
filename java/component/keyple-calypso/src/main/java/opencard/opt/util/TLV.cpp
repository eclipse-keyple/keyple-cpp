#include "TLV.h"
#include "Tag.h"
#include "../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"

namespace opencard {
    namespace opt {
        namespace util {
            using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;

            TLV::TLV() {
                tag_Renamed = std::make_shared<Tag>(0, static_cast<char>(0), false); // This is a primitive TLV
                length_Renamed = 0;
                value.clear();
                parent.reset(); // The new TLV has no parent,
                sibling.reset(); // no sibling,
                child.reset(); // no child
                lastChild.reset(); // and no last child.
            }

            TLV::TLV(std::vector<char> &binary) {
                std::vector<int> offset = {0};
                tag_Renamed = std::make_shared<Tag>(0, static_cast<char>(0), false); // This is a primitive TLV
                length_Renamed = 0;
                value.clear();
                parent.reset(); // The new TLV has no parent,
                sibling.reset(); // no sibling,
                child.reset(); // no child
                lastChild.reset(); // and no last child.
//JAVA TO C++ CONVERTER TODO TASK: You cannot use 'shared_from_this' in a constructor:
                fromBinary(binary, offset, shared_from_this(), nullptr);
            }

            TLV::TLV(std::vector<char> &binary, std::vector<int> &offset) {
                tag_Renamed = std::make_shared<Tag>(0, static_cast<char>(0), false); // This is a primitive TLV
                length_Renamed = 0;
                value.clear();
                parent.reset(); // The new TLV has no parent,
                sibling.reset(); // no sibling,
                child.reset(); // no child
                lastChild.reset(); // and no last child.
//JAVA TO C++ CONVERTER TODO TASK: You cannot use 'shared_from_this' in a constructor:
                fromBinary(binary, offset, shared_from_this(), nullptr);
            }

            TLV::TLV(std::shared_ptr<Tag> tag, std::vector<char> &value) {
                std::vector<int> offset = {0};
                std::shared_ptr<TLV> newTLV = std::make_shared<TLV>();

                this->tag_Renamed = std::make_shared<Tag>(tag);
                if (this->tag_Renamed->isConstructed()) {
                    while (offset[0] < value.size()) {
//JAVA TO C++ CONVERTER TODO TASK: You cannot use 'shared_from_this' in a constructor:
                        fromBinary(value, offset, newTLV, shared_from_this());
                    }
                }
                else {
                    if (value.size() > 0) {
                        this->length_Renamed = value.size();
                    }
                    else {
                        this->length_Renamed = 0;
                    }
                    this->value = value;
                    this->child.reset(); // no child and
                    this->lastChild.reset(); // no last child.
                }
                this->parent.reset(); // The new TLV has no parent,
                this->sibling.reset(); // no sibling,
            }

            TLV::TLV(std::shared_ptr<Tag> tag, int number) {
                int i;

                this->tag_Renamed = std::make_shared<Tag>(tag);

                // Find out how many bytes we need.
                if (number < 0x100) {
                    value = std::vector<char>(1);
                }
                else if (number < 0x10000) {
                    value = std::vector<char>(2);
                }
                else if (number < 0x1000000) {
                    value = std::vector<char>(3);
                }
                else {
                    value = std::vector<char>(4);
                }

                // Do conversion
                for (i = value.size() - 1; i >= 0; i--) {
                    value[i] = static_cast<char>(number % 0x100);
                    number /= 0x100;
                }

                this->length_Renamed = value.size();
                this->child.reset(); // no child and
                this->lastChild.reset(); // no last child.
                this->parent.reset(); // The new TLV has no parent,
                this->sibling.reset(); // no sibling,
            }

            TLV::TLV(std::shared_ptr<Tag> tag, std::shared_ptr<TLV> tlv) {
                this->tag_Renamed = std::make_shared<Tag>(tag);
                this->tag_Renamed->setConstructed(true); // This is a constructed TLV
                this->value.clear(); // therefore it has no direct value
                this->parent.reset(); // The new TLV has no parent.
                this->sibling.reset(); // The new TLV has no sibling.
                this->child = tlv; // TLV becomes child
                this->lastChild = tlv; // and last child (even if it's null).

                if (tlv != nullptr) {
                    this->length_Renamed = tlv->tag_Renamed->size() + tlv->lenBytes() + tlv->length_Renamed;
                }
                else {
                    this->length_Renamed = 0; // empty TLV has length 0.
                }
            }

            std::shared_ptr<TLV> TLV::add(std::shared_ptr<TLV> tlv) {
                std::shared_ptr<TLV> iterTLV;
                int originalReprLength;
                int deltaReprLength = 0;

                if (tag_Renamed->isConstructed()) {
                    tlv->parent = shared_from_this(); // make this the parent of added tlv
                    tlv->sibling.reset(); // last child has no sibling
                    if (lastChild != nullptr) { // if there already has been a child,
                        lastChild->sibling = tlv; // it gets tlv as a new sibling
                    }
                    lastChild = tlv; // tlv becomes last child

                    // update length of this TLV and all it's ancestors
                    iterTLV = shared_from_this();
                    while (iterTLV != nullptr) {
                        originalReprLength = iterTLV->lenBytes();
                        iterTLV->length_Renamed += tlv->length_Renamed + tlv->tag_Renamed->size() + tlv->lenBytes() + deltaReprLength;
                        deltaReprLength += iterTLV->lenBytes() - originalReprLength;
                        iterTLV = iterTLV->parent;
                    }
                    return shared_from_this();
                }
                else {
                    return nullptr;
                }
            }

            std::shared_ptr<TLV> TLV::findTag(std::shared_ptr<Tag> tag, std::shared_ptr<TLV> cursor) {
                std::shared_ptr<TLV> iterTLV;

                if (cursor == nullptr) {
                    iterTLV = child; // start with the first child
                }
                else {
                    iterTLV = cursor->sibling; // start with cursor's successor
                }

                if (tag == nullptr) {
                    return iterTLV; // null is wildcard
                }

                while (iterTLV != nullptr) {
                    if (iterTLV->tag_Renamed->equals(tag)) {
                        return iterTLV;
                    }
                    iterTLV = iterTLV->sibling;
                }
                return nullptr;
            }

            void TLV::fromBinary(std::vector<char> &binary, std::vector<int> &offset, std::shared_ptr<TLV> tlv, std::shared_ptr<TLV> parent) {
                int i = 0;
                int oldOffset = offset[0];
                std::shared_ptr<TLV> iterTLV;

                // Get the Tag from binary representation.
                tlv->tag_Renamed->fromBinary(binary, offset);
                // Get the length from binary representation.
                tlv->length_Renamed = 0;

                if ((binary[offset[0]] & static_cast<char>(0x80)) == static_cast<char>(0x00)) {
                    tlv->length_Renamed += static_cast<int>(binary[offset[0]]);
                }
                else {
                    int numBytes = (binary[offset[0]] & static_cast<char>(0x7F));
                    int j;
                    while (numBytes > 0) {
                        offset[0]++;
                        j = binary[offset[0]];
                        tlv->length_Renamed += (j < 0 ? j += 256 : j);

                        if (numBytes > 1) {
                            tlv->length_Renamed *= 256;
                        }
                        numBytes--;
                    }
                }
                offset[0]++;

                if (tlv->tag_Renamed->isConstructed()) {
                    tlv->value.clear();
                    if (tlv->length_Renamed > 0) {
                        tlv->child = std::make_shared<TLV>();
                        fromBinary(binary, offset, tlv->child, tlv);

                        iterTLV = tlv->child;
                        while (offset[0] <= oldOffset + tlv->length_Renamed) {
                            iterTLV->sibling = std::make_shared<TLV>();
                            fromBinary(binary, offset, iterTLV->sibling, tlv);
                            iterTLV = iterTLV->sibling;
                        }
                        tlv->lastChild = iterTLV;
                    }
                    else {
                        tlv->child.reset();
                    }
                }
                else {
                    tlv->child.reset();
                    tlv->sibling.reset(); // The new TLV has no sibling.
                    tlv->value = std::vector<char>(tlv->length_Renamed);
                    System::arraycopy(binary, offset[0], tlv->value, 0, tlv->length_Renamed);
                    offset[0] += tlv->length_Renamed;
                }
                tlv->parent = parent;
            }

            int TLV::lenBytes() {
                return lenBytes(length_Renamed);
            }

            int TLV::lenBytes(int length) {
                if (length < 0x80) {
                    return 1;
                }
                else if (length < 0x100) {
                    return 2;
                }
                else if (length < 0x10000) {
                    return 3;
                }
                else if (length < 0x1000000) {
                    return 4;
                }
                else {
                    return 5;
                }
            }

            int TLV::length() {
                return length_Renamed;
            }

            std::vector<char> TLV::lengthToBinary(int length) {
                std::vector<char> binary(lenBytes(length));
                if (length < 0x80) {
                    binary[0] = static_cast<char>(length);
                }
                else if (length < 0x100) {
                    binary[0] = static_cast<char>(0x81);
                    binary[1] = static_cast<char>(length);
                }
                else if (length < 0x10000) {
                    binary[0] = static_cast<char>(0x82);
                    binary[1] = static_cast<char>(length / 0x100);
                    binary[2] = static_cast<char>(length % 0x100);
                }
                else if (length < 0x1000000) {
                    binary[0] = static_cast<char>(0x83);
                    binary[1] = static_cast<char>(length / 0x10000);
                    binary[2] = static_cast<char>(length / 0x100);
                    binary[3] = static_cast<char>(length % 0x100);
                }
                return binary;
            }

            void TLV::setValue(std::vector<char> &newValue) {
                int originalReprLength;
                int deltaReprLength;

                originalReprLength = this->lenBytes();

                int oldLength = length_Renamed;
                value = newValue;
                if (newValue.size() > 0) {
                    length_Renamed = value.size();
                }
                else {
                    length_Renamed = 0;
                }

                deltaReprLength = this->lenBytes() - originalReprLength;

                // update length of this TLV and all it's ancestors
                std::shared_ptr<TLV> iterTLV = this->parent;
                while (iterTLV != nullptr) {
                    originalReprLength = iterTLV->lenBytes();
                    iterTLV->length_Renamed += (length_Renamed - oldLength) + deltaReprLength;
                    deltaReprLength += iterTLV->lenBytes() - originalReprLength;
                    iterTLV = iterTLV->parent;
                }
            }

            std::shared_ptr<Tag> TLV::tag() {
                return tag_Renamed;
            }

            std::vector<char> TLV::toBinary() {
                std::vector<int> offset = {0};
                int totalLength = tag_Renamed->size() + lenBytes() + length_Renamed;
                std::vector<char> binary(totalLength);
                this->toBinaryHelper(binary, offset, totalLength);
                return binary;
            }

            std::vector<char> TLV::toBinaryContent() {
                std::vector<int> offset = {0};
                int totalLength = length_Renamed;
                std::vector<char> binary(totalLength);
                this->toBinaryHelperContent(binary, offset, totalLength);
                return binary;
            }

            void TLV::toBinaryHelper(std::vector<char> &binary, std::vector<int> &offset, int max) {
                int i = 0;

                tag_Renamed->toBinary(binary, offset);
                toBinaryLength(binary, offset);
                if (child != nullptr) {
                    child->toBinaryHelper(binary, offset, max);
                }
                else if (value.size() > 0) {
                    System::arraycopy(value, 0, binary, offset[0], value.size());
                    offset[0] += value.size();
                }

                // We must check if offset is less than max, because when the TLV that
                // is converted
                // to binary has siblings, we would run into trouble (array out of
                // bounds)
                if (sibling != nullptr && offset[0] < max) {
                    sibling->toBinaryHelper(binary, offset, max);
                }
            }

            void TLV::toBinaryHelperContent(std::vector<char> &binary, std::vector<int> &offset, int max) {
                int i = 0;

                if (child != nullptr) {
                    child->toBinaryHelper(binary, offset, max);
                }
                else {
                    if (value.size() > 0) {
                        System::arraycopy(value, 0, binary, offset[0], value.size());
                        offset[0] += value.size();
                    }
                }
            }

            void TLV::toBinaryLength(std::vector<char> &binary, std::vector<int> &offset) {
                if (length_Renamed < 0x80) {
                    binary[offset[0]] = static_cast<char>(length_Renamed);
                }
                else if (length_Renamed < 0x100) {
                    binary[offset[0]] = static_cast<char>(0x81);
                    offset[0]++;
                    binary[offset[0]] = static_cast<char>(length_Renamed);
                }
                else if (length_Renamed < 0x10000) {
                    binary[offset[0]] = static_cast<char>(0x82);
                    offset[0]++;
                    binary[offset[0]] = static_cast<char>(length_Renamed / 0x100);
                    offset[0]++;
                    binary[offset[0]] = static_cast<char>(length_Renamed % 0x100);
                }
                else if (length_Renamed < 0x1000000) {
                    binary[offset[0]] = static_cast<char>(0x83);
                    offset[0]++;
                    binary[offset[0]] = static_cast<char>(length_Renamed / 0x10000);
                    offset[0]++;
                    binary[offset[0]] = static_cast<char>(length_Renamed / 0x100);
                    offset[0]++;
                    binary[offset[0]] = static_cast<char>(length_Renamed % 0x100);
                }
                offset[0]++;
            }

            std::string TLV::toString() {
                return "Tag: " + tag_Renamed + ", Length: " + std::to_string(length_Renamed) + ", Value: " + ByteArrayUtils::toHex(value);
            }

            std::string TLV::toString(std::unordered_map ht, int level) {
                std::shared_ptr<StringBuilder> s = std::make_shared<StringBuilder>();
                int i;
                for (i = 0; i < level; i++) {
                    s->append(" ");
                }

                if (ht.empty()) {
                    s->append("[")->append(tag_Renamed)->append(" ")->append(length_Renamed)->append("] ");
                }
                else {
                    s->append(ht[tag_Renamed])->append(" ");
                }

                if (tag_Renamed->isConstructed()) {
                    s->append("\n");
                    for (i = 0; i < level; i++) {
                        s->append(" ");
                    }
                }
                s->append("( ");

                if (tag_Renamed->isConstructed()) {
                    s->append("\n");
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                    s->append(child->toString(ht, level + 2));
                    for (i = 0; i < level; i++) {
                        s->append(" ");
                    }
                    s->append(")\n");
                }
                else {
                    bool fPrintable = true;
                    if (value.size() > 0) {
                        for (i = 0; i < value.size(); i++) {
                            if (value[i] < 32) {
                                fPrintable = false;
                            }
                        }
                        if (fPrintable) {
                            s->append("\"")->append(std::string(value))->append("\"");
                        }
                        else {
                            s->append("'");
                            s->append(ByteArrayUtils::toHex(value));
                            s->append("'");
                        }
                    }
                    s->append(" )\n");
                }

                if (sibling != nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                    s->append(sibling->toString(ht, level));
                }
                return s->toString();
            }

            std::vector<char> TLV::valueAsByteArray() {
                return value;
            }

            int TLV::valueAsNumber() {
                int i;
                int j;
                int number = 0;

                for (i = 0; i < value.size(); i++) {
                    j = value[i];
                    number = number * 256 + (j < 0 ? j += 256 : j);
                }
                return number;
            }
        }
    }
}
