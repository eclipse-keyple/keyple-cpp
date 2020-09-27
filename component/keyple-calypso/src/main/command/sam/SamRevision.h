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

#include <ostream>
#include <string>
#include <vector>

/* Calypso */
#include "KeypleCalypsoExport.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {

/**
    * This enumeration registers all supported revisions of SAM.
    *
    */
class KEYPLECALYPSO_API SamRevision final {
public:
    /**
     * The revision of C1 and S1E SAM. CLA 0x00 or 0x80
     */
    static const SamRevision C1;

    /**
     * The revision of S1E SAM. CLA 0x00 or 0x80
     */
    static const SamRevision S1E;

    /**
     * The revision of S1D SAM SAM. CLA 0x94
     */
    static const SamRevision S1D;

    /**
     *
     */
    static const SamRevision NO_REV;

    /**
     * Joker value matching any application type.
     * <p>
     * Used as an argument in SamSelector.
     * <p>
     * The actual revision will be retrieved from the ATR historical bytes.
     */
    static const SamRevision AUTO;

    /**
     *
     */
    SamRevision(const SamRevision& o);

    /**
     *
     */
    const std::string& getName() const;

    /**
     *
     */
    const std::string& getApplicationTypeMask() const;

    /**
     *
     */
    uint8_t getClassByte() const;

    /**
     *
     */
    bool operator==(const SamRevision& o) const;

    /**
     *
     */
    bool operator!=(const SamRevision& o) const;

    /**
     *
     */
    SamRevision& operator=(const SamRevision& o);

    /**
     *
     */
    static const SamRevision& valueOf(const std::string& name);

	/**
	 *
	 */
    friend KEYPLECALYPSO_API std::ostream& operator<<(std::ostream & os,
                                                      const SamRevision& sr);

private:
    /**
     *
     */
    std::string mName;

    /**
     *
     */
    std::string mApplicationTypeMask;

    /**
     *
     */
    uint8_t mClassByte;

private:
    /**
     *
     */
    SamRevision(const std::string& name,
                const std::string& applicationTypeMask,
                const uint8_t classByte);
};

}
}
}
}
