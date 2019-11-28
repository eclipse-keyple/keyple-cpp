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
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

/* Common */
#include "exceptionhelper.h"
#include "Export.h"
#include "Logger.h"
#include "LoggerFactory.h"

/* Core */
#include "AbstractThreadedObservablePlugin.h"
#include "PluginEvent.h"
#include "SeReader.h"
#include "KeypleBaseException.h"
#include "KeypleReaderException.h"
#include "AbstractObservableReader.h"
#include "KeypleReaderNotFoundException.h"

/* PC/SC plugin */
#include "PcscTerminal.h"

namespace keyple {
namespace plugin {
namespace pcsc {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::plugin;
using namespace keyple::core::seproxy::event;
using namespace keyple::common;

class EXPORT PcscPlugin : public AbstractThreadedObservablePlugin {
public:
    /**
     *
     */
    std::set<std::string> nativeReadersNames;

    /**
     * Constructor
     */
    PcscPlugin();

    /**
     *
     */
    virtual ~PcscPlugin() { }

    /**
     *
     */
    PcscPlugin(const PcscPlugin&) = default;

    /**
     * Gets the single instance of PcscPlugin.
     *
     * @return single instance of PcscPlugin
     */

    static PcscPlugin getInstance();

    /**
     *
     */
    std::unordered_map<std::string, std::string> getParameters() override;

    /**
     *
     */
    void setParameter(const std::string &key, const std::string &value) override;

    /**
     * Enable the logging
     *
     * @param logging If logging is enabled
     * @return Same instance (fluent setter)
     * @deprecated
     */
    std::shared_ptr<PcscPlugin> setLogging(bool logging);

    /**
     *
     */
    void setParameters(std::unordered_map<std::string, std::string> &parameters) override
    {
        return AbstractThreadedObservablePlugin::AbstractLoggedObservable::setParameters(
            parameters);
    }

    /**
     *
     */
    std::shared_ptr<std::set<std::shared_ptr<SeReader>>> getReaders()  override
    {
        return AbstractThreadedObservablePlugin::AbstractObservablePlugin::getReaders();
    }

    /**
     *
     */
    std::shared_ptr<SeReader> getReader(const std::string &name) override
    {
        return AbstractThreadedObservablePlugin::AbstractObservablePlugin::getReader(name);
    }

    /**
     *
     */
    void addObserver(std::shared_ptr<PluginObserver> observer) override
    {
        logger->debug("observer\n");

        return AbstractThreadedObservablePlugin::AbstractObservablePlugin::addObserver(observer);
    }

    /**
     *
     */
    void removeObserver(std::shared_ptr<PluginObserver> observer) override
    {
        logger->debug("removeObserver\n");
        return AbstractThreadedObservablePlugin::AbstractObservablePlugin::removeObserver(observer);
    }

    /**
     *
     */
    void notifyObservers(std::shared_ptr<PluginEvent> event) override
    {
        logger->debug("notifyObservers\n");
        AbstractThreadedObservablePlugin::AbstractLoggedObservable<PluginEvent>::notifyObservers(event);
    }

    /**
     *
     */
    bool equals(std::shared_ptr<void> o) override
    {
        (void)o;
        return true; //AbstractThreadedObservablePlugin::equals(o);
    }

    /**
     *
     */
    int hashCode() override
    {
        return 0; //AbstractThreadedObservablePlugin::hashCode();
    }

protected:
    /**
     *
     */
    std::shared_ptr<std::set<std::string>> fetchNativeReadersNames() override;

    /**
     * Fetch connected native readers (from smartcard.io) and returns a list of corresponding
                         * {@link AbstractObservableReader} {@link AbstractObservableReader} are new instances.
     *
     * @return the list of AbstractObservableReader objects.
     * @throws KeypleReaderException if a reader error occurs
     */
    std::shared_ptr<std::set<std::shared_ptr<SeReader>>> initNativeReaders() override;

    /**
     * Fetch the reader whose name is provided as an argument. Returns the current reader if it is
     * already listed. Creates and returns a new reader if not.
     *
     * Throws an exception if the wanted reader is not found.
     *
     * @param name name of the reader
     * @return the reader object
     * @throws KeypleReaderException if a reader error occurs
     */
    std::shared_ptr<AbstractObservableReader> fetchNativeReader(const std::string &name) override;

    /**
     *
     */
    std::shared_ptr<PcscPlugin> shared_from_this()
    {
        return std::static_pointer_cast<PcscPlugin>(
            AbstractThreadedObservablePlugin::shared_from_this());
    }

private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(PcscPlugin));

    /**
     *
     */
    static constexpr long long SETTING_THREAD_TIMEOUT_DEFAULT = 1000;

    /**
     *
     */
    bool logging = false;

    /**
     *
     */
    std::vector<PcscTerminal> terminals;

    /**
     *
     */
    std::vector<PcscTerminal>& getTerminals();
};

}
}
}
