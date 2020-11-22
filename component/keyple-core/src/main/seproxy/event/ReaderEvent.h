/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
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

#include <ostream>
#include <string>
#include <vector>
#include <memory>

/* Common */
#include "KeypleCoreExport.h"

/* Core */
#include "AbstractDefaultSelectionsResponse.h"
#include "DefaultSelectionsResponse.h"
#include "ReaderPlugin.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace event {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::message;

/**
 * ReaderEvent used to notify changes at reader level
 */
class KEYPLECORE_API ReaderEvent final
: public std::enable_shared_from_this<ReaderEvent> {

public:
    /**
     * The different types of reader events, reflecting the status of the reader regarding the
     * presence of the card
     */
    class KEYPLECORE_API EventType final {
    public:
        /**
         * An timeout error occurred.
         */
        static EventType TIMEOUT_ERROR;

        /**
         * A SE has been inserted.
         */
        static EventType SE_INSERTED;

        /**
         * A SE has been inserted and the default requests process has been successfully operated.
         */
        static EventType SE_MATCHED;

        /**
         * The SE has been removed and is no longer able to communicate with the reader
         */
        static EventType SE_REMOVED;

        /**
         *
         */
        enum class InnerEnum {
            TIMEOUT_ERROR,
            SE_INSERTED,
            SE_MATCHED,
            SE_REMOVAL
        };

        /**
         *
         */
        const InnerEnum innerEnumValue;

        /**
         *
         */
        EventType(const std::string& nameValue, InnerEnum innerEnum, const std::string& name);

        /**
         *
         */
        EventType(const EventType& o);

        /**
         *
         */
        virtual ~EventType() = default;

        /**
         *
         */
        virtual const std::string& getName() const;

        /**
         *
         */
        bool operator==(const EventType& other) const;

        /**
         *
         */
        bool operator!=(const EventType& other) const;

        /**
         *
         */
        static std::vector<EventType> values();

        /**
         *
         */
        int ordinal();

        /**
         *
         */
        std::string toString();

        /**
         *
         */
        static EventType valueOf(const std::string& name);

        /**
         *
         */
        friend KEYPLECORE_API std::ostream& operator<<(std::ostream& os, const EventType& et);

    private:
        /**
         *
         */
        static std::vector<EventType> valueList;

        /**
         *
         */
        class StaticConstructor {
        public:
            StaticConstructor();
        };

        /**
         *
         */
        static StaticConstructor staticConstructor;

        /**
         *
         */
        const std::string nameValue;

        /**
         *
         */
        const int ordinalValue;

        /**
         *
         */
        static int nextOrdinal;

        /**
         * The event name
         */
        std::string name;
    };

    /**
     * ReaderEvent constructor for simple insertion notification mode
     *
     * @param pluginName the name of the current plugin
     * @param readerName the name of the current reader
     * @param eventType the type of event
     * @param defaultSelectionsResponse the response to the default AbstractDefaultSelectionsRequest
     *                                  (may be null)
     */
    ReaderEvent(const std::string& pluginName,
                const std::string& readerName,
                const EventType& eventType,
                std::shared_ptr<AbstractDefaultSelectionsResponse> defaultSelectionsResponse);

    /**
     * @return the name of the plugin from which the reader that generated the event comes from
     */
    const std::string& getPluginName() const;

    /**
     * @return the name of the reader that generated the event comes from
     */
    const std::string& getReaderName() const;

    /**
     * @return the type of event
     */
    const EventType& getEventType() const;

    /**
     * @return the default selection response (when the event is SE_INSERTED or SE_MATCHED)
     */
    const std::shared_ptr<AbstractDefaultSelectionsResponse> getDefaultSelectionsResponse() const;

    /**
     * @return the plugin from which the reader that generated the event comes from
     */
    const std::shared_ptr<ReaderPlugin> getPlugin() const;

    /**
     * @return the reader that generated the event comes from
     */
    const std::shared_ptr<SeReader> getReader() const;

    /**
     *
     */
    friend KEYPLECORE_API std::ostream& operator<<(std::ostream& os, const ReaderEvent& re);

    /**
     *
     */
    friend KEYPLECORE_API std::ostream& operator<<(std::ostream& os,
                                                   const std::shared_ptr<ReaderEvent>& re);

private:
    /**
     * The type of event
     */
    const EventType mEventType;

    /**
     * The name of the plugin handling the reader that produced the event
     */
    const std::string mPluginName;

    /**
     * The name of the reader that produced the event
     */
    const std::string mReaderName;

    /**
     * The response to the selection request Note: although the object is instantiated externally,
     * we use DefaultSelectionsResponse here to keep ReaderEvent serializable
     */
    const std::shared_ptr<DefaultSelectionsResponse> mDefaultResponses;
};

}
}
}
}
