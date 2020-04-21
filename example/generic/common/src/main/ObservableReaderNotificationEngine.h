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

/* Core - Event */
#include "ObservableReader_Import.h"
#include "ObservablePlugin.h"

/* Common */
#include "LoggerFactory.h"

namespace keyple {
namespace example {
namespace generic {
namespace common {

using namespace keyple::core::seproxy::event;
using namespace keyple::common;

class ObservableReaderNotificationEngine
: public std::enable_shared_from_this<ObservableReaderNotificationEngine> {
public:
    /**
     * Constructor
     */
    ObservableReaderNotificationEngine();

    /**
     * Destructor
     */
    virtual ~ObservableReaderNotificationEngine()
    {
    }

    /**
     *
     */
    virtual void setPluginObserver();

    /**
     * This method is called whenever a Reader event occurs (SE
     * insertion/removal)
     */
    class SpecificReaderObserver
    : public std::enable_shared_from_this<SpecificReaderObserver>,
      public ObservableReader::ReaderObserver {
    public:
        /**
         * Constructor
         */
        SpecificReaderObserver(
            ObservableReaderNotificationEngine* outerInstance); //super();

        /**
         *
         */
        virtual ~SpecificReaderObserver()
        {
        }

        /**
         *
         */
        void update(std::shared_ptr<ReaderEvent> event); // override;

        /**
         *
         */
        void update(ReaderEvent event); // override;

    private:
        /**
         *
         */
        ObservableReaderNotificationEngine* outerInstance;
    };

    /**
     * This method is called whenever a Plugin event occurs (reader
     * insertion/removal)
     */
    class SpecificPluginObserver
    : public std::enable_shared_from_this<SpecificPluginObserver>,
      public ObservablePlugin::PluginObserver {
    public:
        /**
         *
         */
        std::shared_ptr<SpecificReaderObserver> readerObserver;

        /**
         * Constructor
         */
        SpecificPluginObserver(
            ObservableReaderNotificationEngine* outerInstance,
            std::shared_ptr<SpecificReaderObserver> readerObserver);

        /**
         *
         */
        virtual ~SpecificPluginObserver()
        {
        }

        /**
         *
         */
        void update(std::shared_ptr<PluginEvent> event) override;

        /**
         *
         */
        void update(PluginEvent event);

    private:
        /**
         *
         */
        ObservableReaderNotificationEngine* outerInstance;
    };

private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(ObservableReaderNotificationEngine));

    /**
     *
     */
    std::shared_ptr<SpecificPluginObserver> pluginObserver;
};

}
}
}
}
