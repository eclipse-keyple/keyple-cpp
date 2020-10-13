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

#pragma once

#include <cstdint>
#include <map>
#include <ostream>

#include "FileData.h"
#include "FileHeader.h"
#include "KeypleCalypsoExport.h"

namespace keyple {
namespace calypso {
namespace transaction {

/**
 * The class {@code ElementaryFile} contains the description of a Calypso EF.
 *
 * @since 0.9
 */
class ElementaryFile final {
public:
    /**
     * (package-private)<br>
     * Constructor
     *
     * @param sfi the associated SFI
     * @since 0.9
     */
    ElementaryFile(const uint8_t sfi);

    /**
     * Copy constructor
     *
     * C++ vs. Java:  Copy constructor is prefereable to .clone() method
     */
    ElementaryFile(const ElementaryFile& o);

    /**
     * Gets the associated SFI.
     *
     * @return the SFI
     * @since 0.9
     */
    uint8_t getSfi() const;

    /**
     * Gets the file header.
     *
     * @return a header reference or null if header is not yet set.
     * @since 0.9
     */
    const std::shared_ptr<FileHeader> getHeader() const;

    /**
     * (package-private)<br>
     * Sets the file header.
     *
     * @param header the file header (should be not null)
     * @return the current instance.
     */
    ElementaryFile& setHeader(const std::shared_ptr<FileHeader> header);
    /**
     * Gets the file data.
     *
     * @return a not null data reference.
     * @since 0.9
     */
    std::shared_ptr<FileData> getData() const;

    /**
     * Comparison is based on field "sfi".
     *
     * @param o the object to compare
     * @return the comparison evaluation
     * @since 0.9
     */
    bool operator==(const ElementaryFile& o) const;

    /**
     *
     */
    friend KEYPLECALYPSO_API std::ostream& operator<<(
        std::ostream& os, const ElementaryFile& ef);

private:
    /**
     *
     */
    const uint8_t mSfi;

    /**
     *
     */
    std::shared_ptr<FileHeader> mHeader;

    /**
     *
     */
    std::shared_ptr<FileData> mData;

    /**
     * (private)<br>
     * Constructor used only by method "clone".
     *
     * @param sfi the SFI
     * @param data the data
     */
    ElementaryFile(const uint8_t sfi, const std::shared_ptr<FileData> data);
};

}
}
}
