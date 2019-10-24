/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
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

#include <string>
#include <vector>

/* Common */
#include "Export.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {

/**
    * This enumeration registers all supported revisions of SAM.
    *
    */
class EXPORT SamRevision final {
public:
    /**
     * The revision of C1 and S1E SAM. CLA 0x00 or 0x80
     */
    static SamRevision C1;

    /**
     * The revision of S1E SAM. CLA 0x00 or 0x80
     */
    static SamRevision S1E;

    /**
     * The revision of S1D SAM SAM. CLA 0x94
     */
    static SamRevision S1D;

    /**
     *
     */
    static SamRevision NO_REV;

    /**
     * Joker value matching any application type.
     * <p>
     * Used as an argument in SamSelector.
     * <p>
     * The actual revision will be retrieved from the ATR historical bytes.
     */
    static SamRevision AUTO;

    /**
     *
     */
    enum class InnerEnum {
        NO_REV,
        C1,
        S1E,
        S1D,
        AUTO
    };

    /**
     *
     */
    InnerEnum innerEnumValue;

    /**
     *
     */
    SamRevision(const std::string &nameValue, InnerEnum innerEnum,
                const std::string &name, const std::string &applicationTypeMask,
                char classByte);

    /**
     *
     */
    SamRevision(const SamRevision& o);

    /**
     *
     */
    virtual ~SamRevision() {}

    /**
     *
     */
    virtual std::string getName();

    /**
     *
     */
    virtual std::string getApplicationTypeMask();

    /**
     *
     */
    virtual char getClassByte();

    /**
     *
     */
    bool operator==(const SamRevision &other);

    /**
     *
     */
    bool operator!=(const SamRevision &other);

    /**
     *
     */
    SamRevision& operator=(SamRevision o);
    /**
     *
     */
    static std::vector<SamRevision> values();

    /**
     *
     */
    int ordinal();

    /**
     *
     */
    std::string toString();

    /**
     *
     */
    static SamRevision valueOf(const std::string &name);

private:
    /**
     *
     */
    static std::vector<SamRevision> valueList;

    /**
     *
     */
    class StaticConstructor {
    public:
        /**
         *
         */
        StaticConstructor();
    };

    /**
     *
     */
    static StaticConstructor staticConstructor;

    /**
     *
     */
    std::string nameValue;

    /**
     *
     */
    int ordinalValue;

    /**
     *
     */
    static int nextOrdinal;

    /**
     *
     */
    std::string name;

    /**
     *
     */
    std::string applicationTypeMask;

    /**
     *
     */
    char classByte;
};

}
}
}
}
