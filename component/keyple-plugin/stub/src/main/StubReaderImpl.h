/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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
#include "LoggerFactory.h"
#include "stringhelper.h"

/* Core */
#include "AbstractObservableLocalReader.h"
#include "MonitoringPool.h"
#include "ReaderEvent.h"
#include "SmartInsertionReader.h"
#include "SmartRemovalReader.h"
#include "TransmissionMode.h"

/* Stub */
#include "KeyplePluginStubExport.h"
#include "StubReader.h"

/* Forward declarations */
namespace keyple {
namespace plugin {
namespace stub {
class StubSecureElement;
}
}
}

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

class KEYPLEPLUGINSTUB_API StubReaderImpl final
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
    ~StubReaderImpl();

    /**
     *
     */
    void closePhysicalChannel() override;

    /**
     *
     */
    std::vector<uint8_t> transmitApdu(const std::vector<uint8_t>& apduIn)
        override;

    /**
     *
     */
    void setParameter(const std::string& name, const std::string& value)
        override;

    /**
     *
     */
    const std::map<const std::string, const std::string>& getParameters() const
        override;

    /**
     * @return the current transmission mode
     */
    const TransmissionMode& getTransmissionMode() const override;

    /**
     * STATE CONTROLLERS FOR INSERTING AND REMOVING SECURE ELEMENT
     */
    void insertSe(std::shared_ptr<StubSecureElement> _se) override;

    /**
     *
     */
    void removeSe() override;

    /**
     *
     */
    std::shared_ptr<StubSecureElement> getSe() override;

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
    std::shared_ptr<MonitoringPool> mExecutorService;

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
    bool protocolFlagMatches(const std::shared_ptr<SeProtocol> protocolFlag)
        override;

    /**
     *
     */
    std::shared_ptr<ObservableReaderStateService> initStateService() override;

private:
    /**
     *
     */
    const std::shared_ptr<Logger> mLogger =
        LoggerFactory::getLogger(typeid(StubReaderImpl));

    /**
     *
     */
    std::shared_ptr<StubSecureElement> mSe;

    /**
     *
     */
    std::map<const std::string, const std::string> mParameters;

    /**
     *
     */
    TransmissionMode mTransmissionMode = TransmissionMode::CONTACTLESS;

    /**
     *
     */
    std::atomic<bool> mLoopWaitSe;

    /**
     *
     */
    std::atomic<bool> mLoopWaitSeRemoval;
};

}
}
}
