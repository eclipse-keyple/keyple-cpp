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
#include "DefaultSelectionRequest.h"

/* Namespace */
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {

                    /* Using */
                    using SeReader                    = org::eclipse::keyple::seproxy::SeReader;
                    using ObservableReader            = org::eclipse::keyple::seproxy::event::ObservableReader;
                    using ReaderEvent                 = org::eclipse::keyple::seproxy::event::ReaderEvent;
                    using KeypleChannelStateException = org::eclipse::keyple::seproxy::exception::KeypleChannelStateException;
                    using KeypleIOReaderException     = org::eclipse::keyple::seproxy::exception::KeypleIOReaderException;
                    using KeypleReaderException       = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using ProxyReader                 = org::eclipse::keyple::seproxy::message::ProxyReader;
                    using SeRequest                   = org::eclipse::keyple::seproxy::message::SeRequest;
                    using SeRequestSet                = org::eclipse::keyple::seproxy::message::SeRequestSet;
                    using SeResponse                  = org::eclipse::keyple::seproxy::message::SeResponse;
                    using SeResponseSet               = org::eclipse::keyple::seproxy::message::SeResponseSet;
                    using LoggerFactory               = org::eclipse::keyple::common::LoggerFactory;
                    using Logger                      = org::eclipse::keyple::common::Logger;
                    using DefaultSelectionRequest     = org::eclipse::keyple::seproxy::event::DefaultSelectionRequest;

                    /**
                     * Abstract definition of an observable reader.
                     * <ul>
                     * <li>High level logging and benchmarking of SeRequestSet and SeRequest transmission</li>
                     * <li>Observability management</li>
                     * <li>Name-based comparison of ProxyReader (required for SortedSet&lt;ProxyReader&gt;)</li>
                     * <li>Plugin naming management</li>
                     * </ul>
                     */

                    class EXPORT AbstractObservableReader : public AbstractLoggedObservable<ReaderEvent>, public ObservableReader, public ProxyReader {

                        /** logger */
                    private:
                        const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(AbstractObservableReader));

                        /** Timestamp recorder */
                        long long before = 0;

                        /** Contains the name of the plugin */
                    protected:
                        const std::string pluginName;

                        /** The default DefaultSelectionRequest to be executed upon SE insertion */
                        std::shared_ptr<DefaultSelectionRequest> defaultSelectionRequest;

                        /** Indicate if all SE detected should be notified or only matching SE */
                        ObservableReader::NotificationMode notificationMode;

                        /** ==== Constructor =================================================== */

                        /**
                         * Reader constructor
                         * <p>
                         * Force the definition of a name through the use of super method.
                         * <p>
                         * Initialize the time measurement
                         *
                         * @param pluginName the name of the plugin that instantiated the reader
                         * @param readerName the name of the reader
                         */
                        AbstractObservableReader(const std::string &pluginName, const std::string &readerName);

                        /** ==== Utility methods =============================================== */

                        /**
                         * Gets the name of plugin provided in the constructor.
                         * <p>
                         * The method will be used particularly for logging purposes. The plugin name is also part of
                         * the ReaderEvent and PluginEvent objects.
                         *
                         * @return the plugin name String
                         */
                        std::string getPluginName();

                        /**
                         * Compare the name of the current SeReader to the name of the SeReader provided in argument
                         *
                         * @param seReader a SeReader object
                         * @return true if the names match (The method is needed for the SortedSet lists)
                         */
                    public:
                        int compareTo(std::shared_ptr<SeReader> seReader);

                        /** ==== High level communication API ================================== */

                        /**
                         * Execute the transmission of a list of {@link SeRequest} and returns a list of
                         * {@link SeResponse}
                         * <p>
                         * The global execution time (inter-exchange and communication) and the SeRequestSet content is
                         * logged (DEBUG level).
                         * <p>
                         * As the method is final, it cannot be extended.
                         *
                         * @param requestSet the request set
                         * @return responseSet the response set
                         * @throws KeypleReaderException if a reader error occurs
                         */
                        std::shared_ptr<SeResponseSet> transmitSet(std::shared_ptr<SeRequestSet> requestSet) override;

                        /**
                         * Abstract method implemented by the AbstractLocalReader and VirtualReader classes.
                         * <p>
                         * This method is handled by transmitSet.
                         *
                         * @param requestSet the SeRequestSet to be processed
                         * @return the SeResponseSet (responses to the SeRequestSet)
                         * @throws KeypleReaderException if reader error occurs
                         */
                    protected:
                        virtual std::shared_ptr<SeResponseSet> processSeRequestSet(std::shared_ptr<SeRequestSet> requestSet) = 0;

                        /**
                         * Execute the transmission of a {@link SeRequest} and returns a {@link SeResponse}
                         * <p>
                         * The individual execution time (inter-exchange and communication) and the SeRequestSet content
                         * is logged (DEBUG level).
                         * <p>
                         * As the method is final, it cannot be extended.
                         *
                         * @param seRequest the request to be transmitted
                         * @return the received response
                         * @throws KeypleReaderException if a reader error occurs
                         */
                    public:
                        std::shared_ptr<SeResponse> transmit(std::shared_ptr<SeRequest> seRequest) override;

                        /**
                         * Abstract method implemented by the AbstractLocalReader and VirtualReader classes.
                         * <p>
                         * This method is handled by transmit.
                         *
                         * @param seRequest the SeRequestSet to be processed
                         * @return the SeResponse (responses to the SeRequest)
                         * @throws KeypleReaderException if reader error occurs
                         */
                    protected:
                        virtual std::shared_ptr<SeResponse> processSeRequest(std::shared_ptr<SeRequest> seRequest) = 0;

                        /** ==== Methods specific to observability ============================= */

                        /**
                         * Starts the monitoring of the reader activity (especially card insertion and removal)
                         * <p>
                         * This abstract method has to be implemented by the class that handle the monitoring thread (
                         * e.g. {@link AbstractThreadedLocalReader}).
                         * <p>
                         * It will be called when a first observer is added (see addObserver).
                         */
                        virtual void startObservation() = 0;

                        /**
                         * Ends the monitoring of the reader activity
                         * <p>
                         * This abstract method has to be implemented by the class that handle the monitoring thread
                         * (e.g. {@link AbstractThreadedLocalReader}). It will be called when the observer is removed.
                         * <p>
                         * It will be called when the last observer is removed (see removeObserver).
                         *
                         */
                        virtual void stopObservation() = 0;

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
                    public:
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
                        void removeObserver(std::shared_ptr<ObservableReader::ReaderObserver> observer) override;

                    protected:
                        std::shared_ptr<AbstractObservableReader> shared_from_this() {
                            return std::static_pointer_cast<AbstractObservableReader>(AbstractLoggedObservable<ReaderEvent>::shared_from_this());
                        }
                    };

                } // namespace plugin
            }     // namespace seproxy
        }         // namespace keyple
    }             // namespace eclipse
} // namespace org
