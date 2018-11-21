/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

/*!
 * \file pcscplugin/PcscReader.hpp.
 *
 * \brief Declares the PcscReader class
 */

#ifndef KEYPLE_PLUGIN_PCSC_READER_H
#define KEYPLE_PLUGIN_PCSC_READER_H

#include <atomic>
#include <thread>
#include <vector>

#include "AbstractThreadedLocalReader.hpp"
#include "CardTerminal.hpp"

using namespace keyple::plugin::pcsc;
using namespace keyple::seproxy::plugin;

namespace keyple {
    namespace plugin {
        namespace pcsc {

            /**
             *
             */
            class PcscReader : public AbstractThreadedLocalReader {
              public:
                static constexpr const char* const KEY = "TheKey";
                static constexpr const char* const SETTING_KEY_PROTOCOL = "protocol";
                static constexpr const char* const SETTING_PROTOCOL_T0 = "T0";
                static constexpr const char* const SETTING_PROTOCOL_T1 = "T1";
                static constexpr const char* const SETTING_PROTOCOL_TX = "Tx";
                static constexpr const char* const SETTING_KEY_MODE = "mode";
                static constexpr const char* const SETTING_MODE_EXCLUSIVE = "exclusive";
                static constexpr const char* const SETTING_MODE_SHARED = "shared";
                static constexpr const char* const SETTING_KEY_DISCONNECT = "disconnect";
                static constexpr const char* const SETTING_DISCONNECT_RESET = "reset";
                static constexpr const char* const SETTING_DISCONNECT_UNPOWER = "unpower";
                static constexpr const char* const SETTING_DISCONNECT_LEAVE = "leave";
                static constexpr const char* const SETTING_DISCONNECT_EJECT = "eject";
                static constexpr const char* const SETTING_KEY_THREAD_TIMEOUT = "thread_wait_timeout";
                static constexpr const char* const SETTING_KEY_LOGGING = "logging";

                /*!
                  * \fn PcscReader::PcscReader();
                  *
                  * \brief Default constructor
                  */
                PcscReader(const std::string &pluginName, CardTerminal &terminal);

                /*!
                 * \fn PcscReader::~PcscReader();
                 *
                 * \brief Destructor
                 */
                ~PcscReader();

                //  ///
                //  /// ProxyReader members

                //  /*!
                //* \fn std::string PcscReader::getName();
                //*
                //* \brief Gets the name of the reader
                //*
                //* \return The name.
                //*/

                //  std::string getName();

                //  /*!
                //* \fn keyple::containers::SeResponse PcscReader::transmit(keyple::containers::SeRequest* inSeApplicationRequest);
                //*
                //* \brief Transmits the given in SE application request, returns a SE response.
                //*
                //* \param [in] inSeApplicationRequest If non-null, the in se application request.
                //*
                //* \return A keyple::containers::SeResponse.
                //*/

                //  keyple::containers::SeResponse transmit(keyple::containers::SeRequest *inSeApplicationRequest);

                /**
                 * Checks if is SE present.
                 *
                 * @return true if a Secure Element is present in the reader
                 * @throws NoStackTraceThrowable a exception without stack trace in order to be catched and
                 *         processed silently
                 */
                bool isSePresent() /*throws NoStackTraceThrowable; */ final override;


                //  ///
                //  /// ConfigurableReader members

                //  /*!
                //* \fn std::map<std::string, std::string>* PcscReader::getParameters();
                //*
                //* \brief Gets the parameters
                //*
                //* \return Null if it fails, else the parameters.
                //*/

                //  std::map<std::string, std::string> *getParameters();

                //  /*!
                //* \fn ExecutionStatus PcscReader::setAParameter(std::string inKey, std::string inValue);
                //*
                //* \brief Sets a parameter
                //*
                //* \param inKey   The in key.
                //* \param inValue The in value.
                //*
                //* \return The ExecutionStatus.
                //*/

                //  ExecutionStatus setAParameter(std::string inKey, std::string inValue);

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
                void setParameter(std::string key, std::string value);

                //  /*!
                //* \fn ExecutionStatus PcscReader::attachObserver(keyple::seproxy::ReaderObserver * callBack);
                //*
                //* \brief Attach observer
                //*
                //* \param [in,out] callBack If non-null, the call back.
                //*
                //* \return The ExecutionStatus.
                //*/

                //  ///
                //  /// ObservableReader members

                //  ExecutionStatus attachObserver(keyple::seproxy::ReaderObserver *callBack);

                //  /*!
                //* \fn ExecutionStatus PcscReader::detachObserver(keyple::seproxy::ReaderObserver * callBack);
                //*
                //* \brief Detach observer
                //*
                //* \param [in,out] callBack If non-null, the call back.
                //*
                //* \return The ExecutionStatus.
                //*/

                //  ExecutionStatus detachObserver(keyple::seproxy::ReaderObserver *callBack);

                //  /*!
                //* \fn void PcscReader::notifyObservers(keyple::containers::ReaderEvent event);
                //*
                //* \brief Notifies the observers
                //*
                //* \param event The event.
                //*/

                //  void notifyObservers(keyple::containers::ReaderEvent event);

                //  /// PCSC Specific members

                //  /*!
                //* \fn ExecutionStatus PcscReader::setName(std::string name);
                //*
                //* \brief Sets a name
                //*
                //* \param name The name.
                //*
                //* \return The ExecutionStatus.
                //*/

                //  ExecutionStatus setName(std::string name);

                //  /*!
                //* \fn ExecutionStatus PcscReader::setContext(SCARDCONTEXT context);
                //*
                //* \brief Sets a context
                //*
                //* \param context The context.
                //*
                //* \return The ExecutionStatus.
                //*/

                //  ExecutionStatus setContext(SCARDCONTEXT context);

                //  /*!
                //* \fn void PcscReader::cardPresenceMonitoringThread();
                //*
                //* \brief Card presence monitoring thread
                //*/

                //  void cardPresenceMonitoringThread();

                /**
                 * Gets the SE Answer to reset
                 *
                 * @return ATR returned by the SE or reconstructed by the reader (contactless)
                 */
                std::vector<uint8_t>& getATR() final override;

                ///**
                // * Tells if the physical channel is open or not
                // *
                // * @return true is the channel is open
                // */
                //bool isPhysicalChannelOpen()
                //{
                //    return false;
                //}

                ///**
                // * Waits for a card. Returns true if a card is detected before the end of the provided timeout.
                // * Returns false if no card detected within the delay.
                // *
                // * @param timeout the delay in millisecond we wait for a card insertion
                // * @return presence status
                // * @throws NoStackTraceThrowable a exception without stack trace in order to be catched and
                // *         processed silently
                // */
                //bool waitForCardPresent(long timeout)
                //{
                //    return false;
                //}
                //// throws NoStackTraceThrowable;

                ///**
                // * Wait until the card disappears. Returns true if a card has disappeared before the end of the
                // * provided timeout. Returns false if the is still present within the delay. Closes the physical
                // * channel when the card has disappeared.
                // *
                // * @param timeout the delay in millisecond we wait for a card to be withdrawn
                // * @return presence status
                // * @throws NoStackTraceThrowable a exception without stack trace in order to be catched and
                // *         processed silently
                // */
                //bool waitForCardAbsent(long timeout)
                //{
                //    return false;
                //}

              private:
                /*
                 *
                 */
                bool logging;
                
                /*
                 *
                 */
                std::string parameterCardProtocol;
                
                /*
                 * 
                 */
                CardTerminal terminal;
                
                /*
                 *
                 */
                Card card;

                //  std::vector<uint8_t> scardTransmit(std::vector<uint8_t> apdu);
                //  std::vector<uint8_t> getFCI(std::vector<uint8_t> aid);
                //  std::vector<uint8_t> getResponseCase4();

                //  /// These properties are accessed by the card presence monitoring thread.
                //  std::thread m_monitor;
                //  std::atomic<bool> m_monitoring_is_running;
                //  std::atomic<bool> m_card_presence;

                //  char m_name[K_OS_KPL_DEV_NAME_MAX_LENGTH]; // TODO change type (string?)
                //  SCARDCONTEXT m_context;
                //  SCARDHANDLE m_card;
                //  uint32_t m_share_mode;
                //  uint32_t m_protocol;
                //  uint32_t m_active_protocol;
                //  uint32_t m_disposition;
                //  bool m_channel_openned;
                //  std::vector<uint8_t> m_fci;
                //  std::vector<uint8_t> m_atr;
                //  std::map<std::string, std::string> m_settings;
                //  std::list<keyple::seproxy::ReaderObserver *> m_observers_list;
            };
        } // namespace pcsc
    }     // namespace plugin
} // namespace keyple

#endif // KEYPLE_PLUGIN_PCSC_READER_H
