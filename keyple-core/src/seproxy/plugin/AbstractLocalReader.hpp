/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#ifndef KEYPLE_SEPROXY_ABSTRACT_LOCAL_READER_H
#define KEYPLE_SEPROXY_ABSTRACT_LOCAL_READER_H

#include "AbstractObservableReader.hpp"

using namespace keyple::seproxy::plugin;

namespace keyple {
    namespace seproxy {
        namespace plugin {

            /**
             *
             */
            class AbstractLocalReader : public AbstractObservableReader {
              public:
                AbstractLocalReader(const std::string &pluginName, std::string &readerName)
                    : AbstractObservableReader(pluginName, readerName)
                {
                }
            };
        } // namespace plugin
    } // namespace seproxy
} // namespace keyple

#endif // KEYPLE_SEPROXY_ABSTRACT_LOCAL_READER_H
