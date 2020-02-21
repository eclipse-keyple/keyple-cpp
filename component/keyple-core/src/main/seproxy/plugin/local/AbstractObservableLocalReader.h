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

#include "AbstractDefaultSelectionsRequest.h"
#include "AbstractLocalReader.h"
#include "KeypleChannelControlException.h"
#include "KeypleIOReaderException.h"
#include "KeypleReaderException.h"
#include "DefaultSelectionsRequest.h"
#include "DefaultSelectionsResponse.h"
#include "ObservableReader_Import.h"
#include "ReaderEvent.h"
#include "SeResponse.h"

/* Common */
#include "Export.h"
#include "LoggerFactory.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {
namespace local {

class AbstractObservableState;
class ObservableReaderStateService;
enum class MonitoringState;

/**
 * Internal events
 */
enum class InternalEvent {
    /** A SE has been inserted */
    SE_INSERTED,
    /** The SE has been removed */
    SE_REMOVED,
    /** The application has completed the processing of the SE */
    SE_PROCESSED,
    /** The application has requested the start of SE detection */
    START_DETECT,
    /** The application has requested that SE detection is to be stopped. */
    STOP_DETECT,
    /** A timeout has occurred (not yet implemented) */
    TIME_OUT
};


/**
 * This abstract class is used to manage the matter of observing SE events in
 * the case of a local reader.
 *
 * <p>
 * It provides the means to configure the plugin's behavior when a SE is
 * detected.
 *
 * The event management implements a ObservableReaderStateService state machine
 * that is composed of four states.
 * <ol>
 * <li>WAIT_FOR_START_DETECTION
 * <p>
 * Infinitely waiting for a signal from the application to start SE detection by
 * changing to WAIT_FOR_SE_INSERTION state. This signal is given by calling the
 * setDefaultSelectionRequest method.
 * <p>
 * Note: The system always starts in the WAIT_FOR_START_DETECTION state.
 * <li>WAIT_FOR_SE_INSERTION
 * <p>
 * Awaiting the SE insertion. After insertion, the processSeInserted method is
 * called.
 * <p>
 * A number of cases arise:
 * <ul>
 * <li>A default selection is defined: in this case it is played and its result
 * leads to an event notification SE_INSERTED or SE_MATCHED or no event (see
 * setDefaultSelectionRequest)
 *
 * <li>There is no default selection: a SE_INSERTED event is then notified.
 * <p>
 * In the case where an event has been notified to the application, the state
 * machine changes to the WAIT_FOR_SE_PROCESSING state otherwise it remains in
 * the WAIT_FOR_SE_INSERTION state.
 * </ul>
 * <p>
 * The notification consists in calling the "update" methods of the defined
 * observers. In the case where several observers have been defined, it is up to
 * the application developer to ensure that there is no long processing in these
 * methods, by making their execution asynchronous for example.
 * <li>WAIT_FOR_SE_PROCESSING
 * <p>
 * Waiting for the end of processing by the application. The end signal is
 * triggered either by a transmission made with a CLOSE_AFTER parameter, or by
 * an explicit call to the notifySeProcessed method (if the latter is called
 * when a "CLOSE" transmission has already been made, it will do nothing,
 * otherwise it will make a pseudo transmission intended only for closing
 * channels).
 * <p>
 * If the instruction given when defining the default selection request is to
 * stop (ObservableReader.PollingMode.SINGLESHOT) then the logical and physical
 * channels are closed immediately and the machine state changes to
 * WAIT_FOR_START_DETECTION state.
 * <p>
 * If the instruction given is continue (ObservableReader.PollingMode.REPEATING)
 * then the state machine changes to WAIT_FOR_SE_REMOVAL.
 * <li>WAIT_FOR_SE_REMOVAL:
 * <p>
 * Waiting for the SE to be removed. When the SE is removed, a SE_REMOVED event
 * is notified to the application and the state machine changes to the
 * WAIT_FOR_SE_INSERTION or WAIT_FOR_START_DETECTION state according the polling
 * mode (ObservableReader.PollingMode).
 * </ol>
 */
class EXPORT AbstractObservableLocalReader : public AbstractLocalReader {
public:
    /**
     * Reader constructor
     * <p>
     * Force the definition of a name through the use of super method.
     * <p>
     *
     * @param pluginName the name of the plugin that instantiated the reader
     * @param readerName the name of the reader
     */
    AbstractObservableLocalReader(const std::string& pluginName,
                                  const std::string& readerName);


    /**
     * Check the presence of a SE
     * <p>
     * This method is recommended for non-observable readers.
     * <p>
     * When the SE is not present the logical and physical channels status may
     * be refreshed through a call to the processSeRemoved method.
     *
     * @return true if the SE is present
     */
    bool isSePresent() override;

    /**
     * Starts the SE detection. Once activated, the application can be notified
     * of the arrival of an
     * SE.
     * <p>
     * This method must be overloaded by readers depending on the particularity
     * of their management of the start of SE detection.
     * <p>
     * Note: they must call the super method with the argument PollingMode.
     *
     * @param pollingMode indicates the action to be followed after processing
     *        the SE: if REPEATING, the SE detection is restarted, if
     *        SINGLESHOT, the SE detection is stopped until a new call to
     *        startSeDetection is made.
     */
    void startSeDetection(const ObservableReader::PollingMode pollingMode);

    /**
     * Stops the SE detection.
     * <p>
     * This method must be overloaded by readers depending on the particularity
     * of their management of the start of SE detection.
     */
    void stopSeDetection();

    /**
     * If defined, the prepared DefaultSelectionRequest will be processed as
     * soon as a SE is inserted. The result of this request set will be added to
     * the reader event notified to the application.
     * <p>
     * If it is not defined (set to null), a simple SE detection will be
     * notified in the end.
     * <p>
     * Depending on the notification mode, the observer will be notified
     * whenever an SE is inserted, regardless of the selection status, or only
     * if the current SE matches the selection criteria.
     * <p>
     *
     * @param defaultSelectionsRequest the {@link
     *        AbstractDefaultSelectionsRequest} to be executed when a SE is
     *        inserted
     * @param notificationMode the notification mode enum (ALWAYS or
     *        MATCHED_ONLY)
     */
    void setDefaultSelectionRequest(
             std::shared_ptr<AbstractDefaultSelectionsRequest>
                 defaultSelectionsRequest,
             const ObservableReader::NotificationMode notificationMode);

    /**
     * A combination of defining the default selection request and starting the
     * SE detection.
     *
     * @param defaultSelectionsRequest the selection request to be operated
     * @param notificationMode indicates whether a SE_INSERTED event should be
     *        notified even if the selection has failed (ALWAYS) or whether the
     *        SE insertion should be ignored in this case (MATCHED_ONLY).
     * @param pollingMode indicates the action to be followed after processing
     *        the SE: if CONTINUE, the SE detection is restarted, if STOP, the
     *        SE detection is stopped until a new call to startSeDetection is
     *        made.
     */
    void setDefaultSelectionRequest(
            std::shared_ptr<AbstractDefaultSelectionsRequest>
                defaultSelectionsRequest,
            const ObservableReader::NotificationMode notificationMode,
            const ObservableReader::PollingMode pollingMode);

    /**
     * This method is invoked when a SE is inserted in the case of an observable
     * reader.
     * <p>
     * e.g. from the monitoring thread in the case of a Pcsc plugin or from the
     * NfcAdapter callback method onTagDiscovered in the case of a Android NFC
     * plugin.
     * <p>
     * It will return a ReaderEvent in the following cases:
     * <ul>
     * <li>SE_INSERTED: if no default selection request was defined
     * <li>SE_MATCHED: if a default selection request was defined in any mode
     * and a SE matched the selection
     * <li>SE_INSERTED: if a default selection request was defined in ALWAYS
     * mode but no SE matched the selection (the DefaultSelectionsResponse is
     * however transmitted)
     * </ul>
     * <p>
     * It returns null if a default selection is defined in MATCHED_ONLY mode
     * but no SE matched the selection.
     *
     * @return ReaderEvent that should be notified to observers, contains the
     *         results of the default selection if any, can be null if no event
     *         should be sent
     */
    std::shared_ptr<ReaderEvent> processSeInserted();

    /**
     * Sends a neutral APDU to the SE to check its presence. The status of the
     * response is not verified as long as the mere fact that the SE responds is
     * sufficient to indicate whether or not it is present.
     * <p>
     * This method has to be called regularly until the SE no longer respond.
     * <p>
     * Having this method not final allows a reader plugin to implement its own
     * method.
     *
     * @return true if the SE still responds, false if not
     */
    bool isSePresentPing();

    /**
     * This method is invoked when a SE is removed in the case of an observable
     * reader.
     * <p>
     * It will also be invoked if isSePresent is called and at least one of the
     * physical or logical channels is still open (case of a non-observable
     * reader)
     * <p>
     * The SE will be notified removed only if it has been previously notified
     * present (observable reader only)
     *
     */
    void processSeRemoved();

    /**
     * Get polling mode
     *
     * @return the current polling mode
     */
    ObservableReader::PollingMode getPollingMode();

    /**
     * Get the current monitoring state
     *
     * @return current getMonitoringState
     */
    MonitoringState getCurrentMonitoringState();

    /**
     * thread safe method to communicate an internal event to this reader Use
     * this method to inform the reader of external event like a tag discovered
     * or a Se inserted
     *
     * @param event internal event
     */
    void onEvent(const InternalEvent event);

    /**
     * This method initiates the SE removal sequence.
     * <p>
     * The reader will remain in the WAIT_FOR_SE_REMOVAL state as long as the SE
     * is present. It will change to the WAIT_FOR_START_DETECTION or
     * WAIT_FOR_SE_INSERTION state depending on what was set when the detection
     * was started.
     */
    void startRemovalSequence();

    /**
     * Changes the state of the state machine
     *
     * @param stateId : new stateId
     */
    void switchState(const MonitoringState stateId);

protected:
    /**
     * Service that handles Internal Events and their impact on the current
     * state of the reader
     */
    std::shared_ptr<ObservableReaderStateService> stateService;

    /**
     * Initialize the ObservableReaderStateService with the possible states and
     * their implementation. ObservableReaderStateService define the initial
     * state.
     * <p>
     * Make sure to initialize the stateService in your reader constructor with
     * stateService = initStateService()
     * <p>
     *
     * @return initialized state stateService with possible states and the init
     * state
     */
    virtual std::shared_ptr<ObservableReaderStateService> initStateService()
        = 0;

private:
    /**
     * Logger
    */
    const std::shared_ptr<Logger> logger =
              LoggerFactory::getLogger(typeid(AbstractObservableLocalReader));


    /**
     * The default DefaultSelectionsRequest to be executed upon SE insertion
     */
    std::shared_ptr<DefaultSelectionsRequest> defaultSelectionsRequest;

    /**
     * Indicate if all SE detected should be notified or only matching SE
     */
    ObservableReader::NotificationMode notificationMode =
        ObservableReader::NotificationMode::ALWAYS;

    /**
     *
     */
    ObservableReader::PollingMode currentPollingMode =
        ObservableReader::PollingMode::SINGLESHOT;

};

}
}
}
}
}
