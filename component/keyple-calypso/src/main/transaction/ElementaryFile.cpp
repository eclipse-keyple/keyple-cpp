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

#include "ElementaryFile.h"

namespace keyple {
namespace calypso {
namespace transaction {

ElementaryFile::ElementaryFile(const uint8_t sfi)
: mSfi(sfi) {}

ElementaryFile::ElementaryFile(const uint8_t sfi, const FileData& data)
: mSfi(sfi), mData(data) {}

const uint8_t ElementaryFile::getSfi() const
{
    return mSfi;
}

const FileHeader& ElementaryFile::getHeader() const
{
    return mHeader;
}

ElementaryFile& ElementaryFile::setHeader(const FileHeader& header)
{
    mHeader = header;
    return *this;
}

const FileData& ElementaryFile::getData() const
{
    return mData;
}

bool ElementaryFile::operator==(const ElementaryFile& o) const
{
    return mSfi == o.mSfi;
}

friend KEYPLECALYPSO_API std::ostream& ElementaryFile::operator<<(
        std::ostream& os, const ElementaryFile& ef) const
{
    os << "ELEMENTARYFILE: {"
       << "SFI = " << ef.mSfi << ", "
       << ef.mHeader << ", "
       << ef.mData
       << "}"

    return os;
}

}
}
}