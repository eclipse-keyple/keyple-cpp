/**************************************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#pragma once

#include <memory>

/* Core - Event */
#include "ObservableReader.h"
#include "ObservablePlugin.h"

/* Common */
#include "LoggerFactory.h"

namespace keyple {
namespace example {
namespace generic {
namespace common {

using namespace keyple::core::seproxy::event;
using namespace keyple::common;

class ObservableReaderNotificationEngine final {
public:
    /**
     * Constructor
     */
    ObservableReaderNotificationEngine();

    /**
     *
     */
    void setPluginObserver();

    /**
     * This method is called whenever a Reader event occurs (SE
     * insertion/removal)
     */
    class SpecificReaderObserver final : public ObservableReader::ReaderObserver {
    public:
        /**
         * Constructor
         */
        SpecificReaderObserver(ObservableReaderNotificationEngine* outerInstance);

        /**
         *
         */
        void update(const std::shared_ptr<ReaderEvent> event) override;

    private:
        /**
         *
         */
        ObservableReaderNotificationEngine* mOuterInstance;
    };

    /**
     * This method is called whenever a Plugin event occurs (reader
     * insertion/removal)
     */
    class SpecificPluginObserver final : public ObservablePlugin::PluginObserver {
    public:
        /**
         *
         */
        std::shared_ptr<SpecificReaderObserver> mReaderObserver;

        /**
         * Constructor
         */
        SpecificPluginObserver(ObservableReaderNotificationEngine* outerInstance,
                               std::shared_ptr<SpecificReaderObserver> readerObserver);


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
        ObservableReaderNotificationEngine* mOuterInstance;
    };

private:
    /**
     *
     */
    const std::shared_ptr<Logger> mLogger =
        LoggerFactory::getLogger(typeid(ObservableReaderNotificationEngine));

    /**
     *
     */
    std::shared_ptr<SpecificPluginObserver> mPluginObserver;
};

}
}
}
}
