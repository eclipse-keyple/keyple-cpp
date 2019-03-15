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

#ifndef KEYPLE_SEPROXY_READER_EVENT_H
#define KEYPLE_SEPROXY_READER_EVENT_H

#include <string>
#include <vector>
#include <memory>

/* Common */
#include "Export.h"

/* Core */
#include "SelectionResponse.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace event {


                    using SelectionResponse = org::eclipse::keyple::transaction::SelectionResponse;

                    /**
                     * ReaderEvent used to notify changes at reader level
                     */
                    class EXPORT ReaderEvent final : public std::enable_shared_from_this<ReaderEvent> {
                    public:
                        class EXPORT EventType final {
                            /**
                             * An io error occurred.
                             */
                         public:
                            static EventType IO_ERROR;

                            /**
                             * A SE has been inserted.
                             */
                            static EventType SE_INSERTED;

                            /**
                             * A SE has been inserted and the default requests process has been operated.
                             */
                            static EventType SE_MATCHED;

                            /**
                             * The SE has been removed.
                             */
                            static EventType SE_REMOVAL;

                        private:
                            static std::vector<EventType> valueList;

                            class StaticConstructor {
                            public:
                                StaticConstructor();
                            };

                            static StaticConstructor staticConstructor;

                        public:
                            enum class InnerEnum {
                                IO_ERROR,
                                SE_INSERTED,
                                SE_MATCHED,
                                SE_REMOVAL
                            };

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

                            std::string toString();

                            static EventType valueOf(const std::string &name);
                        };

                        /**
                         * The name of the plugin handling the reader that produced the event
                         */
                    private:
                        const std::string pluginName;

                        /**
                         * The name of the reader that produced the event
                         */
                        const std::string readerName;

                        /**
                         * The type of event
                         */
                        const EventType eventType;

                        /**
                         * The response to the selection request
                         */
                        const std::shared_ptr<SelectionResponse> defaultResponseSet;

                        /**
                         * The different types of reader event
                         */

                        /**
                         * ReaderEvent constructor for simple insertion notification mode
                         *
                         * @param pluginName the name of the current plugin
                         * @param readerName the name of the current reader
                         * @param eventType the type of event
                         * @param selectionResponse the response to the default {@link SelectionRequest} (may be null)
                         */
                    public:
                        ReaderEvent(const std::string &pluginName, const std::string &readerName, EventType eventType, std::shared_ptr<SelectionResponse> selectionResponse);


                        std::string getPluginName();

                        std::string getReaderName();

                        EventType getEventType();

                        std::shared_ptr<SelectionResponse> getDefaultSelectionResponse();
                    };

                }
            }
        }
    }
}

#endif
