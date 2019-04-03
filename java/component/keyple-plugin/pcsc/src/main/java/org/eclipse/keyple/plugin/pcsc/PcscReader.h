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

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "exceptionhelper.h"
#include "stringhelper.h"

/* Common */
#include "Export.h"
#include "Logger.h"
#include "LoggerFactory.h"

/* Core */
#include "AbstractThreadedLocalReader.h"
#include "ReaderEvent_Import.h"
#include "TransmissionMode.h"

/* Smartcard I/O */
#include "CardTerminal.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace pcsc {

                    using namespace org::eclipse::keyple::seproxy::exception;
                    using AbstractThreadedLocalReader = org::eclipse::keyple::seproxy::plugin::AbstractThreadedLocalReader;
                    using SeProtocol                  = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                    using TransmissionMode            = org::eclipse::keyple::seproxy::protocol::TransmissionMode;
                    using ReaderEvent                 = org::eclipse::keyple::seproxy::event::ReaderEvent;
                    using LoggerFactory               = org::eclipse::keyple::common::LoggerFactory;
                    using Logger                      = org::eclipse::keyple::common::Logger;

                    class EXPORT PcscReader : public AbstractThreadedLocalReader {

                      private:
                        const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(PcscReader));

                      public:
                        static const std::string SETTING_KEY_TRANSMISSION_MODE;
                        static const std::string SETTING_TRANSMISSION_MODE_CONTACTS;
                        static const std::string SETTING_TRANSMISSION_MODE_CONTACTLESS;
                        static const std::string SETTING_KEY_PROTOCOL;
                        static const std::string SETTING_PROTOCOL_T0;
                        static const std::string SETTING_PROTOCOL_T1;
                        static const std::string SETTING_PROTOCOL_T_CL;
                        static const std::string SETTING_PROTOCOL_TX;
                        static const std::string SETTING_KEY_MODE;
                        static const std::string SETTING_MODE_EXCLUSIVE;
                        static const std::string SETTING_MODE_SHARED;
                        static const std::string SETTING_KEY_DISCONNECT;
                        static const std::string SETTING_DISCONNECT_RESET;
                        static const std::string SETTING_DISCONNECT_UNPOWER;
                        static const std::string SETTING_DISCONNECT_LEAVE;
                        static const std::string SETTING_DISCONNECT_EJECT;
                        static const std::string SETTING_KEY_THREAD_TIMEOUT;
                        static const std::string SETTING_KEY_LOGGING;

                      private:
                        static const std::string PROTOCOL_T0;
                        static const std::string PROTOCOL_T1;
                        static const std::string PROTOCOL_T_CL;
                        static const std::string PROTOCOL_ANY;

                        static constexpr long long SETTING_THREAD_TIMEOUT_DEFAULT = 5000;

                        const std::shared_ptr<CardTerminal> terminal;

                        std::string parameterCardProtocol;
                        bool cardExclusiveMode            = false;
                        bool cardReset                    = false;
                        TransmissionMode transmissionMode = static_cast<TransmissionMode>(0);

                        std::shared_ptr<Card> card;
                        std::shared_ptr<CardChannel> channel;

                        bool logging = false;


                        /**
                         * This constructor should only be called by PcscPlugin PCSC reader parameters are initialized
                         * with their default values as defined in setParameter. See
                         * {@link #setParameter(String, String)} for more details
                         *
                         * @param pluginName the name of the plugin
                         * @param terminal the PC/SC terminal
                         */
                    public:
                        PcscReader(const std::string &pluginName, std::shared_ptr<CardTerminal> terminal);

                        virtual ~PcscReader() { }

                    protected:
                        void closePhysicalChannel() override;

                        bool checkSePresence() override;

                        bool waitForCardPresent(long long timeout) override;

                        bool waitForCardAbsent(long long timeout) override;

                        /**
                         * Transmission of single APDU
                         *
                         * @param apduIn APDU in buffer
                         * @return apduOut buffer
                         * @throws KeypleIOReaderException if the transmission failed
                         */
                        std::vector<char> transmitApdu(std::vector<char> &apduIn) override;

                        /**
                         * Tells if the current SE protocol matches the provided protocol flag. If the protocol flag is
                         * not defined (null), we consider here that it matches. An exception is returned when the
                         * provided protocolFlag is not found in the current protocolMap.
                         *
                         * @param protocolFlag the protocol flag
                         * @return true if the current SE matches the protocol flag
                         * @throws KeypleReaderException if the protocol mask is not found
                         */
                        bool protocolFlagMatches(std::shared_ptr<SeProtocol> protocolFlag) override;

                        /**
                         * Set a parameter.
                         * <p>
                         * These are the parameters you can use with their associated values:
                         * <ul>
                         * <li><strong>protocol</strong>:
                         * <ul>
                         * <li>Tx: Automatic negotiation (default)</li>
                         * <li>T0: T0 protocol</li>
                         * <li>T1: T1 protocol</li>
                         * </ul>
                         * </li>
                         * <li><strong>mode</strong>:
                         * <ul>
                         * <li>shared: Shared between apps and threads (default)</li>
                         * <li>exclusive: Exclusive to this app and the current thread</li>
                         * </ul>
                         * </li>
                         * <li><strong>disconnect</strong>:
                         * <ul>
                         * <li>reset: Reset the card</li>
                         * <li>unpower: Simply unpower it</li>
                         * <li>leave: Unsupported</li>
                         * <li>eject: Eject</li>
                         * </ul>
                         * </li>
                         * <li><strong>thread_wait_timeout</strong>: Number of milliseconds to wait</li>
                         * </ul>
                         *
                         * @param name Parameter name
                         * @param value Parameter value
                         * @throws KeypleBaseException This method can fail when disabling the exclusive mode as it's
                         *         executed instantly
                         * @throws IllegalArgumentException when parameter is wrong
                         *
                         *
                         */
                      public:
                        void setParameter(const std::string &name, const std::string &value) override;

                        std::unordered_map<std::string, std::string> getParameters() override;

                      protected:
                        std::vector<char> getATR() override;

                        /**
                         * Tells if a physical channel is open
                         * <p>
                         * This status may be wrong if the card has been removed.
                         * <p>
                         * The caller should test the card presence with isSePresent before calling this method.
                         *
                         * @return true if the physical channel is open
                         */
                        bool isPhysicalChannelOpen() override;

                        /**
                         * Opens a physical channel
                         *
                         * The card access may be set to 'Exclusive' through the reader's settings.
                         *
                         * In this case be aware that on some platforms (ex. Windows 8+), the exclusivity is granted for
                         * a limited time (ex. 5 seconds). After this delay, the card is automatically resetted.
                         *
                         * @throws KeypleChannelStateException if a reader error occurs
                         */
                        void openPhysicalChannel() override;

                        /**
                         * The transmission mode can set with setParameter(SETTING_KEY_TRANSMISSION_MODE, )
                         * <p>
                         * When the transmission mode has not explicitly set, it is deduced from the protocol:
                         * <ul>
                         * <li>T=0: contacts mode</li>
                         * <li>T=1: contactless mode</li>
                         * </ul>
                         *
                         * @return the current transmission mode
                         */
                      public:
                        TransmissionMode getTransmissionMode() override;

                      protected:
                        std::shared_ptr<PcscReader> shared_from_this() {
                            return std::static_pointer_cast<PcscReader>(AbstractThreadedLocalReader::shared_from_this());
                        }

                      public:
                        bool equals(std::shared_ptr<void> o) override;

                        int hashCode() override;

                        void setParameters(std::unordered_map<std::string, std::string> &parameters) override;

                        void notifyObservers(std::shared_ptr<ReaderEvent> event) override;

                        std::string getName() override
                        {
                            return AbstractThreadedLocalReader::AbstractLoggedObservable<std::shared_ptr<ReaderEvent>>::getName();
                        }

                    };

                } // namespace pcsc
            }     // namespace plugin
        }         // namespace keyple
    }             // namespace eclipse
} // namespace org
