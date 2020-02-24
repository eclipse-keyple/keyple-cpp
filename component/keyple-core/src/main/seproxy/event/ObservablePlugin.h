/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/
#pragma once

#include <memory>

/* Core */
#include "ReaderPlugin.h"
#include "Observable.h"

/* Common */
#include "Export.h"

/* Forward class declarations */
namespace keyple {
namespace core {
namespace seproxy {
namespace event {
class PluginEvent;
}
}
}
}

namespace keyple {
namespace core {
namespace seproxy {
namespace event {

using namespace keyple::core::seproxy;

class EXPORT ObservablePlugin : public virtual ReaderPlugin {
public:
    /**
     *
     */
    class PluginObserver : public Observer<PluginEvent> {
    public:
        /**
         *
         */
        virtual ~PluginObserver()
        {
        }

        /**
         *
         */
        virtual void update(std::shared_ptr<PluginEvent> event) = 0;
    };

    /**
     *
     */
    //    ObservablePlugin() {}

    /**
     *
     */
    //    ObservablePlugin(const ObservablePlugin& o) : Nameable(), ReaderPlugin()
    //    {
    //        (void)o;
    //    }

    /**
     *
     */
    virtual ~ObservablePlugin()
    {
    }

    /**
     *
     */
    virtual void addObserver(std::shared_ptr<PluginObserver> observer) = 0;

    /**
     *
     */
    virtual void removeObserver(std::shared_ptr<PluginObserver> observer) = 0;

    /**
     *
     */
    virtual void notifyObservers(std::shared_ptr<PluginEvent> event)
    {
        (void)event;
    }

    /**
     *
     */
    int countObservers();
};

}
}
}
}
