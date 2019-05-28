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

#include <memory>
#include <set>
#include <string>

/* Common */
#include "Export.h"

/* Core */
#include "Comparable.h"
#include "Configurable.h"
#include "KeypleReaderException.h"
#include "KeypleReaderNotFoundException.h"
#include "Nameable.h"

/* Forward class declarations */
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                class SeReader;
            }
        } // namespace keyple
    }     // namespace eclipse
} // namespace org

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {

                using KeypleReaderException         = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                using Configurable                  = org::eclipse::keyple::util::Configurable;
                using Nameable                      = org::eclipse::keyple::util::Nameable;

                /**
                 * Card readers plugin interface.
                 */
                class EXPORT ReaderPlugin : public virtual Nameable,
                                            public Configurable,
                                            public Comparable<ReaderPlugin> {

                    /**
                     * Gets the list of names of all readers
                     *
                     * @return a list of String
                     */
                public:
                    virtual std::shared_ptr<std::set<std::string>> getReaderNames() = 0;

                    /**
                     * Gets the readers.
                     *
                     * @return the ‘unique’ name of the readers’ plugin.
                     * @throws KeypleReaderException if the list of readers has not been initialized
                     */
                  public:
                    virtual std::shared_ptr<std::set<std::shared_ptr<SeReader>>> getReaders() = 0;

                    /**
                     * Gets the reader whose name is provided as an argument
                     *
                     * @param name of the reader
                     * @return the SeReader object.
                     * @throws KeypleReaderNotFoundException if the wanted reader is not found
                     */
                    virtual std::shared_ptr<SeReader> getReader(const std::string &name) = 0;
                };

            } // namespace seproxy
        }     // namespace keyple
    }         // namespace eclipse
} // namespace org
