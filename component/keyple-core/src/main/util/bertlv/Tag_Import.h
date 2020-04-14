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
#include "Export.h"
#include "Object.h"

namespace keyple {
namespace core {
namespace util {
namespace bertlv {

using namespace keyple::common;

/**
 * This class represent a TAG as defined by the Basic Encoding Rules for ASN.1
 * <p>
 * (ITU-T X.690 / ISO 8825)
 */
class IMPORT Tag : public std::enable_shared_from_this<Tag> {
public:
    /**
     *
     */
    enum class TagType { PRIMITIVE, CONSTRUCTED };

    /**
     * The tag class
     */
    static const char UNIVERSAL;
    static const char APPLICATION;
    static const char CONTEXT;
    static const char PRIVATE;

public:
    /**
     * Creates a tag from its attributes.
     * <p>
     *
     * @param tagNumber the tag value.
     * @param tagClass the tag class.
     * @param tagType constructed or primitive
     */
    Tag(int tagNumber, uint8_t tagClass, TagType tagType);

    /**
     * Create a tag from a binary stream.
     * <p>
     *
     * @param binary the byte array containing the TLV data
     * @param offset the start offset in the byte array
     * @throws IndexOutOfBoundsException if the offset is too large
     */
    Tag(std::vector<uint8_t>& binary, int offset);

    /**
     *
     */
    int getTagNumber();

    /**
     *
     */
    char getTagClass();

    /**
     *
     */
    TagType getTagType();

    /**
     *
     */
    int getSize();
		
	/**
     *
	 */
	bool equals(std::shared_ptr<Tag> tag);

    /**
     *
     */
    std::string toString();

private:
    /**
     *
     */
    int tagNumber;

    /**
     *
     */
    uint8_t tagClass;

    /**
     *
     */
    TagType tagType;

    /**
     *
     */
    int size;
};

}
}
}
}
