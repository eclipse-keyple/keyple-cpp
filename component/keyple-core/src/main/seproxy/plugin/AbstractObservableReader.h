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

#pragma once

#include <string>
#include <stdexcept>
#include <memory>

/* Common */
#include "Export.h"
#include "Logger.h"

/* Core */
#include "AbstractLoggedObservable.h"
#include "DefaultSelectionsRequest.h"
#include "KeypleChannelStateException.h"
#include "KeypleIOReaderException.h"
#include "KeypleReaderException.h"
#include "ObservableReader_Import.h"
#include "ProxyReader.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::common;

/**
 * Abstract definition of an observable reader.
 * <ul>
 * <li>High level logging and benchmarking of SeRequestSet and SeRequest transmission</li>
 * <li>Observability management</li>
 * <li>Name-based comparison of ProxyReader (required for SortedSet&lt;ProxyReader&gt;)</li>
 * <li>Plugin naming management</li>
 * </ul>
 */
class EXPORT AbstractObservableReader
: public AbstractLoggedObservable<ReaderEvent>, public ObservableReader,
  public ProxyReader {
public:
    /**
     * Compare the name of the current SeReader to the name of the SeReader
     * provided in argument
     *
     * @param seReader a SeReader object
     * @return true if the names match (The method is needed for the SortedSet
     *         lists)
     */
    int compareTo(std::shared_ptr<SeReader> seReader);

    /**
     * Execute the transmission of a list of {@link SeRequest} and returns a
     * list of {@link SeResponse}
     * <p>
     * The global execution time (inter-exchange and communication) and the
     * SeRequestSet content is logged (DEBUG level).
     * <p>
     * As the method is final, it cannot be extended.
     *
     * @param requestSet the request set
     * @return responseSet the response set
     * @throws KeypleReaderException if a reader error occurs
     */
    std::shared_ptr<SeResponseSet> transmitSet(std::shared_ptr<SeRequestSet>
        requestSet) override;

    /**
     * Execute the transmission of a {@link SeRequest} and returns a
     * {@link SeResponse}
     * <p>
     * The individual execution time (inter-exchange and communication) and the
     * SeRequestSet content is logged (DEBUG level).
     * <p>
     * As the method is final, it cannot be extended.
     *
     * @param seRequest the request to be transmitted
     * @return the received response
     * @throws KeypleReaderException if a reader error occurs
     */
    std::shared_ptr<SeResponse> transmit(std::shared_ptr<SeRequest> seRequest)
        override;

    /**
     * Add a reader observer.
     * <p>
     * The observer will receive all the events produced by this reader (card
     * insertion, removal, etc.)
     * <p>
     * The startObservation() is called when the first observer is added. (to
     * start a monitoring thread for instance)
     *
     * @param observer the observer object
     */
    void addObserver(std::shared_ptr<ObservableReader::ReaderObserver> observer)
        override;

    /**
     * Remove a reader observer.
     * <p>
     * The observer will not receive any of the events produced by this reader.
     * <p>
     * The stopObservation() is called when the last observer is removed. (to
     * stop a monitoring thread for instance)
     *
     * @param observer the observer object
     */
    void removeObserver(std::shared_ptr<ObservableReader::ReaderObserver>
        observer) override;

protected:
    /**
     *
     */
    std::shared_ptr<AbstractObservableReader> shared_from_this()
    {
        return std::static_pointer_cast<AbstractObservableReader>(
                   AbstractLoggedObservable<ReaderEvent>::shared_from_this());
    }

    /**
     *  Contains the name of the plugin
     */
    const std::string pluginName;

    /**
     * The default DefaultSelectionsRequest to be executed upon SE insertion
     */
    std::shared_ptr<DefaultSelectionsRequest> defaultSelectionsRequest;

    /**
     * Indicate if all SE detected should be notified or only matching SE
     */
    ObservableReader::NotificationMode notificationMode;

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
    AbstractObservableReader(const std::string &pluginName,
                             const std::string &readerName);

    /**
     * Gets the name of plugin provided in the constructor.
     * <p>
     * The method will be used particularly for logging purposes. The plugin
     * name is also part of the ReaderEvent and PluginEvent objects.
     *
     * @return the plugin name String
     */
    std::string getPluginName();

    /**
     * Abstract method implemented by the AbstractLocalReader and VirtualReader
     * classes.
     * <p>
     * This method is handled by transmitSet.
     *
     * @param requestSet the SeRequestSet to be processed
     * @return the SeResponseSet (responses to the SeRequestSet)
     * @throws KeypleReaderException if reader error occurs
     */
    virtual std::shared_ptr<SeResponseSet> processSeRequestSet(
        std::shared_ptr<SeRequestSet> requestSet) = 0;

    /**
     * Abstract method implemented by the AbstractLocalReader and VirtualReader
     * classes.
     * <p>
     * This method is handled by transmit.
     *
     * @param seRequest the SeRequestSet to be processed
     * @return the SeResponse (responses to the SeRequest)
     * @throws KeypleReaderException if reader error occurs
     */
    virtual std::shared_ptr<SeResponse>processSeRequest(
        std::shared_ptr<SeRequest> seRequest) = 0;

    /**
     * Starts the monitoring of the reader activity (especially card insertion
     * and removal)
     * <p>
     * This abstract method has to be implemented by the class that handle the
     * monitoring thread ( e.g. {@link AbstractThreadedLocalReader}).
     * <p>
     * It will be called when a first observer is added (see addObserver).
     */
    virtual void startObservation() = 0;

    /**
     * Ends the monitoring of the reader activity
     * <p>
     * This abstract method has to be implemented by the class that handle the
     * monitoring thread (e.g. {@link AbstractThreadedLocalReader}). It will be
     * called when the observer is removed.
     * <p>
     * It will be called when the last observer is removed (see removeObserver).
     *
     */
    virtual void stopObservation() = 0;

private:
    /**
     * Logger
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(AbstractObservableReader));

    /**
     * Timestamp recorder
     */
    long long before = 0;



};

}
}
}
}
