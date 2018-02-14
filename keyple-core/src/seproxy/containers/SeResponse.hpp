/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

/*!
 * \file interfaces\containers\SeResponse.hpp.
 *
 * \brief Declares the SeResponse class
 */

#ifndef KEYPLE_SEPROXY_SERESPONSE_H
#define KEYPLE_SEPROXY_SERESPONSE_H

#include <list>
#include <stdint.h>

#include "ApduResponse.hpp"

namespace keyple {
	namespace containers {

		/*!
		 * \class SeResponse
		 *
		 * \brief A container class to aggregate the elements of a response from a local or remote SE Reader
		 */

		class SeResponse {
		public:

			/*!
			 * \fn SeResponse::SeResponse(bool inChannelPreviouslyOpen, ApduResponse inFci, std::list<ApduResponse>* inApduResponses, std::vector<uint8_t> inAtr)
			 *
			 * \brief Constructor
			 *
			 * \param 	   inChannelPreviouslyOpen True if channel previously open.
			 * \param 	   inFci				   The fci.
			 * \param [in] inApduResponses		   A list of APDU responses.
			 * \param 	   inAtr				   The SE ATR.
			 */

			SeResponse (bool inChannelPreviouslyOpen, ApduResponse inFci, std::list<ApduResponse> inApduResponses, std::vector<uint8_t> inAtr)
			{
				this->channelPreviouslyOpen = inChannelPreviouslyOpen;
				this->fci = inFci;
				this->apduResponses = inApduResponses;
				this->atr = inAtr;
			}

			/*!
			 * \fn SeResponse::~SeResponse()
			 *
			 * \brief Destructor
			 */

			~SeResponse() { }

			/*!
			 * \fn bool SeResponse::wasChannelPreviouslyOpen()
			 *
			 * \brief Returns the previous state of the logical channel.
			 *
			 * \return True if the channel is open.
			 */

			bool wasChannelPreviouslyOpen() { return this->channelPreviouslyOpen; }

			/*!
			 * \fn ApduResponse SeResponse::getFci()
			 *
			 * \brief Returns null or the FCI response if a channel was opened.
			 *
			 * \return The FCI.
			 */

			ApduResponse getFci() { return this->fci; }

			/*!
			 * \fn std::list<ApduResponse>* SeResponse::getApduResponses()
			 *
			 * \brief Return the group of APDUs responses returned by the SE application for this instance of
			 * 		  SeResponse
			 *
			 * \return Null if it fails, else the apdu responses.
			 */

			std::list<ApduResponse> getApduResponses() { return this->apduResponses; }

			/*!
			 * \fn std::vector<uint8_t> SeResponse::getAtr()
			 *
			 * \brief Return bytes of the Answer To Reset
			 *
			 * \return The atr.
			 */

			std::vector<uint8_t> getAtr() { return this->atr; }

		private:

			/*!
			 * \brief Defined as true by the SE reader in case a logical channel was already open with the
			 * 		  target SE application
			 */

			bool channelPreviouslyOpen;

			/*!
			 * \brief Present if channelPreviouslyOpen is false, contains the FCI response of the channel
			 * 		  opening
			 */

			ApduResponse fci;

			/*! \brief A group of ApduResponse returned by the selected SE application on the SE reader */
			std::list<ApduResponse> apduResponses;

			/*! \brief atr of SE */
			std::vector<uint8_t> atr;
		};

/************************************************************/
/* External declarations (package visibility)               */
/************************************************************/

/* Inline functions                                         */

	} // of namespace seproxy
} // of namespace keyple

/************************************************************
 End of SeResponse class header
 ************************************************************/

#endif
