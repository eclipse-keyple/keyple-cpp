/********************************************************************************
* Copyright (c) 2019 Calypso Networks Association https://www.calypsonet-asso.org/
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

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace util {
namespace bertlv {

/**
    * This class represent a TAG as defined by the Basic Encoding Rules for ASN.1
    * <p>
    * (ITU-T X.690 / ISO 8825)
    */
class Tag : public std::enable_shared_from_this<Tag> {
public:
    enum class TagType {
        PRIMITIVE,
        CONSTRUCTED
    };

private:
    int tagNumber;
    char tagClass;
    TagType tagType;
    int size;

    /* the tag class */
public:
    static const char UNIVERSAL = static_cast<char>(0x00);
    static const char APPLICATION = static_cast<char>(0x01);
    static const char CONTEXT = static_cast<char>(0x02);
    static const char PRIVATE = static_cast<char>(0x03);

    /**
        * Creates a tag from its attributes.
        * <p>
        * 
        * @param tagNumber the tag value.
        * @param tagClass the tag class.
        * @param tagType constructed or primitive
        */
public:
    Tag(int tagNumber, char tagClass, TagType tagType);

    /**
        * Create a tag from a binary stream.
        * <p>
        * 
        * @param binary the byte array containing the TLV data
        * @param offset the start offset in the byte array
        * @throws IndexOutOfBoundsException if the offset is too large
        */
    Tag(std::vector<char> &binary, int offset) throw(std::out_of_range);

    virtual int getTagNumber();

    virtual char getTagClass();

    virtual TagType getTagType();

    virtual int getSize();

    virtual bool equals(std::shared_ptr<Tag> tag);

    std::string toString();
};

}
}
}
}
}
}
