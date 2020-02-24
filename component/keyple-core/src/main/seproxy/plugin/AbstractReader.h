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
#include "Configurable.h"
#include "Export.h"
#include "Logger.h"
#include "Nameable.h"
#include "Observable.h"

/* Core */
#include "DefaultSelectionsRequest.h"
#include "KeypleChannelControlException.h"
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
using namespace keyple::core::util;
using namespace keyple::common;

using ReaderObserver = ObservableReader::ReaderObserver;

/**
 * Abstract definition of an observable reader.
 * <ul>
 * <li>High level logging and benchmarking of SeRequestSet and SeRequest
 * transmission</li>
 * <li>Observability management</li>
 * <li>Name-based comparison of ProxyReader (required for SortedSet&lt;
 * ProxyReader&gt;)</li>
 * <li>Plugin naming management</li>
 * </ul>
 */
class EXPORT AbstractReader : public Observable<ReaderEvent>,
                              public ProxyReader,
                              public virtual Nameable,
                              public virtual Configurable {
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
     * The {@link MultiSeRequestProcessing} parameter indicates whether all
     * requests are to be sent regardless of their result (PROCESS_ALL) or
     * whether the process should stop at the first request whose result is a
     * success (FIRST_MATCH).
     * <p>
     * The {@link ChannelControl} parameter specifies whether the physical
     * channel should be closed (CLOSE_AFTER) or not (KEEP_OPEN) after all
     * requests have been transmitted.
     * <p>
     * The global execution time (inter-exchange and communication) and the Set
     * of SeRequest content is logged (DEBUG level).
     * <p>
     * As the method is final, it cannot be extended.
     *
     * @param requestSet the request set
     * @param multiSeRequestProcessing the multi SE request processing mode
     * @param channelControl the channel control indicator
     * @return the response set
     * @throws KeypleReaderException if a reader error occurs
     */
    std::list<std::shared_ptr<SeResponse>>
    transmitSet(std::set<std::shared_ptr<SeRequest>>& requestSet,
                MultiSeRequestProcessing multiSeRequestProcessing,
                ChannelControl channelControl) override;

    /**
     * Simplified version of transmitSet for standard use.
     *
     * @param requestSet the request set
     * @return the response set
     * @throws KeypleReaderException if a reader error occurs
     */
    std::list<std::shared_ptr<SeResponse>>
    transmitSet(std::set<std::shared_ptr<SeRequest>>& requestSet) override;

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
    std::shared_ptr<SeResponse>
    transmit(std::shared_ptr<SeRequest> seRequest,
             ChannelControl channelControl) override;

    /**
     * Simplified version of transmit for standard use.
     *
     * @param seRequest the request to be transmitted
     * @return the received response
     * @throws KeypleReaderException if a reader error occurs
     */
    std::shared_ptr<SeResponse>
    transmit(std::shared_ptr<SeRequest> seRequest) override;

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
    void addObserver(std::shared_ptr<ReaderObserver> observer);

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
    void removeObserver(std::shared_ptr<ReaderObserver> observer);

    /**
     * Gets the name of plugin provided in the constructor.
     * <p>
     * The method will be used particularly for logging purposes. The plugin
     * name is also part of the ReaderEvent and PluginEvent objects.
     *
     * @return the plugin name String
     */
    const std::string& getPluginName();

    /**
     * Gets the reader name
     *
     * @return the reader name string
     */
    const std::string& getName() const override;

    /**
     * Allows the application to signal the end of processing and thus proceed
     * with the removal sequence, followed by a restart of the card search.
     * <p>
     * Do nothing if the closing of the physical channel has already been
     * requested.
     * <p>
     * Send a request without APDU just to close the physical channel if it has
     * not already been closed.
     *
     */
    void notifySeProcessed();

    /**
     *
     */
    void notifyObservers(std::shared_ptr<ReaderEvent> event) override;

    /**
     * Sets at once a set of parameters for a reader
     * <p>
     * See {@link #setParameter(String, String)} for more details
     *
     * @param parameters a Map &lt;String, String&gt; parameter set
     * @throws KeypleBaseException if one of the parameters could not be set up
     */
    void setParameters(
        const std::map<std::string, std::string>& parameters) override;

protected:
    /**
     *
     */
    std::shared_ptr<AbstractReader> shared_from_this()
    {
        return std::static_pointer_cast<AbstractReader>(
            Observable<ReaderEvent>::shared_from_this());
    }

    /**
     *  Contains the name of the plugin
     */
    const std::string pluginName;

    /**
     * The reader name (must be unique)
     */
    const std::string name;

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
    AbstractReader(const std::string& pluginName, const std::string& name);

    /**
     * Abstract method implemented by the AbstractLocalReader and VirtualReader
     * classes.
     * <p>
     * This method is handled by transmitSet.
     *
     * @param requestSet the Set of {@link SeRequest} to be processed
     * @param multiSeRequestProcessing the multi se processing mode
     * @param channelControl indicates if the channel has to be closed at the
     *        end of the processing
     * @return the List of {@link SeResponse} (responses to the Set of {@link
     *         SeRequest})
     * @throws KeypleReaderException if reader error occurs
     */
    virtual std::list<std::shared_ptr<SeResponse>>
    processSeRequestSet(std::set<std::shared_ptr<SeRequest>>& requestSet,
                        MultiSeRequestProcessing multiSeRequestProcessing,
                        ChannelControl channelControl) = 0;

    /**
     * Abstract method implemented by the AbstractLocalReader and VirtualReader
     * classes.
     * <p>
     * This method is handled by transmit.
     *
     * @param seRequest the {@link SeRequest} to be processed
     * @param channelControl a flag indicating if the channel has to be closed
     *        after the processing of the {@link SeRequest}
     * @return the {@link SeResponse} (responses to the {@link SeRequest})
     * @throws KeypleReaderException if reader error occurs
     */
    virtual std::shared_ptr<SeResponse>
    processSeRequest(std::shared_ptr<SeRequest> seRequest,
                     ChannelControl channelControl) = 0;

private:
    /**
     * Logger
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(AbstractReader));

    /**
     * Timestamp recorder
     */
    long long before = 0;

    /**
     * This flag is used with transmit or transmitSet
     * <p>
     * It will be used by the notifySeProcessed method to determine if a request
     * to close the physical channel has been already made and therefore to
     * switch directly to the removal sequence for the observed readers.
     */
    bool forceClosing = true;
};

}
}
}
}
