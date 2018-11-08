/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * This program and the accompanying materials are made available under the terms of the Eclipse
 * Public License version 2.0 which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 */

#ifndef KEYPLE_SEPROXY_READER_PLUGIN_H
#define KEYPLE_SEPROXY_READER_PLUGIN_H

#include <set>

#include "NameableConfigurable.hpp"
#include "ProxyReader.hpp"
#include "ReaderPlugin.hpp"

namespace keyple {
    namespace seproxy {

        /**
         *
         */
        class ReaderPlugin : public NameableConfigurable {
          public:
            /**
             * Gets the readers.
             *
             * @return the ‘unique’ name of the readers’ plugin.
             * @throws KeypleReaderException if the list of readers has not been initialized
             */
            virtual std::set<ProxyReader> *getReaders() = 0; //throws KeypleReaderException;

            /**
             * Gets the reader whose name is provided as an argument
             * 
             * @param name of the reader
             * @return the ProxyReader object.
             * @throws KeypleReaderNotFoundException if the wanted reader is not found
             */
            virtual ProxyReader *getReader(std::string name) = 0; // throws KeypleReaderNotFoundException;
        };
    } // namespace seproxy
} // namespace keyple

#endif // KEYPLE_SEPROXY_READER_PLUGIN_H
