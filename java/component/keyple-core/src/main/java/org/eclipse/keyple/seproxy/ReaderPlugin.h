#pragma once

#include <set>
#include "../util/Configurable.h"
#include "../util/Nameable.h"
#include <string>
#include <memory>
#include "KeypleReaderException.h"
#include "KeypleReaderNotFoundException.h"
#include "Comparable.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class SeReader; } } } }

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

                using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                using Configurable = org::eclipse::keyple::util::Configurable;
                using Nameable = org::eclipse::keyple::util::Nameable;



                /**
                 * Card readers plugin interface.
                 */
                class ReaderPlugin : public Nameable, public Configurable, public Comparable<std::shared_ptr<ReaderPlugin>> {

                    /**
                     * Gets the readers.
                     *
                     * @return the ‘unique’ name of the readers’ plugin.
                     * @throws KeypleReaderException if the list of readers has not been initialized
                     */
                public:
                    virtual std::shared_ptr<std::set<SeReader>> getReaders() = 0;

                    /**
                     * Gets the reader whose name is provided as an argument
                     *
                     * @param name of the reader
                     * @return the SeReader object.
                     * @throws KeypleReaderNotFoundException if the wanted reader is not found
                     */
                    virtual std::shared_ptr<SeReader> getReader(const std::string &name) = 0;
                };

            }
        }
    }
}
