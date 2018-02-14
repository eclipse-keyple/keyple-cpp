/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

// TestPlugin.cpp : Defines the entry point for the console application.
//

#include "KplSdkConfig.hpp"
#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <system_error>

#include "ReadersPlugin.hpp"
#include "SeProxyService.hpp"
#include "ConfigurableReader.hpp"
#include "ObservableReader.hpp"
#include "ReaderParameters.hpp"

#include "Observer.hpp"

using namespace keyple;
using namespace seproxy;
using namespace containers;
using namespace std;

/// A factory of ReadersPlugin-implementing objects
typedef ReadersPlugin* (*ReadersPluginFactory)();

/// A factory of SeProxyService-implementing objects
typedef SeProxyService* (*SeProxyServiceFactory)();

/*!
 * \fn ExecutionStatus Observer::notify(keyple::containers::ReaderEvent *event)
 *
 * \brief Observer callback, called when a reader event occurs
 *
 * \return The ExecutionStatus.
 */

ExecutionStatus Observer::notify(ReaderEvent event)
{
	ExecutionStatus status = UNEXPECTED_READER_ERROR;

	if (event.getEventType() == ReaderEvent::EventType::SE_INSERTED)
	{
		ProxyReader *reader = static_cast<ProxyReader*>(event.getReader());

		DBG_INFO_MSG("Reader " << reader->getName());

		std::vector<uint8_t> AID = { 0xA0, 0x00, 0x00, 0x04, 0x04, 0x01, 0x25, 0x09, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

		std::list<ApduRequest> listApdu;

		std::vector<uint8_t> apdu = { 0 };

		ApduRequest *apduRequest = new ApduRequest(apdu, false);

		listApdu.push_back(*apduRequest);

		SeRequest *se_request = new SeRequest(AID, listApdu, false);
		
		try {
			SeResponse se_response = reader->transmit(se_request);

			std::vector<uint8_t> atr = se_response.getAtr();

			DBG_DUMP_HEX_VECTOR("ATR: ", atr);

			std::vector<uint8_t> fci = se_response.getFci().getBytes();

			DBG_DUMP_HEX_VECTOR("FCI: ", fci);
		}
		catch (const std::system_error& ex)
		{
			DBG_ERROR_MSG(ex.code().value());
			DBG_ERROR_MSG(ex.code().message());
		}
	}
	return status; 
}

int main()
{
	// Load the plugin DLL
	handle_t plugin_dll_handle = ::OsOpenLibrary(PcscPlugin);
	if (!plugin_dll_handle) {
		DBG_ERROR_MSG("Load library error: " << OsGetLastError());
		exit(EXIT_FAILURE);
	}

	// Get the function from the DLL
	ReadersPluginFactory readers_plugin_factory_func = reinterpret_cast<ReadersPluginFactory>(::OsGetSymbol(plugin_dll_handle, "CreateClass"));
	if (!readers_plugin_factory_func) {
		DBG_ERROR_MSG("Unable to load CreateClass from library!");
		::OsCloseLibrary(plugin_dll_handle);
		exit(EXIT_FAILURE);
	}

	// Ask the factory for a new object implementing the ReadersPlugin
	// interface
	ReadersPlugin* readers_plugin_instance = readers_plugin_factory_func();

	// Play with the plugin object
	DBG_INFO_MSG("plugin name = " << readers_plugin_instance->getName());

	// Load the SeProxyService DLL
	handle_t seproxy_dll_handle = ::OsOpenLibrary(SeProxyService);
	if (!seproxy_dll_handle) {
		DBG_ERROR_MSG("Load library error: " << OsGetLastError());
		exit(EXIT_FAILURE);
	}

	// Get the CreateClass function from the DLL
	SeProxyServiceFactory se_proxy_service_factory_func = reinterpret_cast<SeProxyServiceFactory>(
		::OsGetSymbol(seproxy_dll_handle, "CreateClass"));
	if (!se_proxy_service_factory_func) {
		DBG_ERROR_MSG("Unable to load CreateClass from library!");
		::OsCloseLibrary(seproxy_dll_handle);
		exit(EXIT_FAILURE);
	}

	// Ask the factory for a new object implementing the SeProxyService
	// interface
	SeProxyService* se_proxy_service_instance = se_proxy_service_factory_func();

	// Play with the seproxyservice object
	list<ReadersPlugin*> readers_plugin_list_in;

	readers_plugin_list_in.push_back(readers_plugin_instance);

	se_proxy_service_instance->setPlugins(readers_plugin_list_in);

	list<ReadersPlugin*> readers_plugin_list_out;

	readers_plugin_list_out = se_proxy_service_instance->getPlugins();

	list<ReadersPlugin*>::iterator iterator_plugin = readers_plugin_list_out.begin();

	ReadersPlugin* plugin_instance_2 = *iterator_plugin;

	// play with retrieved plugin
	DBG_INFO_MSG("plugin_instance_2->getName() = " << plugin_instance_2->getName());

	list<ProxyReader*>readers_list;
	readers_list = plugin_instance_2->getReaders();

	list<ProxyReader*>::iterator iterator_reader = readers_list.begin();

	Observer observer;

	for (uint32_t ii = 0; ii < readers_list.size(); ii++)
	{
		ProxyReader* reader = *iterator_reader;
		DBG_INFO_MSG("[" << ii << "] reader->getName() = " << reader->getName());

		ConfigurableReader *configurable_reader = dynamic_cast<ConfigurableReader*>(reader);

		configurable_reader->setAParameter(PARAM_KEY_SHARE_MODE, PARAM_VALUE_SHARE_SHARED);

		configurable_reader->getParameters();

		ObservableReader *observable_reader = dynamic_cast<ObservableReader*>(reader);

		observable_reader->attachObserver(&observer);

		iterator_reader++;
	}


	//std::vector<uint8_t> AIDback = se_request->getAidToSelect();




	DBG_INFO_MSG("Waiting for events, press a key to exit...");

	cin.get();

	/// Explicitly destroys created objects
	//delete se_proxy_service_instance;
	//readers_plugin_instance->destroy();

	/// Unload libraries
	::OsCloseLibrary(plugin_dll_handle);
	::OsCloseLibrary(seproxy_dll_handle);

	exit(EXIT_SUCCESS);
}

