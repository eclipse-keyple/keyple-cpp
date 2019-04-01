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

#include <set>
#include <string>
#include <vector>
#include <memory>

/* Common */
#include "Export.h"

/* Core */
#include "ReaderEvent.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace event {

                    /**
                     * A {@link PluginEvent} is used to propagate a change of reader state in reader plugin.
                     * <p>
                     * The getReaderNames and getEventType methods allow the event recipient to retrieve the names of
                     * the readers involved and the type of the event.
                     * <p>
                     * At the moment, two types of events are supported: a connection or disconnection of the reader.
                     * <p>
                     * Since the event provides a list of reader names, a single event can be used to notify a change
                     * for one or more readers.
                     * <p>
                     * However, only one type of event is notified at a time.
                     */
                    class EXPORT PluginEvent final : public std::enable_shared_from_this<PluginEvent> {
                    private:
                        /**
                         * The name of the plugin handling the reader that produced the event
                         */
                        const std::string pluginName;

                        /**
                         * The name of the readers involved
                         */
                        std::shared_ptr<std::set<std::string>> readerNames = std::make_shared<std::set<std::string>>();

                    public:
                        class EXPORT EventType final {
                        public:
                            enum class InnerEnum {
                                READER_CONNECTED,
                                READER_DISCONNECTED
                            };

                            /**
                             * A reader has been connected.
                             */
                            static const EventType READER_CONNECTED;

                            /**
                             * A reader has been disconnected.
                             */
                            static const EventType READER_DISCONNECTED;

                        private:
                            static std::vector<EventType> valueList;

                            class StaticConstructor {
                            public:
                                StaticConstructor();
                            };

                            static StaticConstructor staticConstructor;

                        public:
                            const InnerEnum innerEnumValue;
                        private:
                            const std::string nameValue;
                            const int ordinalValue;
                            static int nextOrdinal;

                            /** The event name. */
                        private:
                            std::string name;

                        public:
                            EventType(const std::string &nameValue, InnerEnum innerEnum, const std::string &name);

                            virtual std::string getName();

                        public:
                            bool operator == (const EventType &other);

                            bool operator != (const EventType &other);

                            static std::vector<EventType> values();

                            int ordinal();

                            /**
                             *
                             */
                            friend std::ostream &operator<<(std::ostream &os, const EventType &e)
                            {
                                os << "name: " << e.name;

                                return os;
                            }

                            static EventType valueOf(const std::string &name);
                        };

                    public:
                        /**
                         * Create a PluginEvent for a single reader
                         *
                         * @param pluginName name of the plugin
                         * @param readerName name of the reader
                         * @param eventType type of the event, connection or disconnection
                         */
                        PluginEvent(const std::string &pluginName, const std::string &readerName, EventType eventType);

                        /**
                         * Create a PluginEvent for multiple readers
                         *
                         * @param pluginName name of the plugin
                         * @param readerNames list of reader names
                         * @param eventType type of the event, connection or disconnection
                         */
                        PluginEvent(const std::string &pluginName, std::shared_ptr<std::set<std::string>> readerNames, EventType eventType);

                        std::string getPluginName();

                        std::shared_ptr<std::set<std::string>> getReaderNames();

                        EventType getEventType();

                    private:

                        /**
                         * The name of the reader involved
                         */
                        const std::string readerName;

                        /**
                         * The type of event
                         */
                        const EventType eventType;

                        /**
                         * The different types of reader event
                         */
                     };
                }
            }
        }
    }
}
