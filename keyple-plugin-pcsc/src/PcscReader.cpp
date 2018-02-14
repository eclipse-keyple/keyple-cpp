/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

/*!
 * \file PcscReader.cpp.
 *
 * \brief Implements the PcscReader class
 */

#include "PlatformConfig.hpp"
#include <cstring>
#include <iostream>
#include <iomanip>
#include <thread>
#include <system_error>
#include <winscard.h>
#ifdef __linux__ 
#include <reader.h>
#endif // __linux__

#include "ReaderParameters.hpp"
#include "ReaderEvent.hpp"
#include "PcscReader.hpp"

using namespace keyple;
using namespace seproxy;

/*!
 * \fn PcscReader::PcscReader()
 *
 * \brief Default constructor
 */

PcscReader::PcscReader()
{
	DBG_TRACE_CALL();
	// initialize default value of private properties
	this->m_context = 0;
	this->m_card = 0;
	strcpy(this->m_name, "Undefined");
	this->m_share_mode = SCARD_SHARE_EXCLUSIVE;
	this->m_protocol = SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1;
	this->m_disposition = SCARD_RESET_CARD;
	this->m_card_presence = false;
	this->m_channel_openned = false;
	this->m_monitoring_is_running = false;
}

/*!
 * \fn PcscReader::~PcscReader()
 *
 * \brief Destructor
 */

PcscReader::~PcscReader()
{
	DBG_TRACE_CALL();
	this->m_monitoring_is_running = false;
}

/*!
 * \fn std::string PcscReader::getName()
 *
 * \brief Gets the name
 *
 * \return The name.
 */

std::string PcscReader::getName()
{
	DBG_TRACE_CALL();
	return this->m_name;
}

/*!
* \fn keyple::containers::SeResponse PcscReader::transmit(keyple::containers::SeRequest* inSeApplicationRequest)
*
* \brief Transmits the given in se application request
*
* \exception std::system_error Raised when a system error condition occurs.
*
* \param [in,out] inSeApplicationRequest If non-null, the in se application request.
*
* \return A keyple::containers::SeResponse.
*/

keyple::containers::SeResponse PcscReader::transmit(keyple::containers::SeRequest* inSeApplicationRequest)
{
	DBG_TRACE_CALL();
	std::vector<uint8_t> fci;

	/// Do a card connection if needed, keep ATR
	if (this->m_card == 0)
	{
		if (SCardConnect(this->m_context,
			this->m_name,
			this->m_share_mode,
			this->m_protocol,
			&this->m_card,
			(unsigned long*)&this->m_active_protocol) != SCARD_S_SUCCESS)
		{
			this->m_card = 0;
			this->m_atr.clear();
		}
		else
		{
			uint8_t *atr = NULL;
			unsigned long cbyte = SCARD_AUTOALLOCATE;
			this->m_atr.clear();
			/// Get the ATR buffer as attribute
			if (SCardGetAttrib(this->m_card,
							   SCARD_ATTR_ATR_STRING,
							   (uint8_t*)&atr,
							   &cbyte) == SCARD_S_SUCCESS)
			{
				/// Put response in ATR vector
				this->m_atr.assign(atr, atr + cbyte);
				SCardFreeMemory(this->m_context, atr);
			}
			else
			{
				/// Fail to get ATR. Reset card presence. TODO check this point!
				this->m_card = 0;
				this->m_atr.clear();
			}
		}
		/// We consider here that no channel is open since we just connect to the card
		this->m_channel_openned = false;
	}

	/// Handle channel opening
	if (this->m_card != 0
		&& this->m_channel_openned == false)
	{
		/// 
		std::vector<uint8_t> aid = inSeApplicationRequest->getAidToSelect();
		if (aid.size() > 0) {
			/// Get the FCI executing application selection
			std::vector<uint8_t> selectApplicationAPDU(5);
			/// Build the selection application APDU
			selectApplicationAPDU[0] = (uint8_t)0x00;
			selectApplicationAPDU[1] = (uint8_t)0xA4;
			selectApplicationAPDU[2] = (uint8_t)0x04;
			selectApplicationAPDU[3] = (uint8_t)0x00;
			selectApplicationAPDU[4] = (uint8_t)aid.size();

			selectApplicationAPDU.insert(std::end(selectApplicationAPDU),
				std::begin(aid),
				std::end(aid));
			fci = scardTransmit(selectApplicationAPDU);
			if (fci.size() == 0)
			{
				this->m_channel_openned = false;
			}
			else
			{
				this->m_fci = fci;
				this->m_channel_openned = true;
			}
		}
		else
		{
			/// We assume here that the Secure Element do not need an explicit application selection.
			// TODO check what to do here!
			this->m_fci.clear();
			this->m_channel_openned = true;
		}
	}
	else
	{
		/// Set the FCI previously obtained from the card.
		fci = this->m_fci;
	}

	/// Build FCI Apdu response output (may be empty)
	keyple::containers::ApduResponse fciApduResponse(fci, true);

	/// Handle the requests looping for all requests present in the provided list
	std::list<keyple::containers::ApduResponse> apduResponses;
	std::list<keyple::containers::ApduRequest>::iterator iterator;
	
	for (iterator = inSeApplicationRequest->getApduRequests().begin(); iterator != inSeApplicationRequest->getApduRequests().end(); iterator++)
	{
		keyple::containers::ApduRequest apduRequest = *iterator;
		std::vector<uint8_t> apdu = apduRequest.getBytes();

		/// low level transmission (may throw an exeception)
		std::vector<uint8_t> response = scardTransmit(apdu);

		/// build ApduResponse
		// TODO change condition to set successful flag true or false
		// TODO handle case4
		keyple::containers::ApduResponse apduResponse(response, response.size() != 0 ? true : false);

		/// add response to the list
		apduResponses.push_back(apduResponse);
	}

	/// Close connection if requested by the application
	if (inSeApplicationRequest->askKeepChannelOpen() == false)
	{
		/// Disconnect according to disposition setting
		// TODO handle error code
		SCardDisconnect(this->m_card, this->m_disposition);
	}

	/// build returned SeResponse object
	keyple::containers::SeResponse seResponse(true, fciApduResponse, apduResponses, this->m_atr);

	return seResponse;
}

/*!
 * \fn bool PcscReader::isSePresent()
 *
 * \brief Query if this object is se present
 *
 * \return True if se present, false if not.
 */

bool PcscReader::isSePresent()
{
	DBG_TRACE_CALL();

	SCARDHANDLE     hCardHandle;
	LONG            lReturn;
	DWORD           dwAP;

	if (this->m_card_presence == false)
	{
		// try to connect

		lReturn = SCardConnect((SCARDCONTEXT)this->m_context,
								this->m_name,
								this->m_share_mode,
								this->m_protocol,
								&hCardHandle,
								&dwAP);
		if (SCARD_S_SUCCESS != lReturn)
		{
			DBG_ERROR_MSG("Failed SCardConnect");
		}
		else
		{
			// Use the connection.
			// Display the active protocol.
			switch (dwAP)
			{
			case SCARD_PROTOCOL_T0:
				DBG_INFO_MSG("Active protocol T0");
				break;

			case SCARD_PROTOCOL_T1:
				DBG_INFO_MSG("Active protocol T1");
				break;

			case SCARD_PROTOCOL_UNDEFINED:
			default:
				DBG_INFO_MSG("Active protocol unnegotiated or unknown");
				break;
			}
			this->m_card = hCardHandle;
			this->m_card_presence = true;
		}
	}
	
	return this->m_card_presence;
}

/*!
 * \fn std::map<std::string, std::string>* PcscReader::getParameters()
 *
 * \brief Gets the parameters
 *
 * \return Null if it fails, else the parameters.
 */

std::map<std::string, std::string>* PcscReader::getParameters()
{
	DBG_TRACE_CALL();

	this->m_settings.clear();

	// SHARE_MODE 
	switch (this->m_share_mode)
	{
	case SCARD_SHARE_SHARED:
		DBG_INFO_MSG("Key = " << PARAM_KEY_SHARE_MODE << ", Value = " << PARAM_VALUE_SHARE_SHARED);
		this->m_settings.insert(std::pair<std::string, std::string>(PARAM_KEY_SHARE_MODE, PARAM_VALUE_SHARE_SHARED));
		break;
	case SCARD_SHARE_EXCLUSIVE:
		DBG_INFO_MSG("Key = " << PARAM_KEY_SHARE_MODE << ", Value = " << PARAM_VALUE_SHARE_EXCLUSIVE);
		this->m_settings.insert(std::pair<std::string, std::string>(PARAM_KEY_SHARE_MODE, PARAM_VALUE_SHARE_EXCLUSIVE));
		break;
	case SCARD_SHARE_DIRECT:
		DBG_INFO_MSG("Key = " << PARAM_KEY_SHARE_MODE << ", Value = " << PARAM_VALUE_SHARE_DIRECT);
		this->m_settings.insert(std::pair<std::string, std::string>(PARAM_KEY_SHARE_MODE, PARAM_VALUE_SHARE_DIRECT));
		break;
	}

	// PROTOCOL 
	switch (this->m_protocol)
	{
	case SCARD_PROTOCOL_T0:
		DBG_INFO_MSG("Key = " << PARAM_KEY_PROTOCOL << ", Value = " << PARAM_VALUE_PROTOCOL_T0);
		this->m_settings.insert(std::pair<std::string, std::string>(PARAM_KEY_SHARE_MODE, PARAM_VALUE_PROTOCOL_T0));
		break;
	case SCARD_PROTOCOL_T1:
		DBG_INFO_MSG("Key = " << PARAM_KEY_PROTOCOL << ", Value = " << PARAM_VALUE_PROTOCOL_T1);
		this->m_settings.insert(std::pair<std::string, std::string>(PARAM_KEY_SHARE_MODE, PARAM_VALUE_PROTOCOL_T1));
		break;
	case SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1:
		DBG_INFO_MSG("Key = " << PARAM_KEY_PROTOCOL << ", Value = " << PARAM_VALUE_PROTOCOL_T0T1);
		this->m_settings.insert(std::pair<std::string, std::string>(PARAM_KEY_SHARE_MODE, PARAM_VALUE_PROTOCOL_T0T1));
		break;
	}

	// DISPOSITION 
	switch (this->m_disposition)
	{
	case SCARD_LEAVE_CARD:
		DBG_INFO_MSG("Key = " << PARAM_KEY_DISPOSITION << ", Value = " << PARAM_VALUE_DISPOSITION_LEAVE);
		this->m_settings.insert(std::pair<std::string, std::string>(PARAM_KEY_DISPOSITION, PARAM_VALUE_DISPOSITION_LEAVE));
		break;
	case SCARD_RESET_CARD:
		DBG_INFO_MSG("Key = " << PARAM_KEY_DISPOSITION << ", Value = " << PARAM_VALUE_DISPOSITION_RESET);
		this->m_settings.insert(std::pair<std::string, std::string>(PARAM_KEY_DISPOSITION, PARAM_VALUE_DISPOSITION_RESET));
		break;
	case SCARD_UNPOWER_CARD:
		DBG_INFO_MSG("Key = " << PARAM_KEY_DISPOSITION << ", Value = " << PARAM_VALUE_DISPOSITION_UNPOWER);
		this->m_settings.insert(std::pair<std::string, std::string>(PARAM_KEY_DISPOSITION, PARAM_VALUE_DISPOSITION_UNPOWER));
		break;
	case SCARD_EJECT_CARD:
		DBG_INFO_MSG("Key = " << PARAM_KEY_DISPOSITION << ", Value = " << PARAM_VALUE_DISPOSITION_EJECT);
		this->m_settings.insert(std::pair<std::string, std::string>(PARAM_KEY_DISPOSITION, PARAM_VALUE_DISPOSITION_EJECT));
		break;
	}

	return &this->m_settings;
}

/*!
 * \fn ExecutionStatus PcscReader::setAParameter(std::string inKey, std::string inValue)
 *
 * \brief Sets a single parameter
 *
 * \param inKey   The in key.
 * \param inValue The in value.
 *
 * \return The ExecutionStatus.
 */

ExecutionStatus PcscReader::setAParameter(std::string inKey, std::string inValue)
{
	DBG_TRACE_CALL();
	ExecutionStatus status = EXECUTION_OK;

	DBG_INFO_MSG("Key = " << inKey << ", Value = " << inValue);

	if (inKey == PARAM_KEY_SHARE_MODE)
	{
		// SHARE MODE
		if (inValue == PARAM_VALUE_SHARE_SHARED)
		{
			this->m_share_mode = SCARD_SHARE_SHARED;
		}
		else if (inValue == PARAM_VALUE_SHARE_EXCLUSIVE)
		{
			this->m_share_mode = SCARD_SHARE_EXCLUSIVE;
		}
		else if (inValue == PARAM_VALUE_SHARE_DIRECT)
		{
			this->m_share_mode = SCARD_SHARE_DIRECT;
		}
		else
		{
			status = INCONSISTENT_PARAMETER_VALUE_ERROR;
		}
	}
	else if (inKey == PARAM_KEY_PROTOCOL) {
		// PROTOCOL
		if (inValue == PARAM_VALUE_PROTOCOL_T0)
		{
			this->m_protocol = SCARD_PROTOCOL_T0;
		}
		else if (inValue == PARAM_VALUE_PROTOCOL_T1)
		{
			this->m_protocol = SCARD_PROTOCOL_T1;
		}
		else if (inValue == PARAM_VALUE_PROTOCOL_T0T1)
		{
			this->m_protocol = SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1;
		}
		else
		{
			status = INCONSISTENT_PARAMETER_VALUE_ERROR;
		}
	}
	else if (inKey == PARAM_KEY_DISPOSITION) {
		// DISPOSITION
		if (inValue == PARAM_VALUE_DISPOSITION_LEAVE)
		{
			this->m_protocol = SCARD_LEAVE_CARD;
		}
		else if (inValue == PARAM_VALUE_DISPOSITION_RESET)
		{
			this->m_protocol = SCARD_RESET_CARD;
		}
		else if (inValue == PARAM_VALUE_DISPOSITION_UNPOWER)
		{
			this->m_protocol = SCARD_UNPOWER_CARD;
		}
		else if (inValue == PARAM_VALUE_DISPOSITION_EJECT)
		{
			this->m_protocol = SCARD_EJECT_CARD;
		}
		else
		{
			status = INCONSISTENT_PARAMETER_VALUE_ERROR;
		}
	}
	{
		status = WRONG_PARAMETER_KEY_ERROR;
	}
	return status;
}

/*!
 * \fn ExecutionStatus PcscReader::setParameters(std::map<std::string, std::string>* inSettings)
 *
 * \brief Sets the parameters
 *
 * \param [in,out] inSettings a map containing a list of key/value pairs.
 *
 * \return The ExecutionStatus.
 */

ExecutionStatus PcscReader::setParameters(std::map<std::string, std::string>* inSettings)
{
	DBG_TRACE_CALL();
	ExecutionStatus status;

	std::map<std::string, std::string>::iterator configIter = inSettings->begin();

	while (configIter != inSettings->end()) {
		status = this->setAParameter(configIter->first, configIter->second);
		if (status != EXECUTION_OK)
		{
			// exit loop on error
			break;
		}
		configIter++;
	}

	return status;
}

/*!
 * \fn ExecutionStatus PcscReader::attachObserver(ReaderObserver *callBack)
 *
 * \brief Attach an observer.
 * 		  If not already running, the card presence monitoring thread is started.
 *
 * \param [in,out] callBack If non-null, the call back.
 *
 * \return The ExecutionStatus.
 */

ExecutionStatus PcscReader::attachObserver(ReaderObserver *callBack)
{
	DBG_TRACE_CALL();
	ExecutionStatus status = EXECUTION_OK;

	/// Start monitoring thread if needed
	if (this->m_monitoring_is_running == false)
	{
		this->m_monitor = std::thread(&PcscReader::cardPresenceMonitoringThread, this);
		this->m_monitoring_is_running = true;
	}

	this->m_observers_list.push_back(callBack);

	return status;
}

/*!
 * \fn ExecutionStatus PcscReader::detachObserver(ReaderObserver *callBack)
 *
 * \brief Detach observer. To be called before exit the program.
 *
 * \param [in,out] callBack If non-null, the call back.
 *
 * \return The ExecutionStatus.
 */

ExecutionStatus PcscReader::detachObserver(ReaderObserver *callBack)
{
	DBG_TRACE_CALL();

	/// Remove the observer from the list
	this->m_observers_list.remove(callBack);

	/// Stop the monitoring thread if the list is empty
	if (this->m_observers_list.size() == 0)
	{
		this->m_monitoring_is_running = false;
	}

	return EXECUTION_OK;
}

/*!
 * \fn void PcscReader::notifyObservers(void *event)
 *
 * \brief Notifies the obverservers
 *
 * \param [in,out] event If non-null, the event.
 */

void PcscReader::notifyObservers(keyple::containers::ReaderEvent event)
{
	DBG_TRACE_CALL();

	for (std::list<ReaderObserver*>::iterator  iterator_observer = m_observers_list.begin(), end = m_observers_list.end();
		iterator_observer != end;
		++iterator_observer) {
		ReaderObserver* observer = *iterator_observer;
		observer->notify(event);
	}

	return;
}

/*!
 * \fn ExecutionStatus PcscReader::setName(std::string name)
 *
 * \brief Sets a name
 *
 * \param name The name.
 *
 * \return The ExecutionStatus.
 */

ExecutionStatus PcscReader::setName(std::string name)
{
	DBG_TRACE_CALL();
	ExecutionStatus status;
	if (name.length() <= sizeof(this->m_name))
	{
		strcpy(this->m_name, name.c_str());
		status = EXECUTION_OK;
	}
	else
	{
		status = PLUGIN_SPECIFIC_ERROR;
	}
	return status;
}

/*!
 * \fn ExecutionStatus PcscReader::setContext(SCARDCONTEXT context)
 *
 * \brief Sets the PC/SC context
 *
 * \param context The context.
 *
 * \return The ExecutionStatus.
 */

ExecutionStatus PcscReader::setContext(SCARDCONTEXT context)
{
	DBG_TRACE_CALL();
	this->m_context = context;
	return EXECUTION_OK;
}

/*!
 * \fn void PcscReader::cardPresenceMonitoringThread()
 *
 * \brief Card presence monitoring thread
		  Launched when the PcscReader object is created, this thread is 
		  dedicated to the card presence PC/SC event monitoring.
		  The thread is waiting for a change and updates the m_card_presence flag
		  accordingly.
		  (see SCardGetStatusChange).
 */

void PcscReader::cardPresenceMonitoringThread()
{
	DBG_TRACE_CALL();
	int32_t lReturn;
	SCARD_READERSTATE rgscState;

	while (this->m_monitoring_is_running == true && this->m_context == 0)
	{
		// wait for context establishment
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	memset(&rgscState, 0, sizeof(rgscState));
	
	rgscState.szReader = this->m_name;
	rgscState.dwCurrentState = SCARD_STATE_UNAWARE;

	if (m_monitoring_is_running == true && this->m_context != 0)
	{
		while (m_monitoring_is_running == true)
		{
			lReturn = SCardGetStatusChange(this->m_context,
										   INFINITE, // infinite wait
										   &rgscState,
										   1);
			rgscState.dwCurrentState = rgscState.dwEventState;
			if (lReturn == SCARD_S_SUCCESS)
			{
				if (rgscState.dwEventState & SCARD_STATE_PRESENT)
				{
					if(this->m_card == 0)
					{
						keyple::containers::ReaderEvent event(this, keyple::containers::ReaderEvent::SE_INSERTED);
						this->notifyObservers(event);
						DBG_INFO_MSG("Reader " << this->m_name << ": card is present. dwEventState = " << std::hex << rgscState.dwEventState);
					}
				}
				else
				{
					if (this->m_card != 0)
					{
						keyple::containers::ReaderEvent event(dynamic_cast<ObservableReader*>(this), keyple::containers::ReaderEvent::SE_REMOVAL);
						this->notifyObservers(event);
						DBG_INFO_MSG("Reader " << this->m_name << ": card is not present. dwEventState = " << std::hex << rgscState.dwEventState);
					}
					this->m_card = 0;
				}
			}
			else
			{
				DBG_ERROR_MSG("SCardGetStatusChange returned " << std::hex << lReturn);
				m_monitoring_is_running = false;
			}
		}
	}
}

/*!
 * \fn std::vector<uint8_t> PcscReader::scardTransmit(std::vector<uint8_t> apdu)
 *
 * \brief Scard transmit
 *
 * \exception std::system_error Raised when a system error condition occurs.
 *
 * \param apdu The apdu.
 *
 * \return A std::vector&lt;uint8_t&gt;
 */

std::vector<uint8_t> PcscReader::scardTransmit(std::vector<uint8_t> apdu)
{
	DBG_TRACE_CALL();
	uint32_t lReturn;
	uint8_t responseBuffer[256];
	uint32_t responseApduLength = sizeof(responseBuffer);
	SCARD_IO_REQUEST scioReq;

	scioReq.dwProtocol = this->m_active_protocol;
	scioReq.cbPciLength = sizeof(scioReq);

	DBG_DUMP_HEX_VECTOR("APDU: ", apdu);

	lReturn = SCardTransmit(this->m_card,
							&scioReq,
							reinterpret_cast<uint8_t*>(apdu.data()),
							apdu.size(),
							NULL,
							responseBuffer,
							(unsigned long*)&responseApduLength);

	if (lReturn == SCARD_S_SUCCESS)
	{
		/// Put response in responseApdu vector
		std::vector<uint8_t> responseApdu(responseBuffer, responseBuffer + responseApduLength);

		DBG_DUMP_HEX_VECTOR("RESPONSE: ", apdu);

		return responseApdu;
	}
	else
	{
		throw(std::system_error((int)lReturn, std::system_category()));
	}
}
