/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#include "AbstractLoggedObservable.hpp"
#include "ReaderEvent.hpp"

namespace keyple {
    namespace seproxy {
        namespace plugin {
            class AbstractObservableReader : public AbstractLoggedObservable<keyple::seproxy::event::ReaderEvent> {
              public:
                /**
                 * Reader constructor
                 *
                 * Force the definition of a name through the use of super method.
                 *
                 * @param pluginName the name of the plugin that instantiated the reader
                 * @param readerName the name of the reader
                 */
                AbstractObservableReader(std::string &pluginName, std::string &readerName)
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
                 * Timestamp recorder
                 */
                long before; 
            };
        } // namespace plugin
    } // namespace seproxy
} // namespace keyple
