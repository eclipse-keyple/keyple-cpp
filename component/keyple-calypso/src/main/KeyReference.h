/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#pragma once

#include <memory>

/* Calypso */
#include "KeypleCalypsoExport.h"

namespace keyple {
namespace calypso {

/**
 * The KeyReference class groups all information about a Calypso key
 */
class KEYPLECALYPSO_API KeyReference {
public:
    /**
     * Constructor
     */
    KeyReference(const uint8_t kif, const uint8_t kvc);

    /**
     *
     */
    virtual ~KeyReference() = default;

    /**
     * @return the key identifier
     */
    virtual uint8_t getKif() const;

    /**
     * @return the key version
     */
    virtual uint8_t getKvc() const;

private:
    /**
     * Key identifier
     */
    const uint8_t mKif;

    /*
     * Key version
     s*/
    const uint8_t mKvc;
};

}
}
