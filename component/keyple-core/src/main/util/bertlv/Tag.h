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

/* Common */
#include "Object.h"

/* Core */
#include "KeypleCoreExport.h"

namespace keyple {
namespace core {
namespace util {
namespace bertlv {

using namespace keyple::common;

/**
 * This class represent a TAG as defined by the Basic Encoding Rules for ASN.1
 * <br>
 * This implementation limits the tag size to 2.<br>
 * (ITU-T X.690 / ISO 8825)
 */
class KEYPLECORE_API Tag : public std::enable_shared_from_this<Tag> {
public:
    /**
     *
     */
    enum class TagType { PRIMITIVE, CONSTRUCTED };

    /**
     *
     */
    enum class TagClass : uint8_t {
        UNIVERSAL = 0,
        APPLICATION,
        CONTEXT,
        PRIVATE
    };

public:
    /**
     * Creates a tag from its attributes.
     * <p>
     *
     * @param tagNumber the tag value.
     * @param tagClass the tag class.
     * @param tagType constructed or primitive
     * @param tagSize the tag size (1 or 2)
     */
    Tag(int tagNumber, const TagClass tagClass, const TagType tagType,
        int tagSize);

    /**
     * Create a tag from a binary stream.
     * <p>
     *
     * @param binary the byte array containing the TLV data
     * @param offset the start offset in the byte array
     * @throws IndexOutOfBoundsException if the offset is too large
     */
    Tag(const std::vector<uint8_t>& binary, int offset);

    /**
     *
     */
    int getTagNumber() const;

    /**
     *
     */
    TagClass getTagClass() const;

    /**
     *
     */
    TagType getTagType() const;

    /**
     *
     */
    int getTagSize() const;

    /**
     *
     */
    bool operator==(const Tag& o) const;

    /**
     *
     */
    bool operator!=(const Tag& o) const;

    /**
     *
     */
    friend KEYPLECORE_API std::ostream& operator<<(std::ostream& os,
                                                   const Tag::TagClass& tc);
    /**
     *
     */
    friend KEYPLECORE_API std::ostream& operator<<(std::ostream& os,
                                                   const Tag::TagType& tt);

    /**
     *
     */
    friend KEYPLECORE_API std::ostream& operator<<(std::ostream& os,
                                                   const Tag& t);


private:
    /**
     *
     */
    int mTagNumber;

    /**
     *
     */
    TagClass mTagClass;

    /**
     *
     */
    TagType mTagType;

    /**
     *
     */
    int mTagSize;
};

}
}
}
}
