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
#include <unordered_map>
#include <vector>
#include <memory>


/* Common */
#include "exceptionhelper.h"
#include "Export.h"
#include "LoggerFactory.h"
#include "stringhelper.h"

/* Core */
#include "AbstractObservableLocalReader.h"
#include "MonitoringPool.h"
#include "ReaderEvent_Import.h"
#include "SmartInsertionReader.h"
#include "SmartRemovalReader.h"
#include "TransmissionMode.h"

/* Stub */
#include "StubReader.h"

/* Forward declarations */
namespace keyple { namespace plugin { namespace stub {
    class StubSecureElement; } } }

namespace keyple {
namespace plugin {
namespace stub {

using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::plugin;
using namespace keyple::core::seproxy::plugin::local;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::seproxy::event;
using namespace keyple::common;

class EXPORT StubReaderImpl
: public AbstractObservableLocalReader, public StubReader,
  public SmartInsertionReader, public SmartRemovalReader {
public:
    /**
     * Do not use directly
     *
     * @param readerName
     */
    StubReaderImpl(const std::string& pluginName,
                   const std::string& readerName);

    /**
     * Specify
     *
     * @param pluginName
     * @param name
     * @param transmissionMode
     */
    StubReaderImpl(const std::string& pluginName, const std::string& name,
                   TransmissionMode transmissionMode);

    /**
     *
     */
    void closePhysicalChannel() override;

    /**
     *
     */
    std::vector<uint8_t> transmitApdu(std::vector<uint8_t>& apduIn) override;

    /**
     *
     */
    void setParameter(const std::string& name, const std::string& value)
        override;

    /**
     *
     */
    const std::map<const std::string, const std::string> getParameters() override;

    /**
     * @return the current transmission mode
     */
    TransmissionMode getTransmissionMode() override;

    /**
     * STATE CONTROLLERS FOR INSERTING AND REMOVING SECURE ELEMENT
     */
    void insertSe(std::shared_ptr<StubSecureElement> _se);

    /**
     *
     */
    void removeSe();

    /**
     *
     */
    std::shared_ptr<StubSecureElement> getSe();

    /**
     * This method is called by the monitoring thread to check SE presence
     *
     * @return true if the SE is present
     */
    bool waitForCardPresent() override;

    /**
     *
     */
    void stopWaitForCard() override;

    /**
     * Defined in the {@link
     * keyple::core::seproxy::plugin::local::SmartRemovalReader} interface, this
     * method is called by the monitoring thread to check SE absence
     *
     * @return true if the SE is absent
     */
    bool waitForCardAbsentNative() override;

    /**
     *
     */
    void stopWaitForCardRemoval() override;

    /**
     *
     */
    bool checkSePresence() override;

protected:
    /**
     *
     */
    std::shared_ptr<MonitoringPool> executorService;

    /**
     *
     */
    const std::vector<uint8_t>& getATR() override;

    /**
     *
     */
    bool isPhysicalChannelOpen() override;

    /**
     *
     */
    void openPhysicalChannel() override;

    /**
     *
     */
    bool protocolFlagMatches(const SeProtocol& protocolFlag) override;

    /**
     *
     */
    std::shared_ptr<ObservableReaderStateService> initStateService() override;

private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(StubReaderImpl));

    /**
     *
     */
    std::shared_ptr<StubSecureElement> se;

    /**
     *
     */
    std::map<const std::string, const std::string> parameters;

    /**
     *
     */
    TransmissionMode transmissionMode = TransmissionMode::CONTACTLESS;

    /**
     *
     */
    std::atomic<bool> loopWaitSe;

    /**
     *
     */
    std::atomic<bool> loopWaitSeRemoval;

    /**
     * Add a reader observer.
     * <p>
     * The observer will receive all the events produced by this reader (card
     * insertion, removal, etc.)
     *
     * @param observer the observer object
     *
     * /!\/!\/!\
     *
     * Function addObserver() is present in two base classes (AbstractReader and
     * ObservableReader). AbstractReader implements the virtual function but
     * ObservableReader and its derived classes don't, therefore function
     * ObservableReader::addObserver() is considered virtual. Override needed in
     * this class.
     */
    void addObserver(std::shared_ptr<ReaderObserver> observer) override;

    /**
     * Remove a reader observer.
     * <p>
     * The observer will not receive any of the events produced by this reader.
     *
     * @param observer the observer object
     *
     * /!\/!\/!\
     *
     * Function removeObserver() is present in two base classes (AbstractReader
     * and ObservableReader). AbstractReader implements the virtual function
     * but ObservableReader and its derived classes don't, therefore function
     * ObservableReader::removeObserver() is considered virtual. Override needed
     * in this class.
     */
    void removeObserver(std::shared_ptr<ReaderObserver> observer) override;

    /**
     * Starts the SE detection. Once activated, the application can be notified
     * of the arrival of an SE.
     *
     * @param pollingMode indicates the action to be followed after processing
     *        the SE: if CONTINUE, the SE detection is restarted, if STOP, the
     *        SE detection is stopped until a new call to startSeDetection is
     *        made.
     *
     * /!\/!\/!\
     *
     * Function startSeDetection() is present in two base classes (
     * AbstractObservableLocalReader and ObservableReader).
     * AbstractObservableLocalReader implements the virtual function but
     * ObservableReader and its derived classes don't, therefore function
     * ObservableReader::startSeDetection() is considered virtual. Override
     * needed in this class.
     */
    void startSeDetection(PollingMode pollingMode) override;

    /**
     * Stops the SE detection.
     * <p>
     * This method must be overloaded by readers depending on the particularity
     * of their management of the start of SE detection.
     *
     * /!\/!\/!\
     *
     * Function removeObserver() is present in two base classes (
     * AbstractObservableLocalReader and ObservableReader).
     * AbstractObservableLocalReader implements the virtual function but
     * ObservableReader and its derived classes don't, therefore function
     * ObservableReader::removeObserver() is considered virtual. Override needed
     * in this class.
     */
    void stopSeDetection() override;

    /**
     * Defines the selection request to be processed when an SE is inserted.
     * Depending on the SE and the notificationMode parameter, a SE_INSERTED,
     * SE_MATCHED or no event at all will be notified to the application
     * observers.
     *
     * @param defaultSelectionsRequest the selection request to be operated
     * @param notificationMode indicates whether a SE_INSERTED event should be
     *        notified even if the selection has failed (ALWAYS) or whether the
     *        SE insertion should be ignored in this case (MATCHED_ONLY).
     *
     * /!\/!\/!\
     *
     * Function setDefaultSelectionRequest() is present in two base classes (
     * AbstractObservableLocalReader and ObservableReader).
     * AbstractObservableLocalReader implements the virtual function but
     * ObservableReader and its derived classes don't, therefore function
     * ObservableReader::setDefaultSelectionRequest() is considered virtual.
     * Override needed in this class.
     */
    void setDefaultSelectionRequest(
                std::shared_ptr<AbstractDefaultSelectionsRequest>
                    defaultSelectionsRequest,
                NotificationMode notificationMode) override;
};

}
}
}
