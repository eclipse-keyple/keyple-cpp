/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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

#include "TLV.h"
#include "Tag.h"
#include "ByteArrayUtil.h"

/* Common */
#include "Arrays.h"
#include "Logger.h" /* operator<< on std::vector<uint8_t> */
#include "stringhelper.h"
#include "System.h"

namespace keyple {
namespace core {
namespace util {
namespace bertlv {

using namespace keyple::core::util;

TLV::TLV(const std::vector<uint8_t>& binary)
: mLength(0), mBinary(binary), mPosition(0)
{
    mTag = std::make_shared<Tag>(0, static_cast<Tag::TagClass>(0),
                                 Tag::TagType::PRIMITIVE, 1);
}

bool TLV::parse(const std::shared_ptr<Tag> tag, int offset)
{
    if (tag == nullptr) {
        throw std::invalid_argument("TLV parsing: tag can't be null.");
    }

    try {
        mTag = std::make_shared<Tag>(mBinary, offset);
    } catch (const std::out_of_range& e) {
        (void)e;
        throw std::invalid_argument("TLV parsing: index is too large.");
    }

    mLength = 0;
    if (*tag.get() == *(mTag.get())) {
        offset += mTag->getTagSize();
        mPosition += mTag->getTagSize();
        if ((mBinary[offset] & 0x80) == 0x00) {
            /* short form: single octet length */
            mLength += static_cast<int>(mBinary[offset]);
            mPosition++;
        } else {
            /*
             * Long form: first octet (b6-b0)) gives the number of following
             * length octets
             */
            int following = (mBinary[offset] & 0x7F);
            mPosition++;
            while (following > 0) {
                offset++;
                mPosition++;
                mLength += (mBinary[offset] & 0xFF);
                if (following > 1) {
                    mLength <<= 8;
                }
                following--;
            }
        }
        return true;
    } else {
        return false;
    }
}

const std::vector<uint8_t> TLV::getValue()
{
    std::vector<uint8_t> value;

    value = Arrays::copyOfRange(mBinary, mPosition, mPosition + mLength);
    mPosition += mLength;

    return value;
}

int TLV::getPosition() const
{
    return mPosition;
}

std::ostream& operator<<(std::ostream& os, const TLV& tlv)
{
    os << "TLV: {"
       << tlv.mTag << ", "
       << "LENGTH = " << tlv.mLength << ", "
       << "VALUE = " << tlv.mBinary
       << "}";

    return os;
}


}
}
}
}
