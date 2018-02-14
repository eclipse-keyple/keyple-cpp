/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */


 /*!
 * \file pcscplugin/PcscPlugin.cpp.
 *
 * \brief Implements the PcscPlugin class
 */

#include "PlatformConfig.hpp"
#include <iostream>
#include <winscard.h>
#include "ReadersPlugin.hpp"
#include "ProxyReader.hpp"
#include "PcscPlugin.hpp"
#include "PcscReader.hpp"

using namespace keyple;
using namespace seproxy;

/*!
 * \fn PcscPlugin::PcscPlugin()
 *
 * \brief Default constructor
 */

PcscPlugin::PcscPlugin() 
{
	DBG_TRACE_CALL();
	LONG            lReturn;
	this->m_context = 0;
	//m_monitoring_is_running = false;

	/// Establish the PC/SC context.
	lReturn = SCardEstablishContext(SCARD_SCOPE_USER,
		NULL,
		NULL,
		&this->m_context);
	if (SCARD_S_SUCCESS != lReturn)
	{
		DBG_ERROR_MSG("Failed SCardEstablishContext");
		// TODO define behaviour
	}
	else
	{
		// Use the context as needed. When done,
		// free the context by calling SCardReleaseContext.
		// ...
	}
}


/*!
 * \fn PcscPlugin::~PcscPlugin()
 *
 * \brief Destructor
 */

PcscPlugin::~PcscPlugin()
{
	DBG_TRACE_CALL();
}

/*!
 * \fn ReadersPlugin* PcscPlugin::getInstance()
 *
 * \brief Gets the instance
 *
 * \return Null if it fails, else the instance.
 */

ReadersPlugin* PcscPlugin::getInstance()
{
	DBG_TRACE_CALL();
	return &m_instance;
}

void PcscPlugin::destroy()
{
	/// Stop the monitoring thread.
	/// TODO m_monitoring_is_running = false;

	/// Delete all reader objects.
	std::list<ProxyReader*>::iterator iterator_reader = m_readers_list.begin();
	for (uint32_t ii = 0; ii < m_readers_list.size(); ii++)
	{
		ProxyReader* proxy_reader = *iterator_reader;
		delete proxy_reader;
		iterator_reader++;
	}

	/// Release the PC/SC context.
	if (this->m_context != 0)
	{
		SCardReleaseContext(this->m_context);
	}

	delete this;
}

/*!
 * \fn std::string PcscPlugin::getName()
 *
 * \brief Returns the unique name of the readers’ plugin
 *
 * \return The name.
 */

std::string PcscPlugin::getName()
{
	DBG_TRACE_CALL();
	return "PcscPlugin";
}

/*!
 * \fn std::list<ProxyReader*> PcscPlugin::getReaders()
 *
 * \brief Returns the list of available connected SE readers for the selected plugin
 *
 * \return Null if it fails, else the readers.
 */

std::list<ProxyReader*> PcscPlugin::getReaders()
{
	DBG_TRACE_CALL();
	LPTSTR          pmszReaders = NULL;
	LPTSTR          pReader;
	LONG            lReturn, lReturn2;
	DWORD           cch = SCARD_AUTOALLOCATE;

	/// Retrieve the list the readers.
	///	 m_context was set by a previous call to SCardEstablishContext.
	lReturn = SCardListReaders(this->m_context,
		NULL,
		(LPTSTR)&pmszReaders,
		&cch);
	switch (lReturn)
	{
	case SCARD_E_NO_READERS_AVAILABLE:
		DBG_INFO_MSG("Reader is not in groups.");
		// TODO Take appropriate action.
		// ...
		break;

	case SCARD_S_SUCCESS:
		// Do something with the multi string of readers.
		// Output the values.
		// A double-null terminates the list of values.
		pReader = pmszReaders;
		while ('\0' != *pReader)
		{
			DBG_INFO_MSG("Reader: " << pReader);

			/// Create a new PcscReader object
			PcscReader *proxyReader = new PcscReader();
			
			/// Set PCSC reader name
			std::string readerName(pReader);
			proxyReader->setName(readerName);

			/// Set PCSC context
			proxyReader->setContext(this->m_context);

			/// Add reader to the list
			m_readers_list.push_back(static_cast<ProxyReader*>(static_cast<ConfigurableReader*>(proxyReader)));

			/// Advance to the next value.
			pReader = pReader + strlen(pReader) + 1;
		}
		/// Free the memory.
		lReturn2 = SCardFreeMemory(m_context,
			pmszReaders);
		if (SCARD_S_SUCCESS != lReturn2)
			DBG_ERROR_MSG("Failed SCardFreeMemory");
		break;

	default:
		DBG_ERROR_MSG("Failed SCardListReaders");
		// TODO Take appropriate action.
		// ...
		break;
	}
	return m_readers_list;
}

/*!
 * \fn void PcscPlugin::readerPresenceMonitoringThread()
 *
 * \brief Reader presence monitoring thread (experimental)
 */

void PcscPlugin::readerPresenceMonitoringThread()
{
	uint32_t lReturn;
	SCARD_READERSTATE rgscState;
	const char* readerNotification = "\\\\?PnP?\\Notification";

	memset(&rgscState, 0, sizeof(rgscState));

	rgscState.szReader = readerNotification;
	rgscState.dwCurrentState = SCARD_STATE_UNAWARE;

	while (true)
	{
		lReturn = SCardGetStatusChange(this->m_context,
									   INFINITE, // infinite wait
									   &rgscState,
									   1);
		rgscState.dwCurrentState = rgscState.dwEventState;
		if (lReturn == SCARD_S_SUCCESS)
		{
			DBG_ERROR_MSG("SCardGetStatusChange returned " << std::hex << lReturn);
		}
		else
		{
			DBG_ERROR_MSG("SCardGetStatusChange returned " << std::hex << lReturn);
		}
	}
}

/*! \brief The pcsc plugin m instance */
PcscPlugin PcscPlugin::m_instance = PcscPlugin();

/*!
 * \fn extern "C" EXPORT ReadersPlugin* CreateClass()
 *
 * \brief Creates the class ("singleton in library" pattern)
 *
 * \return Null if it fails, else the new class.
 */

extern "C" EXPORT ReadersPlugin* CreateClass()
{
	DBG_TRACE_CALL();
	return PcscPlugin::getInstance();
}