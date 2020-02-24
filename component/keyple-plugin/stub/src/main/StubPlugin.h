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

#include <set>
#include <string>
#include <map>
#include <memory>

/* Core */
#include "AbstractThreadedObservablePlugin.h"
#include "TransmissionMode.h"

/* Common */
#include "Export.h"
#include "Logger.h"
#include "LoggerFactory.h"
#include "InterruptedException.h"
#include "ObservablePlugin.h"
#include "KeypleBaseException.h"

namespace keyple {
namespace plugin {
namespace stub {

using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::plugin;
using namespace keyple::common;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::protocol;

/**
 * Stubplugin allows to simulate a {@link keyple::core::seproxy::ReaderPlugin}
 * and a {@link StubSecureElement}
 */
class EXPORT StubPlugin : public ObservablePlugin {
public:
    /**
     * Plug a Stub Reader
     *
     * @param name : name of the created reader
     * @param synchronous : should the stubreader added synchronously (without
     *        waiting for the observation thread)
     */
    virtual void plugStubReader(const std::string& name, bool synchronous) = 0;

    /**
     * Plug a Stub Reader
     *
     * @param name : name of the created reader
     * @param transmissionMode : transmissionMode of the created reader
     * @param synchronous : should the stubreader added synchronously (without
     *        waiting for the observation thread)
     */
    virtual void plugStubReader(const std::string& name,
                                TransmissionMode transmissionMode,
                                bool synchronous) = 0;

    /**
     * Plug a list of stub Reader at once
     *
     * @param names : names of readers to be connected
     * @param synchronous : should the stubreader be added synchronously
     *        (without waiting for the observation thread). An READER_CONNECTED
     *        event is raised in both cases
     */
    virtual void plugStubReaders(const std::set<std::string>& names,
                                 bool synchronous) = 0;

    /**
     * Unplug a Stub Reader
     *
     * @param name the name of the reader
     * @throws KeypleReaderException in case of a reader exception
     * @param synchronous : should the stubreader be removed synchronously
     *        (without waiting for the observation thread). An
     *        READER_DISCONNECTED event is raised in both cases
     */
    virtual void unplugStubReader(const std::string& name,
                                  bool synchronous) = 0;

    /**
     * Unplug a list of readers
     *
     * @param names : names of the reader to be unplugged
     * @param synchronous : should the stubreader removed synchronously
     *        (without waiting for the observation thread). An
     *        READER_DISCONNECTED event is raised in both cases
     */
    virtual void unplugStubReaders(const std::set<std::string>& names,
                                   bool synchronous) = 0;
};

}
}
}
