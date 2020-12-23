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
#include <map>

/* Core*/
#include "AbstractReader.h"
#include "AbstractSeProxyComponent.h"
#include "KeypleCoreExport.h"
#include "ReaderPlugin.h"
#include "ProxyReader.h"
#include "SeReader.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::common;

/**
 * Observable plugin. These plugin can report when a reader is added or removed.
 */
class KEYPLECORE_API AbstractPlugin : public AbstractSeProxyComponent, public virtual ReaderPlugin {
public:
    using AbstractSeProxyComponent::getName;

    /**
     * Returns the current readers name instance map.
     *
     * The map is initialized in the constructor and may be updated in
     * background in the case of a threaded plugin
     * keyple::core::seproxy::plugin::AbstractThreadedObservablePlugin
     *
     * @return the current readers map, can be an empty
     */
    virtual ConcurrentMap<const std::string, std::shared_ptr<SeReader>>& getReaders() override;

    /**
     * Returns the current list of reader names.
     *
     * The list of names is built from the current readers list
     *
     * @return a list of String
     */
    virtual const std::set<std::string> getReaderNames() const override;

    /**
     * Gets a specific reader designated by its name in the current readers list
     *
     * @param name of the reader
     * @return the reader
     * @throws KeypleReaderNotFoundException if the wanted reader is not found
     *
     * Alex: once again the std::shared_ptr return type prevents us from using
     *       covariant return type. Downcasting to SeReader for now, but should
     *       be looked into, maybe returning a reference would
     *       could be best here?
     */
    const std::shared_ptr<SeReader> getReader(const std::string& name) final;

protected:
    /**
     * Instanciates a new ReaderPlugin. Retrieve the current readers list.
     *
     * Initialize the list of readers calling the abstract method
     * initNativeReaders
     *
     * When readers initialisation failed, a KeypleReaderException is thrown
     *
     * @param name name of the plugin
     * @throw KeypleReaderException when an issue is raised with reader
     */
    explicit AbstractPlugin(const std::string& name);

    /**
     *
     */
    virtual ~AbstractPlugin() = default;

    /**
     * Init connected native readers (from third party library) and returns a
     * map of corresponding keyple::core::seproxy::SeReader whith their name as key.
     * <p>
     * keyple::core::seproxy::SeReader are new instances.
     * <p>
     * this method is called once in the plugin constructor.
     *
     * @return the map of AbstractReader objects.
     * @throw KeypleReaderIOException if the communication with the reader or
     *        the SE has failed
     */
    virtual ConcurrentMap<const std::string, std::shared_ptr<SeReader>>& initNativeReaders() = 0;

protected:
    /**
     * C++ vs. Java: store list in class to avoid copy or move operations on ConcurrentMap
     */
    ConcurrentMap<const std::string, std::shared_ptr<SeReader>> mNativeReaders;
};

}
}
}
}
