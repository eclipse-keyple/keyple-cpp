/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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

#include <set>
#include <string>
#include <vector>
#include <memory>

/* Common */
#include "KeypleCoreExport.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace event {

/**
 * A {@link PluginEvent} is used to propagate a change of reader currentState in
 * reader plugin.
 * <p>
 * The getReaderNames and getEventType methods allow the event recipient to
 * retrieve the names of the readers involved and the type of the event.
 * <p>
 * At the moment, two types of events are supported: a connection or
 * disconnection of the reader.
 * <p>
 * Since the event provides a list of reader names, a single event can be used
 * to notify a change for one or more readers.
 * <p>
 * However, only one type of event is notified at a time.
 */
class KEYPLECORE_API PluginEvent final {
public:
    /**
     *
     */
    class KEYPLECORE_API EventType final {
    public:
        /**
         * A reader has been connected.
         */
        static const EventType READER_CONNECTED;

        /**
         * A reader has been disconnected.
         */
        static const EventType READER_DISCONNECTED;

        /**
         *
         */
        virtual const std::string& getName() const;

        /**
         *
         */
        bool operator==(const EventType& o) const;

        /**
         *
         */
        bool operator!=(const EventType& o) const;

        /**
         *
         */
        friend KEYPLECORE_API std::ostream& operator<<(std::ostream& os,
                                                       const EventType& et);

        /**
         *
         */
        static const EventType& valueOf(const std::string& name);

    private:
        /**
         *
         */
        EventType(const std::string& name);
        /**
         * The event name
         */
        const std::string mName;
    };

    /**
     * Create a PluginEvent for a single reader
     *
     * @param pluginName name of the plugin
     * @param readerName name of the reader
     * @param eventType type of the event, connection or disconnection
     */
    PluginEvent(const std::string& pluginName,
                const std::string& readerName,
                const EventType& eventType);

    /**
     * Create a PluginEvent for multiple readers
     *
     * @param pluginName name of the plugin
     * @param readerNames list of reader names
     * @param eventType type of the event, connection or disconnection
     */
    PluginEvent(const std::string& pluginName,
                std::shared_ptr<std::set<std::string>> readerNames,
                const EventType& eventType);

    /**
     *
     */
    const std::string& getPluginName() const;

    /**
     *
     */
    const std::set<std::string>& getReaderNames() const;

    /**
     *
     */
    const EventType& getEventType() const;

    /**
	 *
	 */
    friend KEYPLECORE_API std::ostream& operator<<(std::ostream& os,
		                                           const PluginEvent& re);

private:
    /**
     * The name of the reader involved
     */
    const std::string mReaderName;

    /**
     * The type of event
     */
    const EventType& mEventType;

    /**
    * The name of the plugin handling the reader that produced the event
    */
    const std::string mPluginName;

    /**
    * The name of the readers involved
    */
    std::set<std::string> mReaderNames;
};

}
}
}
}
