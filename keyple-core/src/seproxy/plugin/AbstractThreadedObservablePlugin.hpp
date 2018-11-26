/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#include <thread>

#include "AbstractObservablePlugin.hpp"

namespace keyple {
    namespace seproxy {
        namespace plugin {

            /**
             *
             */
            class AbstractThreadedObservablePlugin : public virtual AbstractObservablePlugin {
            public:
                /**
                 *
                 * @param name
                 */
                AbstractThreadedObservablePlugin(std::string name) : AbstractObservablePlugin(name)
                {
                    /* Create and launch the monitoring thread */
                    /*thread = new EventThread(this.getName());
                    thread.start();*/
                }
            };
        } // namespace plugin
    } // namespace seproxy
} // namespace keyple
