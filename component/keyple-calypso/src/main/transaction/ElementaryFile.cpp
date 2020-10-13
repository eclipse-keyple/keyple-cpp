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


ElementaryFile::ElementaryFile(
  const uint8_t sfi, std::shared_ptr<FileData> data)
: mSfi(sfi), mData(data) {}

ElementaryFile::ElementaryFile(const uint8_t sfi)
: ElementaryFile(sfi, std::make_shared<FileData>()) {}


ElementaryFile::ElementaryFile(const ElementaryFile& o)
: ElementaryFile(o.getSfi(), std::make_shared<FileData>(*o.getData().get())) {}

uint8_t ElementaryFile::getSfi() const
{
    return mSfi;
}

const std::shared_ptr<FileHeader> ElementaryFile::getHeader() const
{
    return mHeader;
}

ElementaryFile& ElementaryFile::setHeader(
    const std::shared_ptr<FileHeader> header)
{
    mHeader = header;
    return *this;
}

std::shared_ptr<FileData> ElementaryFile::getData() const
{
    return mData;
}

bool ElementaryFile::operator==(const ElementaryFile& o) const
{
    return mSfi == o.mSfi;
}

std::ostream& operator<<(std::ostream& os, const ElementaryFile& ef)
{
    os << "ELEMENTARYFILE: {"
       << "SFI = " << ef.mSfi << ", "
       << ef.mHeader << ", "
       << ef.mData
       << "}";

    return os;
}

}
}
}