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

#ifndef KEYPLE_SEPROXY_READER_EVENT_H
#define KEYPLE_SEPROXY_READER_EVENT_H

#include <string>
#include <vector>
#include <memory>

/* Common */
#include "Export.h"

/* Core */
#include "AbstractDefaultSelectionsResponse.h"
#include "DefaultSelectionsResponse.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace event {

using namespace keyple::core::seproxy::message;

/**
 * ReaderEvent used to notify changes at reader level
 */
class EXPORT ReaderEvent final
: public std::enable_shared_from_this<ReaderEvent> {

public:
    /**
     * The different types of reader events, reflecting the status of the reader
     * regarding the presence of the card
     */
    class EXPORT EventType final {
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
         * A SE has been inserted and the default requests process has been
         * successfully operated.
         */
        static EventType SE_MATCHED;

        /**
         * The SE has been removed and is no longer able to communicate with the
         * reader
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
        EventType(const std::string &nameValue, InnerEnum innerEnum,
                  const std::string &name);

        /**
         *
         */
        EventType(const EventType& o);

        /**
         *
         */
        virtual ~EventType() {}

        /**
         *
         */
        virtual std::string getName();

        /**
         *
         */
        bool operator == (const EventType &other);

        /**
         *
         */
        bool operator != (const EventType &other);

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
        static EventType valueOf(const std::string &name);

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
     * @param defaultSelectionsResponse the response to the default
     *        AbstractDefaultSelectionsRequest (may be null)
     */
    ReaderEvent(const std::string &pluginName, const std::string &readerName,
                EventType eventType,
                std::shared_ptr<AbstractDefaultSelectionsResponse>
                    defaultSelectionsResponse);

    /**
     *
     */
    std::string getPluginName();

    /**
     *
     */
    std::string getReaderName();

    /**
     *
     */
    EventType getEventType();

    /**
     *
     */
    std::shared_ptr<AbstractDefaultSelectionsResponse>
        getDefaultSelectionsResponse();

private:
    /**
     * The type of event
     */
    const EventType eventType;

    /**
     * The name of the plugin handling the reader that produced the event
     */
    const std::string pluginName;

    /**
     * The name of the reader that produced the event
     */
    const std::string readerName;

    /**
     * The response to the selection request Note: although the object is
     * instantiated externally, we use DefaultSelectionsResponse here to keep
     * ReaderEvent serializable
     */
    const std::shared_ptr<DefaultSelectionsResponse> defaultResponseSet;
};

}
}
}
}

#endif
