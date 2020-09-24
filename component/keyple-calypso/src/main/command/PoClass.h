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

#include <ostream>
#include <string>
#include <vector>

/* Common */
#include "KeypleCalypsoExport.h"

namespace keyple {
namespace calypso {
namespace command {

/*
 * Class for the Calypso command: LEGACY for REV1 / BPRIME type PO, ISO for
 * REV2/3 / B type
 */
class KEYPLECALYPSO_API PoClass final {
public:
    /**
     *
     */
    static const PoClass LEGACY;

    /**
     *
     */
    static const PoClass ISO;

    /**
     *
     */
    virtual uint8_t getValue() const;

    /**
     *
     */
    PoClass(const std::string& name, const uint8_t cla);

    /**
     *
     */
    virtual ~PoClass() = default;

    /**
     *
     */
    PoClass& operator=(const PoClass& o);

    /**
     *
     */
    bool operator==(const PoClass& o) const;

    /**
     *
     */
    bool operator!=(const PoClass& o) const;

    /**
     *
     */
    static const PoClass& valueOf(const std::string& name);

	/**
	 *
	 */
	friend std::ostream& operator<<(std::ostream& os, const PoClass& pc);

private:
    /**
     *
     */
    std::string mName;

    /**
     *
     */
    uint8_t mCla;
};

}
}
}
