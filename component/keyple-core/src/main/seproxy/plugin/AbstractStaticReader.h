/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#pragma once

#include <string>
#include <stdexcept>
#include <memory>

#include "AbstractLocalReader.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

/**
 * Abstract Observable Reader class dedicated to static reader configurations.
 * <p>
 * A static reader doesn't offer card insertion/removal mechanism (e.g. AndroidOmapiReader)
 */
class AbstractStaticReader : public AbstractLocalReader {
public:
    /**
     * Prevents the use of observers that are not available in a static reader context
     */
    void addObserver(std::shared_ptr<ObservableReader::ReaderObserver> observer) override;

    /**
     *
     */
    void removeObserver(std::shared_ptr<ObservableReader::ReaderObserver> observer) override;

protected:
    /**
     *
     */
    AbstractStaticReader(const std::string &pluginName, const std::string &readerName);

    /**
     * Empty start and stopObservation implementations to avoid implementation at the plugin level
     * and remain in compliance with {@link AbstractLocalReader}.
     */
    void startObservation() override final;

    /**
     *
     */
    void stopObservation() override final;

    /**
     *
     */
    std::shared_ptr<AbstractStaticReader> shared_from_this()
    {
        return std::static_pointer_cast<AbstractStaticReader>(AbstractLocalReader::shared_from_this());
    }
};

}
}
}
}
