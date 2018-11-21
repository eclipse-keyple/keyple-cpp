/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#include "AbstractLoggedObservable.hpp"
#include "ProxyReader.hpp"
#include "ReaderEvent.hpp"

using namespace keyple::seproxy::event;

namespace keyple {
    namespace seproxy {
        namespace plugin {
            class AbstractObservableReader : public AbstractLoggedObservable<ReaderEvent>,
                                             public ProxyReader {
              public:
                /**
                 * Reader constructor
                 *
                 * Force the definition of a name through the use of super method.
                 *
                 * @param pluginName the name of the plugin that instantiated the reader
                 * @param readerName the name of the reader
                 */
                AbstractObservableReader(const std::string &pluginName, std::string &readerName)
                    : AbstractLoggedObservable(readerName), pluginName(pluginName)
                {
                    this->before     = 0; //System.nanoTime();
                }

              private:
                /**
                 * The name of the plugin handling the reader that produced the event
                 */
                const std::string pluginName;
                
                /**
                 *
                 */
                const std::string name;
              
                /**
                 * Timestamp recorder
                 */
                long before; 
            };
        } // namespace plugin
    } // namespace seproxy
} // namespace keyple
