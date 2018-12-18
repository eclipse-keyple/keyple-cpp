#pragma once

#include "AbstractLocalReader.h"
#include <string>
#include <stdexcept>
#include <memory>

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
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {


                    /**
                     * Abstract Observable Reader class dedicated to static reader configurations
                     */
                    class AbstractStaticReader : public AbstractLocalReader {

                    protected:
                        AbstractStaticReader(const std::string &pluginName, const std::string &readerName);

                    public:
                        void addObserver(std::shared_ptr<Observer> observer);

                        void removeObserver(std::shared_ptr<Observer> observer);

protected:
                        std::shared_ptr<AbstractStaticReader> shared_from_this() {
                            return std::static_pointer_cast<AbstractStaticReader>(AbstractLocalReader::shared_from_this());
                        }
                    };

                }
            }
        }
    }
}
