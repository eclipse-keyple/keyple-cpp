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

#include <memory>

namespace keyple {
namespace calypso {

/**
 * The KeyReference class groups all information about a Calypso key
 */
class KeyReference : public std::enable_shared_from_this<KeyReference> {
public:
    /**
     * Constructor
     */
    KeyReference(char kif, char kvc);

    /**
     *
     */
    virtual ~KeyReference() {}

    /**
     * @return the key identifier
     */
    virtual char getKif();

    /**
     * @return the key version
     */
    virtual char getKvc();

private:
    /**
     * Key identifier
     */
    const char kif;

    /*
     * Key version
     s*/
    const char kvc;

};

}
}
