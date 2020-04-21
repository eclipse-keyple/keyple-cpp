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
#include "LoggerFactory.h"

/* Core */
#include "AbstractThreadedObservablePlugin.h"
#include "PluginEvent.h"
#include "SeReader.h"
#include "KeypleBaseException.h"
#include "KeypleReaderException.h"
#include "KeypleReaderNotFoundException.h"

/* PC/SC plugin */
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

class EXPORT PcscPluginImpl : public AbstractThreadedObservablePlugin,
                              public virtual PcscPlugin {
public:
    /**
     *
     */
    std::set<std::string> nativeReadersNames;

    /**
     *
     */
    virtual ~PcscPluginImpl()
    {
    }

    /**
     *
     */
    PcscPluginImpl(const PcscPluginImpl&) = default;

    /**
     * Gets the single instance of PcscPlugin.
     *
     * @return single instance of PcscPlugin
     */
    static PcscPluginImpl& getInstance();

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

protected:
    /**
     * Fetch the list of connected native reader (from smartcardio) and returns
     * their names
     *
     * @return connected readers' name list
     * @throws KeypleReaderException if a reader error occurs
     */
    const std::set<std::string>& fetchNativeReadersNames() override;

    /**
     * Fetch connected native readers (from smartcard.io) and returns a list of
     * corresponding {@link AbstractObservableReader} are new instances.
     *
     * @return the list of AbstractObservableReader objects.
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
     * @param name name of the reader
     * @return the reader object
     * @throws KeypleReaderException if a reader error occurs
     */
    std::shared_ptr<SeReader>
        fetchNativeReader(const std::string& name) override;

    /**
     *
     */
    void addObserver(std::shared_ptr<PluginObserver> observer) override;

    /**
     *
     */
    void removeObserver(std::shared_ptr<PluginObserver> observer) override;

private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(PcscPlugin));

    /**
     *
     */
    static constexpr long long SETTING_THREAD_TIMEOUT_DEFAULT = 1000;

    /**
     * singleton instance of SeProxyService
     */
    static PcscPluginImpl uniqueInstance;

    /**
     * /!\ clang compiler warning - not used
     */
    //bool logging = false;

    /**
     *
     */
    std::vector<PcscTerminal> terminals;

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
};

}
}
}
