/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#ifndef KEYPLED_SEPROXY_READER_EVENT_H
#define KEYPLED_SEPROXY_READER_EVENT_H

namespace keyple {
    namespace seproxy {
        namespace event {
            /**
             * ReaderEvent used to notify changes at reader level
             */
            class ReaderEvent {
            public:
                /**
                 * The different types of reader event
                 */
                enum EventType
                {
                    /**
                     * An io error occurred.
                    */
                    IO_ERROR,

                    /**
                     * A SE has been inserted.
                     */
                    SE_INSERTED,

                    /**
                     * The SE has been removed.
                     */
                    SE_REMOVAL,
                };

                /**
                 * ReaderEvent constructor
                 * 
                 * @param pluginName the name of the current plugin
                 * @param readerName the name of the current reader
                 * @param eventType the type of event
                 */
                ReaderEvent(std::string &pluginName, std::string &readerName, EventType eventType = IO_ERROR)
                    : pluginName(pluginName), readerName(readerName)
                {
                    this->eventType = eventType;
                }

              private:
                /**
                 * The name of the plugin handling the reader that produced the event
                 */
                const std::string pluginName;

                /**
                 * The name of the reader that produced the event
                 */
                const std::string readerName;

                /**
                 * The type of event
                 */
                EventType eventType;
            };
        } // namespace event
    } // namespace seproxy
} // namespace keyple

#endif // KEYPLED_SEPROXY_READER_EVENT_H
