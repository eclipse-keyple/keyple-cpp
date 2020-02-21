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

/* Stub Plugin */
#include "ReaderPoolPlugin.h"
#include "StubSecureElement.h"

/* Core */
#include "SeReader.h"

namespace keyple {
namespace plugin {
namespace stub {

using namespace keyple::core::seproxy;

/**
 * StubPoolPlugin allows to simulate a {@link
 * keyple::core::seproxy::ReaderPoolPlugin} and a {@link StubSecureElement}
 */
class StubPoolPlugin : public ReaderPoolPlugin {
public:
    /**
     * Plug synchronously a new @{@link StubReaderImpl} in Pool with
     * groupReference and a StubSE. A READER_CONNECTED event will be raised.
     *
     * @param groupReference : group refence of the new stub reader
     * @param readerName : name of the new stub reader
     * @param se : insert a se at creation (can be null)
     * @return created StubReader
     */
    virtual std::shared_ptr<SeReader> plugStubPoolReader(
        const std::string& groupReference, const std::string& readerName,
        std::shared_ptr<StubSecureElement> se) = 0;

    /**
     * Unplug synchronously a new reader by groupReference. A
     * READER_DISCONNECTED event will be raised.
     *
     * @param groupReference groupReference of the reader to be unplugged
     */
    virtual void unplugStubPoolReader(const std::string& groupReference) = 0;

};

}
}
}
