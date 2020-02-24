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

/* Core */
#include "KeypleBaseException.h"
#include "KeypleReaderException.h"
#include "KeypleReaderNotFoundException.h"
#include "ReaderPlugin.h"
#include "ReaderPoolPlugin.h"
#include "SeReader.h"

/* Stub */
#include "StubPluginImpl.h"
#include "StubPoolPlugin.h"
#include "StubReaderImpl.h"

namespace keyple {
namespace plugin {
namespace stub {

/**
 * Simulates a @{@link ReaderPoolPlugin} with {@link StubReaderImpl} and {@link
 * StubSecureElement} Manages allocation readers by group reference,
 * Limitations : - each group can contain only one StubReader thus one
 * StubSecureElement This class uses internally @{@link StubPluginImpl} which is
 * a singleton.
 */
class StubPoolPluginImpl : public StubPoolPlugin {
public:
    /**
     *
     */
    StubPluginImpl& stubPlugin;

    /**
     * groupReference, seReader = limitation each
     */
    std::map<const std::string, std::shared_ptr<StubReaderImpl>> readerPool;

    /**
     * Can have only one reader
     */
    std::map<const std::string, const std::string> allocatedReader;

    /**
     *
     */
    StubPoolPluginImpl(const std::string& pluginName);

    /**
     *
     */
    const std::string& getName() const override;

    /**
     *
     */
    std::set<std::string> getReaderGroupReferences() override;

    /**
     *
     */
    std::shared_ptr<SeReader>
    plugStubPoolReader(const std::string& groupReference,
                       const std::string& readerName,
                       std::shared_ptr<StubSecureElement> se) override;

    /**
     *
     */
    void unplugStubPoolReader(const std::string& groupReference) override;

    /**
     * Allocate a reader if available by groupReference
     *
     * @param groupReference the reference of the group to which the reader
     *        belongs (may be null depending on the implementation made)
     * @return seReader if available, null otherwise
     */
    std::shared_ptr<SeReader>
    allocateReader(const std::string& groupReference) override;

    /**
     * Release a reader
     *
     * @param seReader the SeReader to be released.
     */
    void releaseReader(std::shared_ptr<SeReader> seReader) override;

    /**
     *
     */
    const std::map<const std::string, const std::string>&
    listAllocatedReaders();

    /*
     * Delegate methods to embedded stub plugin
     */

    /**
     *
     */
    const std::set<std::string> getReaderNames() override;

    /**
     *
     */
    std::set<std::shared_ptr<SeReader>>& getReaders() override;

    /**
     *
     */
    std::shared_ptr<SeReader> getReader(const std::string& name) override;

    /**
     * This function overrides AbstractPlugin's or AbstractReader's function
     * through the Comparable class. We don't have that class in C++, no need
     * for override keyword.
     */
    int compareTo(std::shared_ptr<ReaderPlugin> plugin);

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
    void setParameters(
        const std::map<std::string, std::string>& parameters) override;
};

}
}
}
