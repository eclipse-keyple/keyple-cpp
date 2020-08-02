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
class KEYPLECORE_API AbstractPlugin
: public AbstractSeProxyComponent, public virtual ReaderPlugin {
public:
    /**
     *
     */
    void initReaders();

    /**
     * Returns the current readers name instance map.
     *
     * The map is initialized in the constructor and may be updated in
     * background in the case of a threaded plugin {@link
     * AbstractThreadedObservablePlugin}
     *
     * @return the current readers map, can be an empty
     */
    virtual ConcurrentMap<const std::string, std::shared_ptr<SeReader>>&
        getReaders() override;

    /**
     * Returns the current list of reader names.
     *
     * The list of names is built from the current readers list
     *
     * @return a list of String
     */
    virtual const std::set<std::string> getReaderNames() const override;

    /**
     * Compare the name of the current ReaderPlugin to the name of the
     * ReaderPlugin provided in argument
     *
     * @param plugin a {@link ReaderPlugin} object
     * @return true if the names match (The method is needed for the std::set
     * lists)
     */
    virtual int compareTo(std::shared_ptr<ReaderPlugin> plugin);

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
     * The list of readers
     */
    ConcurrentMap<const std::string, std::shared_ptr<SeReader>> mReaders;

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
    AbstractPlugin(const std::string& name);

    /**
     * Init connected native readers (from third party library) and returns a
     * map of corresponding {@link SeReader} whith their name as key.
     * <p>
     * {@link SeReader} are new instances.
     * <p>
     * this method is called once in the plugin constructor.
     *
     * @return the map of AbstractReader objects.
     * @throw KeypleReaderIOException if the communication with the reader or
     *        the SE has failed
     */
    virtual ConcurrentMap<const std::string, std::shared_ptr<SeReader>>
        initNativeReaders() = 0;
};

}
}
}
}
