#pragma once

#include "../ReaderPlugin.h"
#include "../../util/Observable.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace @event { class PluginEvent; } } } } }

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
                namespace event_Renamed {

                    using ReaderPlugin = org::eclipse::keyple::seproxy::ReaderPlugin;
                    using Observable = org::eclipse::keyple::util::Observable;

                    class ObservablePlugin : public ReaderPlugin {
                    private:
                        class PluginObserver : public Observable::Observer<std::shared_ptr<PluginEvent>> {
                        public:
                            virtual void update(std::shared_ptr<PluginEvent> event_Renamed) = 0;
                        };

                    public:
                        virtual void addObserver(std::shared_ptr<PluginObserver> observer) = 0;

                        virtual void removeObserver(std::shared_ptr<PluginObserver> observer) = 0;

                        virtual void notifyObservers(std::shared_ptr<PluginEvent> event_Renamed) = 0;
                    };

                }
            }
        }
    }
}
