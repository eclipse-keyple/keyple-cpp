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

#include <string>
#include <stdexcept>
#include <memory>

/* Common */
#include "Export.h"
#include "Logger.h"

/* Core */
#include "AbstractLoggedObservable.h"
#include "KeypleChannelStateException.h"
#include "KeypleIOReaderException.h"
#include "KeypleReaderException.h"
#include "ObservableReader.h"
#include "ProxyReader.h"

/* Namespace */
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {

                    /* Using */
                    using SeReader         = org::eclipse::keyple::seproxy::SeReader;
                    using ObservableReader = org::eclipse::keyple::seproxy::event::ObservableReader;
                    using ReaderEvent      = org::eclipse::keyple::seproxy::event::ReaderEvent;
                    using KeypleChannelStateException =
                        org::eclipse::keyple::seproxy::exception::KeypleChannelStateException;
                    using KeypleIOReaderException =
                        org::eclipse::keyple::seproxy::exception::KeypleIOReaderException;
                    using KeypleReaderException =
                        org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using ProxyReader      = org::eclipse::keyple::seproxy::message::ProxyReader;
                    using SeRequest        = org::eclipse::keyple::seproxy::message::SeRequest;
                    using SeRequestSet     = org::eclipse::keyple::seproxy::message::SeRequestSet;
                    using SeResponse       = org::eclipse::keyple::seproxy::message::SeResponse;
                    using SeResponseSet    = org::eclipse::keyple::seproxy::message::SeResponseSet;
                    using SelectionRequest = org::eclipse::keyple::transaction::SelectionRequest;
                    using LoggerFactory    = org::eclipse::keyple::common::LoggerFactory;
                    using Logger = org::eclipse::keyple::common::Logger;

                    /**
                     *
                     * Abstract definition of an observable reader. Factorizes setSetProtocols and will factorize the
                     * transmit method logging
                     *
                     */

                    class EXPORT AbstractObservableReader : public AbstractLoggedObservable<ReaderEvent>,
                                                            public ObservableReader,
                                                            public ProxyReader {
                      public:
                        using AbstractLoggedObservable<ReaderEvent>::name;
                        /**
                         * Add a reader observer.
                         * <p>
                         * The observer will receive all the events produced by this reader (card insertion, removal,
                         * etc.)
                         * <p>
                         * The startObservation() is called when the first observer is added. (to start a monitoring
                         * thread for instance)
                         *
                         * @param observer the observer object
                         */
                        void addObserver(std::shared_ptr<ObservableReader::ReaderObserver> observer) override;

                        /**
                         * Remove a reader observer.
                         * <p>
                         * The observer will not receive any of the events produced by this reader.
                         * <p>
                         * The stopObservation() is called when the last observer is removed. (to stop a monitoring
                         * thread for instance)
                         *
                         * @param observer the observer object
                         */
                        void
                        removeObserver(std::shared_ptr<ObservableReader::ReaderObserver> observer) override;

                        /**
                         * Execute the transmission of a list of {@link SeRequest} and returns a list of
                         * {@link SeResponse}
                         *
                         * @param requestSet the request set
                         * @return responseSet the response set
                         * @throws KeypleReaderException if a reader error occurs
                         */
                        std::shared_ptr<SeResponseSet> transmitSet(
                            std::shared_ptr<SeRequestSet> requestSet) throw(KeypleReaderException) override;

                        /**
                         * Execute the transmission of a {@link SeRequest} and returns a {@link SeResponse}
                         *
                         * @param seRequest the request to be transmitted
                         * @return the received response
                         * @throws KeypleReaderException if a reader error occurs
                         */
                        std::shared_ptr<SeResponse>
                        transmit(std::shared_ptr<SeRequest> seRequest) throw(KeypleReaderException) override;

                        /**
                         * Compare the name of the current SeReader to the name of the SeReader provided in argument
                         *
                         * @param seReader a SeReader object
                         * @return true if the names match (The method is needed for the SortedSet lists)
                         */
                        int compareTo(std::shared_ptr<SeReader> seReader);

                        /**
                         *
                         */
                        std::string getName() override;

                      protected:
                        /**
                         *
                         */
                        const std::shared_ptr<Logger> logger =
                            LoggerFactory::getLogger(typeid(AbstractObservableReader));

                        /**
                         * Timestamp recorder
                         */
                        long long before = 0;

                        /**
                         *
                         */
                        const std::string pluginName;

                        /**
                         * The default SelectionRequest to be executed upon SE insertion
                         */
                        std::shared_ptr<SelectionRequest> defaultSelectionRequest;

                        /** 
                         * Indicate if all SE detected should be notified or only matching SE
                         */
                        ObservableReader::NotificationMode notificationMode =
                            event::ObservableReader::NotificationMode::ALWAYS;

                        /**
                         *
                         */
                        virtual std::shared_ptr<SeResponseSet>
                        processSeRequestSet(std::shared_ptr<SeRequestSet> requestSet) = 0;

                        /**
                         *
                         */
                        virtual std::shared_ptr<SeResponse>
                        processSeRequest(std::shared_ptr<SeRequest> seRequest) = 0;

                        /**
                         * Reader constructor
                         *
                         * Force the definition of a name through the use of super method.
                         *
                         * @param pluginName the name of the plugin that instantiated the reader
                         * @param readerName the name of the reader
                         */
                        AbstractObservableReader(const std::string &pluginName,
                                                 const std::string &readerName);

                        /**
                         * Starts the monitoring thread
                         * <p>
                         * This method has to be overloaded by the class that handle the monitoring thread. It will be
                         * called when a first observer is added.
                         */
                        virtual void startObservation();

                        /**
                         * Ends the monitoring thread
                         * <p>
                         * This method has to be overloaded by the class that handle the monitoring thread. It will be
                         * called when the observer is removed.
                         */
                        virtual void stopObservation();

                        /**
                         * @return Plugin name
                         */
                        std::string getPluginName();

                        /**
                         *
                         */
                        std::shared_ptr<AbstractObservableReader> shared_from_this()
                        {
                            return std::static_pointer_cast<AbstractObservableReader>(
                                AbstractLoggedObservable<ReaderEvent>::shared_from_this());
                        }
                    };

                } // namespace plugin
            }     // namespace seproxy
        }         // namespace keyple
    }             // namespace eclipse
} // namespace org
