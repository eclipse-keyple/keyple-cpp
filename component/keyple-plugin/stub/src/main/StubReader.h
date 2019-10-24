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
#include <unordered_map>
#include <vector>
#include <memory>

/* Common */
#include "Export.h"
#include "exceptionhelper.h"

/* Core */
#include "AbstractThreadedLocalReader.h"
#include "TransmissionMode.h"
#include "KeypleBaseException.h"
#include "KeypleChannelStateException.h"
#include "NoStackTraceThrowable.h"

/* Forward declarations */
namespace keyple { namespace plugin { namespace stub { class StubSecureElement; } } }

namespace keyple {
namespace plugin {
namespace stub {

using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::plugin;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::seproxy::event;
using namespace keyple::common;

class EXPORT StubReader final : public AbstractThreadedLocalReader {
public:
    /**
     *
     */
    static const std::string ALLOWED_PARAMETER_1;
    static const std::string ALLOWED_PARAMETER_2;
    static const std::string CONTACTLESS_PARAMETER;
    static const std::string CONTACTS_PARAMETER;

    /**
     *
     */
    static const std::string pluginName;

    /**
     *
     */
    std::string readerName = "StubReader";

    /**
     *
     */
    TransmissionMode transmissionMode = TransmissionMode::CONTACTLESS;

    /**
     *
     */
    StubReader(const std::string &name);

    /**
     *
     */
    ~StubReader();

    /**
     *
     */
    void closePhysicalChannel() override;

    /**
     *
     */
    std::vector<char> transmitApdu(std::vector<char> &apduIn) override;

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
    bool equals(std::shared_ptr<void> o) override;

    /**
     *
     */
    int hashCode() override;

    /**
     *
     */
    void setParameters(std::unordered_map<std::string, std::string> &parameters) override;

    /**
     *
     */
    std::string getName() override
    {
        return AbstractThreadedLocalReader::AbstractLoggedObservable<ReaderEvent>::getName();
    }

protected:
    /**
     *
     */
    std::vector<char> getATR() override;

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
    bool protocolFlagMatches(const SeProtocol& protocolFlag)  override;

    /**
     *
     */
    bool checkSePresence() override;

    /**
     *
     */
    void setParameter(const std::string &name, const std::string &value)  override;

    /**
     *
     */
    std::unordered_map<std::string, std::string> getParameters() override;

    /**
     * @return the current transmission mode
     */
    TransmissionMode getTransmissionMode() override;

    /**
     * HELPERS TO TEST INTERNAL METHOD TODO : is this necessary?
     */
    std::shared_ptr<ApduResponse> processApduRequestTestProxy(std::shared_ptr<ApduRequest> apduRequest);

    /**
     *
     */
    std::shared_ptr<SeResponseSet> processSeRequestSetTestProxy(std::shared_ptr<SeRequestSet> requestSet);

    /**
     *
     */
    bool isLogicalChannelOpenTestProxy();

    /**
     * This method is called by the monitoring thread to check SE presence
     *
     * @param timeout the delay in millisecond we wait for a card insertion
     * @return true if the SE is present
     * @throws NoStackTraceThrowable in case of unplugging the reader
     */
    bool waitForCardPresent(long long timeout) override;

    /**
     * This method is called by the monitoring thread to check SE absence
     *
     * @param timeout the delay in millisecond we wait for a card withdrawing
     * @return true if the SE is absent
     * @throws NoStackTraceThrowable in case of unplugging the reader
     */
    bool waitForCardAbsent(long long timeout) override;

    /**
     *
     */
    std::shared_ptr<StubReader> shared_from_this()
    {
        return std::static_pointer_cast<StubReader>(AbstractThreadedLocalReader::shared_from_this());
    }

private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(StubReader));

    /**
     *
     */
    std::shared_ptr<StubSecureElement> se;

    /**
     *
     */
    bool sePresent = false;

    /**
     *
     */
    std::unordered_map<std::string, std::string> parameters =
        std::unordered_map<std::string, std::string>();

};

}
}
}
