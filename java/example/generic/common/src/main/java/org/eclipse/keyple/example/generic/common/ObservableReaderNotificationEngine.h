/********************************************************************************
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * See the NOTICE file(s) distributed with this work for additional information
 * regarding copyright ownership.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0
 *
 * SPDX-License-Identifier: EPL-2.0
 ********************************************************************************/

#pragma once

#include <memory>

/* Core - Event */
#include "ObservableReader.h"
#include "ObservablePlugin.h"

/* Common */
#include "Logger.h"
#include "LoggerFactory.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace example {
namespace generic {
namespace common {

using ObservablePlugin = org::eclipse::keyple::seproxy::event::ObservablePlugin;
using ObservableReader = org::eclipse::keyple::seproxy::event::ObservableReader;
using PluginEvent = org::eclipse::keyple::seproxy::event::PluginEvent;
using ReaderEvent = org::eclipse::keyple::seproxy::event::ReaderEvent;

class ObservableReaderNotificationEngine
: public std::enable_shared_from_this<ObservableReaderNotificationEngine> {
public:
    /**
     * Constructor
     */
    ObservableReaderNotificationEngine();

    /**
     *
     */
    virtual void setPluginObserver();

    /**
     * This method is called whenever a Reader event occurs (SE insertion/removal)
     */
    class SpecificReaderObserver
    : public std::enable_shared_from_this<SpecificReaderObserver>,
      public ObservableReader::ReaderObserver {
    private:
        /**
         *
         */
        ObservableReaderNotificationEngine *outerInstance;


    public:
        /**
         * Constructor
         */
        SpecificReaderObserver(ObservableReaderNotificationEngine *outerInstance); //super();

        virtual void update(std::shared_ptr<ReaderEvent> event_Renamed);
    };

    /**
     * This method is called whenever a Plugin event occurs (reader insertion/removal)
     */
    class SpecificPluginObserver
    : public std::enable_shared_from_this<SpecificPluginObserver>,
      public ObservablePlugin::PluginObserver {
    private:
        /**
         *
         */
        ObservableReaderNotificationEngine *outerInstance;

    public:
        /**
         *
         */
        std::shared_ptr<SpecificReaderObserver> readerObserver;

        /**
         * Constructor
         */
        SpecificPluginObserver(ObservableReaderNotificationEngine *outerInstance, std::shared_ptr<SpecificReaderObserver> readerObserver);

        void update(std::shared_ptr<PluginEvent> event) override;
    };

private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger;

    /**
     *
     */
    std::shared_ptr<SpecificPluginObserver> pluginObserver;
};

}
}
}
}
}
}
