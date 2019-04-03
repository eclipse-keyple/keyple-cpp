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
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace stub { class StubSecureElement; }}}}}

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace stub {

                    using KeypleChannelStateException = org::eclipse::keyple::seproxy::exception::KeypleChannelStateException;
                    using KeypleIOReaderException     = org::eclipse::keyple::seproxy::exception::KeypleIOReaderException;
                    using KeypleReaderException       = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using KeypleBaseException       = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                    using NoStackTraceThrowable       = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
                    using ApduRequest                 = org::eclipse::keyple::seproxy::message::ApduRequest;
                    using ApduResponse                = org::eclipse::keyple::seproxy::message::ApduResponse;
                    using SeRequestSet                = org::eclipse::keyple::seproxy::message::SeRequestSet;
                    using SeResponseSet               = org::eclipse::keyple::seproxy::message::SeResponseSet;
                    using AbstractThreadedLocalReader = org::eclipse::keyple::seproxy::plugin::AbstractThreadedLocalReader;
                    using SeProtocol                  = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                    using TransmissionMode            = org::eclipse::keyple::seproxy::protocol::TransmissionMode;
                    using ReaderEvent                 = org::eclipse::keyple::seproxy::event::ReaderEvent;
                    using Logger                      = org::eclipse::keyple::common::Logger;
                    using LoggerFactory               = org::eclipse::keyple::common::LoggerFactory;

                    class EXPORT StubReader final : public AbstractThreadedLocalReader {

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

                      public:
                        static const std::string ALLOWED_PARAMETER_1;
                        static const std::string ALLOWED_PARAMETER_2;
                        static const std::string CONTACTLESS_PARAMETER;
                        static const std::string CONTACTS_PARAMETER;

                        static const std::string pluginName;
                        std::string readerName = "StubReader";

                        TransmissionMode transmissionMode = TransmissionMode::CONTACTLESS;

                        StubReader(const std::string &name);

                        ~StubReader();

                      protected:
                        std::vector<char> getATR() override;

                        bool isPhysicalChannelOpen() override;

                        void openPhysicalChannel() throw(KeypleChannelStateException) override;

                      public:
                        void closePhysicalChannel() throw(KeypleChannelStateException) override;

                        std::vector<char> transmitApdu(std::vector<char> &apduIn) throw(KeypleIOReaderException) override;

                      protected:
                        bool protocolFlagMatches(std::shared_ptr<SeProtocol> protocolFlag) throw(KeypleReaderException) override;

                      
                         bool checkSePresence() override;

                      public:
                        void setParameter(const std::string &name,
                                          const std::string &value) throw(KeypleReaderException) override;

                        std::unordered_map<std::string, std::string> getParameters() override;

                        /**
                         * @return the current transmission mode
                         */
                        TransmissionMode getTransmissionMode() override;

                        /*
                         * HELPERS TO TEST INTERNAL METHOD TODO : is this necessary?
                         */
                        std::shared_ptr<ApduResponse> processApduRequestTestProxy(std::shared_ptr<ApduRequest> apduRequest) throw(KeypleReaderException);

                        std::shared_ptr<SeResponseSet> processSeRequestSetTestProxy(std::shared_ptr<SeRequestSet> requestSet) throw(KeypleReaderException);

                        bool isLogicalChannelOpenTestProxy();



                        /*
                         * STATE CONTROLLERS FOR INSERTING AND REMOVING SECURE ELEMENT
                         */
                        void insertSe(std::shared_ptr<StubSecureElement> _se);

                        void removeSe();

                        /**
                         * This method is called by the monitoring thread to check SE presence
                         * 
                         * @param timeout the delay in millisecond we wait for a card insertion
                         * @return true if the SE is present
                         * @throws NoStackTraceThrowable in case of unplugging the reader
                         */
                      protected:
                        bool waitForCardPresent(long long timeout) throw(NoStackTraceThrowable) override;

                        /**
                         * This method is called by the monitoring thread to check SE absence
                         * 
                         * @param timeout the delay in millisecond we wait for a card withdrawing
                         * @return true if the SE is absent
                         * @throws NoStackTraceThrowable in case of unplugging the reader
                         */
                        bool waitForCardAbsent(long long timeout) throw(NoStackTraceThrowable) override;

                      protected:
                        std::shared_ptr<StubReader> shared_from_this() {
                            return std::static_pointer_cast<StubReader>(AbstractThreadedLocalReader::shared_from_this());
                        }

                      public:
                        /**
                         *
                         */
                        void notifyObservers(std::shared_ptr<ReaderEvent> event) override;

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
                        void setParameters(std::unordered_map<std::string, std::string> &parameters) throw(std::invalid_argument, KeypleBaseException) override;

                        std::string getName() override
                        {
                            return AbstractThreadedLocalReader::AbstractLoggedObservable<std::shared_ptr<ReaderEvent>>::getName();
                        }
                    };

                } // namespace stub
            }     // namespace plugin
        }         // namespace keyple
    }             // namespace eclipse
} // namespace org
