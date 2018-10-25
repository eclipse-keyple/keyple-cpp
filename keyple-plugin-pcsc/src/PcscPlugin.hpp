/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

/*!
 * \file pcscplugin/PcscPlugin.hpp.
 *
 * \brief Declares the PcscPlugin class
 */

#ifndef KEYPLE_SEPROXY_PCSC_READERS_PLUGIN_H
#define KEYPLE_SEPROXY_PCSC_READERS_PLUGIN_H

#include "ProxyReader.hpp"
#include "ReadersPlugin.hpp"

#include <string>
#include <list>
#include <thread>
#include <atomic>

/*!
 * \class PcscPlugin
 * 
 * \brief PCSC plugin class.
 */

class PcscPlugin : public keyple::seproxy::ReadersPlugin {
  public:
    /*!
	 * \fn PcscPlugin::PcscPlugin();
	 *
	 * \brief Default constructor
	 */

    PcscPlugin();

    /*!
	 * \fn PcscPlugin::~PcscPlugin();
	 *
	 * \brief Destructor
	 */

    ~PcscPlugin();

    /*!
	 * \fn static ReadersPlugin* PcscPlugin::getInstance();
	 *
	 * \brief Gets the instance
	 *
	 * \return Null if it fails, else the instance.
	 */

    static ReadersPlugin *getInstance();

    /*!
	 * \fn void PcscPlugin::destroy();
	 *
	 * \brief Destroys this object
	 */

    void destroy();

    /*!
	 * \fn std::string PcscPlugin::getName();
	 *
	 * \brief Gets the name
	 *
	 * \return The name.
	 */

    std::string getName();

    /*!
	 * \fn std::list<keyple::seproxy::ProxyReader*> PcscPlugin::getReaders();
	 *
	 * \brief Gets the readers
	 *
	 * \return Null if it fails, else the readers.
	 */

    std::list<keyple::seproxy::ProxyReader *> getReaders();

    /*!
	 * \fn void PcscPlugin::readerPresenceMonitoringThread();
	 *
	 * \brief Reader presence monitoring thread
	 */

    void readerPresenceMonitoringThread();

  private:
    PcscPlugin &operator=(const PcscPlugin *){};

    PcscPlugin(const PcscPlugin *){};

    static PcscPlugin m_instance;

    static std::atomic<bool> m_monitoring_is_running;

    std::list<keyple::seproxy::ProxyReader *> m_readers_list;

    /* PCSC Specific */
    SCARDCONTEXT m_context;
};

#endif // KEYPLE_SEPROXY_PCSC_READERS_PLUGIN_H
