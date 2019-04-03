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

#include "../ReaderPlugin.h"
#include "../../util/Observable.h"
#include <memory>
#include "Observable.h"

/* Common */
#include "Export.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace event { class PluginEvent; } } } } }

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace event {

                    using ReaderPlugin = org::eclipse::keyple::seproxy::ReaderPlugin;

                    class EXPORT ObservablePlugin : public virtual ReaderPlugin {
                    public:
                        class PluginObserver : public org::eclipse::keyple::util::Observer<PluginEvent> {
                        public:
                            virtual void update(std::shared_ptr<PluginEvent> event) = 0;
                        };

                    public:
                        virtual void addObserver(std::shared_ptr<PluginObserver> observer) = 0;

                        virtual void removeObserver(std::shared_ptr<PluginObserver> observer) = 0;

                        virtual void notifyObservers(std::shared_ptr<PluginEvent> event) { (void)event; }
                    };

                }
            }
        }
    }
}
