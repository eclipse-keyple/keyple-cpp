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

#ifndef KEYPLE_SEPROXY_PCSC_READER_H
#define KEYPLE_SEPROXY_PCSC_READER_H

#include "ProxyReader.hpp"
#include "ConfigurableReader.hpp"
#include "ObservableReader.hpp"
#include "ReaderObserver.hpp"

#include <atomic>
#include <thread>
#include <vector>

/*!
 * \class PcscReader
 *
 * \brief PCSC reader plugin class.
 */

class PcscReader : public keyple::seproxy::ConfigurableReader, public keyple::seproxy::ObservableReader {
  public:
    /*!
	 * \fn PcscReader::PcscReader();
	 *
	 * \brief Default constructor
	 */

    PcscReader();

    /*!
	 * \fn PcscReader::~PcscReader();
	 *
	 * \brief Destructor
	 */

    ~PcscReader();

    ///
    /// ProxyReader members

    /*!
	 * \fn std::string PcscReader::getName();
	 *
	 * \brief Gets the name of the reader
	 *
	 * \return The name.
	 */

    std::string getName();

    /*!
	 * \fn keyple::containers::SeResponse PcscReader::transmit(keyple::containers::SeRequest* inSeApplicationRequest);
	 *
	 * \brief Transmits the given in SE application request, returns a SE response.
	 *
	 * \param [in] inSeApplicationRequest If non-null, the in se application request.
	 *
	 * \return A keyple::containers::SeResponse.
	 */

    keyple::containers::SeResponse transmit(keyple::containers::SeRequest *inSeApplicationRequest);

    /*!
	 * \fn bool PcscReader::isSePresent();
	 *
	 * \brief Query if Secure Element is present
	 *
	 * \return True if se present, false if not.
	 */

    bool isSePresent();

    ///
    /// ConfigurableReader members

    /*!
	 * \fn std::map<std::string, std::string>* PcscReader::getParameters();
	 *
	 * \brief Gets the parameters
	 *
	 * \return Null if it fails, else the parameters.
	 */

    std::map<std::string, std::string> *getParameters();

    /*!
	 * \fn ExecutionStatus PcscReader::setAParameter(std::string inKey, std::string inValue);
	 *
	 * \brief Sets a parameter
	 *
	 * \param inKey   The in key.
	 * \param inValue The in value.
	 *
	 * \return The ExecutionStatus.
	 */

    ExecutionStatus setAParameter(std::string inKey, std::string inValue);

    /*!
	 * \fn ExecutionStatus PcscReader::setParameters(std::map<std::string, std::string>* inSettings);
	 *
	 * \brief Sets the parameters
	 *
	 * \param [in,out] inSettings If non-null, the in settings.
	 *
	 * \return The ExecutionStatus.
	 */

    ExecutionStatus setParameters(std::map<std::string, std::string> *inSettings);

    /*!
	 * \fn ExecutionStatus PcscReader::attachObserver(keyple::seproxy::ReaderObserver * callBack);
	 *
	 * \brief Attach observer
	 *
	 * \param [in,out] callBack If non-null, the call back.
	 *
	 * \return The ExecutionStatus.
	 */

    ///
    /// ObservableReader members

    ExecutionStatus attachObserver(keyple::seproxy::ReaderObserver *callBack);

    /*!
	 * \fn ExecutionStatus PcscReader::detachObserver(keyple::seproxy::ReaderObserver * callBack);
	 *
	 * \brief Detach observer
	 *
	 * \param [in,out] callBack If non-null, the call back.
	 *
	 * \return The ExecutionStatus.
	 */

    ExecutionStatus detachObserver(keyple::seproxy::ReaderObserver *callBack);

    /*!
	 * \fn void PcscReader::notifyObservers(keyple::containers::ReaderEvent event);
	 *
	 * \brief Notifies the observers
	 *
	 * \param event The event.
	 */

    void notifyObservers(keyple::containers::ReaderEvent event);

    /// PCSC Specific members

    /*!
	 * \fn ExecutionStatus PcscReader::setName(std::string name);
	 *
	 * \brief Sets a name
	 *
	 * \param name The name.
	 *
	 * \return The ExecutionStatus.
	 */

    ExecutionStatus setName(std::string name);

    /*!
	 * \fn ExecutionStatus PcscReader::setContext(SCARDCONTEXT context);
	 *
	 * \brief Sets a context
	 *
	 * \param context The context.
	 *
	 * \return The ExecutionStatus.
	 */

    ExecutionStatus setContext(SCARDCONTEXT context);

    /*!
	 * \fn void PcscReader::cardPresenceMonitoringThread();
	 *
	 * \brief Card presence monitoring thread
	 */

    void cardPresenceMonitoringThread();

  private:
    std::vector<uint8_t> scardTransmit(std::vector<uint8_t> apdu);
    std::vector<uint8_t> getFCI(std::vector<uint8_t> aid);
    std::vector<uint8_t> getResponseCase4();

    /// These properties are accessed by the card presence monitoring thread.
    std::thread m_monitor;
    std::atomic<bool> m_monitoring_is_running;
    std::atomic<bool> m_card_presence;

    char m_name[K_OS_KPL_DEV_NAME_MAX_LENGTH]; // TODO change type (string?)
    SCARDCONTEXT m_context;
    SCARDHANDLE m_card;
    uint32_t m_share_mode;
    uint32_t m_protocol;
    uint32_t m_active_protocol;
    uint32_t m_disposition;
    bool m_channel_openned;
    std::vector<uint8_t> m_fci;
    std::vector<uint8_t> m_atr;
    std::map<std::string, std::string> m_settings;
    std::list<keyple::seproxy::ReaderObserver *> m_observers_list;
};

#endif
