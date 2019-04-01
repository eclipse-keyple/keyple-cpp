#pragma once

#include "AbstractObservablePlugin.h"
#include <string>
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
                     * Abstract Observable Plugin class dedicated to static reader configurations
                     */
                    class AbstractStaticPlugin : public AbstractObservablePlugin {
                    protected:
                        AbstractStaticPlugin(const std::string &name);

                        /**
                         * Empty start and stopObservation implementations to avoid implementation at the plugin level
                         * and remain in compliance with {@link AbstractObservablePlugin}.
                         */

                        void startObservation() override final;

                        void stopObservation() override final;

                        /** Prevents the use of observers that are not available in a static reader context */

                    public:
                        void addObserver(std::shared_ptr<ObservablePlugin::PluginObserver> observer);

                        void removeObserver(std::shared_ptr<ObservablePlugin::PluginObserver> observer);

protected:
                        std::shared_ptr<AbstractStaticPlugin> shared_from_this() {
                            return std::static_pointer_cast<AbstractStaticPlugin>(AbstractObservablePlugin::shared_from_this());
                        }
                    };

                }
            }
        }
    }
}
