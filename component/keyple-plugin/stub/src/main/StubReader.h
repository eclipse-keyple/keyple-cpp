/**************************************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                                                *
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

#include "ObservableReader.h"
#include "StubSecureElement.h"

namespace keyple {
namespace plugin {
namespace stub {

using namespace keyple::core::seproxy::event;

class StubReader : public virtual ObservableReader {
public:
    /**
     *
     */
    virtual ~StubReader() = default;

    /**
     * Insert a stub se into the reader. Will raise a SE_INSERTED event.
     *
     * @param _se stub secure element to be inserted in the reader
     */
    virtual void insertSe(std::shared_ptr<StubSecureElement> _se) = 0;

    /**
     * Remove se from reader if any
     */
    virtual void removeSe() = 0;

    /**
     * Get inserted SE
     *
     * @return se, can be null if no Se inserted
     */
    virtual std::shared_ptr<StubSecureElement> getSe() = 0;
};

}
}
}
