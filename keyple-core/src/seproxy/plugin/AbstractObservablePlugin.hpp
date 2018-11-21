/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#ifndef KEYPLE_SEPROXY_ABSTRACT_OBSERVABLE_PLUGIN
#define KEYPLE_SEPROXY_ABSTRACT_OBSERVABLE_PLUGIN

#include <set>
#include <string>
#include <thread>

#include "AbstractLocalReader.hpp"
#include "ObservableReader.hpp"
#include "PluginEvent.hpp"
#include "ProxyReader.hpp"
#include "ReaderPlugin.hpp"

using namespace keyple::seproxy;
using namespace keyple::seproxy::event;

namespace keyple {
    namespace seproxy {
        namespace plugin {

            /**
             *
             */
            class AbstractObservablePlugin : public AbstractLoggedObservable<PluginEvent>,
                                             public ReaderPlugin {
              public:
                /**
                 * Instanciates a new ReaderPlugin. Retrieve the current readers list.
                 *
                 * Gets the list for the native method the first time (null)
                 *
                 * @param name name of the plugin
                 */
                AbstractObservablePlugin(std::string name) : AbstractLoggedObservable(name)
                {
                    /*if (readers == null)
                    {
                        try
                        {
                            readers = getNativeReaders();
                        } catch (KeypleReaderException e)
                        {
                            e.printStackTrace();
                        }
                    }*/
                }

                /**
                 * Returns the current readers list.
                 *
                 * The list is initialized in the constructor and may be updated in background in the case of a
                 * threaded plugin {@link AbstractThreadedObservablePlugin}
                 *
                 * @throws KeypleReaderException
                 * @return the current reader list, can be null if the
                 */
                std::set<ProxyReader /*AbstractObservableReader*/> const* getReaders() const override
                {
                    return NULL; //dynamic_cast<std::set<ProxyReader> const *>(readers);
                }

                /**
                 * Gets a specific reader designated by its name in the current readers list
                 *
                 * @param name of the reader
                 * @return the reader
                 * @throws KeypleReaderNotFoundException if the wanted reader is not found
                 */
                ProxyReader const& getReader(std::string name) const override
                { //throws KeypleReaderNotFoundException {
//                    for (auto reader : readers) {
//                        if (reader.getName().compare(name) == 0) {
//                            return reader;
//                        }
//                    }
                    //throw new KeypleReaderNotFoundException("Reader " + name + "not found.");
                }

            protected:
                /**
                 * The list of readers
                 */
                std::set<AbstractObservableReader> readers;
            };
        } // namespace plugin
    } // namespace seproxy
} // namespace keyple

#endif // KEYPLE_SEPROXY_ABSTRACT_OBSERVABLE_PLUGIN
