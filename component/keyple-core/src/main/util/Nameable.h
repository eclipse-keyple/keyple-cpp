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

namespace keyple {
namespace core {
namespace util {

/**
 * Allow {@link ProxyReader}s and {@link ReaderPlugin}s to be named.
 */
class Nameable {

public:
    /**
     *
     */
    Nameable() {}

    /**
     *
     */
    Nameable(const Nameable& o)
    {
        (void)o;
    }

    /**
     *
     */
    virtual ~Nameable() {}

    /**
     *
     * @return the unique name of the item
     */
    virtual const std::string& getName() const = 0;
};

}
}
}
