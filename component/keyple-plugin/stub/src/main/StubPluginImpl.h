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

#include <map>
#include <set>

/* Core */
#include "AbstractThreadedObservablePlugin.h"
#include "SeReader.h"

/* Common */
#include "LoggerFactory.h"

/* Stub Plugin */
#include "StubPlugin.h"

namespace keyple {
namespace plugin {
namespace stub {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::plugin;

/**
 * This plugin allows to simulate Secure Element communication by creating
 * @{@link StubReaderImpl} and @{@link StubSecureElement}. Plug a new StubReader
 * with StubPlugin#plugStubReader and insert an implementation of your own of
 * {@link StubSecureElement} to start simulation communication. This class is a
 * singleton, use StubPlugin#getInstance to access it
 *
 */
class StubPluginImpl : public AbstractThreadedObservablePlugin,
                       public StubPlugin {
public:
    /**
     * Constructor
     *
     * @param pluginName : custom name for the plugin
     */
    StubPluginImpl(const std::string& pluginName);

    /**
     * Gets the single instance of StubPlugin.
     *
     * @return single instance of StubPlugin
     */
    /*
     * public static StubPlugin getInstance() { return uniqueInstance; }
     */

    /**
     *
     */
    const std::map<const std::string, const std::string>
    getParameters() override;

    /**
     *
     */
    void setParameter(const std::string& key,
                      const std::string& value) override;

    /**
     *
     */
    void plugStubReader(const std::string& readerName,
                        bool synchronous) override;

    /**
     *
     */
    void plugStubReader(const std::string& readerName,
                        TransmissionMode transmissionMode,
                        bool synchronous) override;

    /**
     *
     */
    void plugStubReaders(const std::set<std::string>& readerNames,
                         bool synchronous) override;

    /**
     *
     */
    void unplugStubReader(const std::string& readerName,
                          bool synchronous) override;

    /**
     *
     */
    void unplugStubReaders(const std::set<std::string>& readerNames,
                           bool synchronous) override;

    /**
     * Fetch the list of connected native reader (from a simulated list) and
     * returns their names (or id)
     *
     * @return connected readers' name list
     */
    const std::set<std::string>& fetchNativeReadersNames() override;

    /**
     *
     */
    void addObserver(std::shared_ptr<PluginObserver> observer) override;

    /**
     *
     */
    void removeObserver(std::shared_ptr<PluginObserver> observer) override;

protected:
    /**
     * Init native Readers to empty Set
     *
     * @return the list of SeReader objects.
     * @throws KeypleReaderException if a reader error occurs
     */
    std::set<std::shared_ptr<SeReader>> initNativeReaders() override;

    /**
     * Fetch the reader whose name is provided as an argument. Returns the
     * current reader if it is already listed. Creates and returns a new reader
     * if not.
     *
     * Throws an exception if the wanted reader is not found.
     *
     * @param readerName name of the reader
     * @return the reader object
     */
    std::shared_ptr<SeReader>
    fetchNativeReader(const std::string& readerName) override;

private:
    // private static final StubPlugin uniqueInstance = new StubPlugin();

    /**
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(StubPluginImpl));

    /**
     *
     */
    std::map<const std::string, const std::string> parameters;

    /**
     * Simulated list of real-time connected stubReader
     */
    std::set<std::string> connectedStubNames;
};

}
}
}
