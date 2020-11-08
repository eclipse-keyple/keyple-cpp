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
#include <stdexcept>
#include <string>
#include <map>
#include <vector>

/* Common */
#include "LoggerFactory.h"

/* Core */
#include "AbstractThreadedObservablePlugin.h"
#include "PluginEvent.h"
#include "SeReader.h"
#include "KeypleReaderException.h"
#include "KeypleReaderNotFoundException.h"

/* PC/SC plugin */
#include "KeyplePluginPcscExport.h"
#include "PcscPlugin.h"
#include "PcscTerminal.h"

namespace keyple {
namespace plugin {
namespace pcsc {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::plugin;
using namespace keyple::core::seproxy::event;
using namespace keyple::common;

class KEYPLEPLUGINPCSC_API PcscPluginImpl
: public virtual AbstractThreadedObservablePlugin, public virtual PcscPlugin {
public:
    /**
     *
     */
    std::set<std::string> nativeReadersNames;

    /**
     * Gets the single instance of PcscPlugin.
     *
     * @return single instance of PcscPlugin
     * @throw KeypleReaderException if a reader error occurs
     */
    static std::shared_ptr<PcscPluginImpl> getInstance();

    /**
     *
     */
    const std::map<const std::string, const std::string>& getParameters() const override;

    /**
     *
     */
    void setParameter(const std::string& key, const std::string& value) override;

    /**
     * C++ vs. Java: temp hack
     */
    virtual const std::string& getName() const final
    {
        return AbstractThreadedObservablePlugin::getName();
    }

    /**
     *  C++ vs Java: temp hack
     */
    virtual void setParameters(const std::map<const std::string, const std::string>& parameters)
        final
    {
        AbstractSeProxyComponent::setParameters(parameters);
    }

protected:
    /**
     * Fetch the list of connected native reader (from smartcardio) and returns their names
     *
     * @return connected readers' name list
     * @throw KeypleReaderIOException if the communication with the reader or the SE has failed
     */
    const std::set<std::string>& fetchNativeReadersNames() override;

    /**
     * Fetch connected native readers (from smartcard.io) and returns a list of corresponding
     * {@link AbstractObservableReader} are new instances.
     *
     * @return the list of AbstractObservableReader objects.
     * @throw KeypleReaderException if a reader error occurs
     */
    ConcurrentMap<const std::string, std::shared_ptr<SeReader>>& initNativeReaders() override;

    /**
     * Fetch the reader whose name is provided as an argument. Returns the current reader if it is
     * already listed. Creates and returns a new reader if not.
     *
     * Throws an exception if the wanted reader is not found.
     *
     * @param name name of the reader
     * @return the reader object
     * @throw KeypleReaderNotFoundException if a reader is not found by its name
     * @throw KeypleReaderIOException if the communication with the reader or the SE has failed
     */
    std::shared_ptr<SeReader> fetchNativeReader(const std::string& name)
        override;

private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(PcscPlugin));

    /**
     * Singleton instance of SeProxyService
     */
    static std::shared_ptr<PcscPluginImpl> mInstance;

    /**
     *
     */
    std::vector<PcscTerminal> mTerminals;

    /**
     *
     */
    std::vector<PcscTerminal>& getTerminals();

    /**
     * Constructor
     */
    PcscPluginImpl();

    /**
     *
     */
    bool scardNoServiceHackNeeded;

    /**
     * /!\ C++ vs. Java: Java simply returns null. C++ will return an empty map
     *                   instead. Declare it as private memver though to allow
     *                   'const' member function.
     */
    std::map<const std::string, const std::string> mParameters;
};

}
}
}
