/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#ifndef KEYPLE_SEPROXY_ABSTRACT_THREADED_LOCAL_READER_H
#define KEYPLE_SEPROXY_ABSTRACT_THREADED_LOCAL_READER_H

#include "AbstractSelectionLocalReader.hpp"

namespace keyple {
    namespace seproxy {
        namespace plugin {

            /**
             *
             */
            class AbstractThreadedLocalReader : public AbstractSelectionLocalReader {
              public:
                /**
                 *
                 */
                AbstractThreadedLocalReader(const std::string &pluginName, std::string &readerName)
                    : AbstractSelectionLocalReader(pluginName, readerName)
                {
                    /// create and launch a monitoring thread
                    //thread = new EventThread(this.getPluginName(), this.getName());
                    //thread.start();
                }
                /**
                 * setter to fix the wait timeout in ms.
                 *
                 * @param timeout Timeout to use
                 */
                virtual void setThreadWaitTimeout(long timeout)
                {
                    this->threadWaitTimeout = timeout;
                }

                /**
                 * Waits for a card. Returns true if a card is detected before the end of the provided timeout.
                 * Returns false if no card detected within the delay.
                 *
                 * @param timeout the delay in millisecond we wait for a card insertion
                 * @return presence status
                 * @throws NoStackTraceThrowable a exception without stack trace in order to be catched and
                 *         processed silently
                 */
                virtual bool waitForCardPresent(long timeout);
                // throws NoStackTraceThrowable;

                /**
                 * Wait until the card disappears. Returns true if a card has disappeared before the end of the
                 * provided timeout. Returns false if the is still present within the delay. Closes the physical
                 * channel when the card has disappeared.
                 *
                 * @param timeout the delay in millisecond we wait for a card to be withdrawn
                 * @return presence status
                 * @throws NoStackTraceThrowable a exception without stack trace in order to be catched and
                 *         processed silently
                 */
                virtual bool waitForCardAbsent(long timeout);
                //throws NoStackTraceThrowable;

              private:
                /**
                 * Thread wait timeout in ms
                 */
                long threadWaitTimeout;
            };
        } // namespace plugin
    } // namespace seproxy
} // namespace keyple

#endif // KEYPLE_SEPROXY_ABSTRACT_THREADED_LOCAL_READER_H
