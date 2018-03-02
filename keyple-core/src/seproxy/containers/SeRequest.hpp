/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

/*!
 * \file interfaces\containers\SeRequest.hpp.
 *
 * \brief Declares the SeRequest class
 */

#ifndef KEYPLE_SEPROXY_SEREQUEST_H
#define KEYPLE_SEPROXY_SEREQUEST_H

#include <list>
#include <vector>
#include <stdint.h>

#include "ApduRequest.hpp"

namespace keyple {
	namespace containers {

		/*!
		 * \class SeRequest
		 *
		 * \brief A container class to aggregate the elements of a request to a local or remote SE Reader
		 */

		class SeRequest {
		public:

			/*!
			 * \fn SeRequest::SeRequest(std::vector<uint8_t> inAidToSelect, std::list<ApduRequest> inApduRequests, bool inKeepChannelOpen)
			 *
			 * \brief Constructor
			 *
			 * \param 	   inAidToSelect	 The first parameter.
			 * \param [in] inApduRequests    List of APDU requests.
			 * \param 	   inKeepChannelOpen True to in keep channel open.
			 */

			SeRequest(std::vector<uint8_t> inAidToSelect, std::list<ApduRequest> inApduRequests, bool inKeepChannelOpen) {
				aidToSelect = inAidToSelect;
				apduRequests = inApduRequests;
				keepChannelOpen = inKeepChannelOpen;
			}

			/*!
			 * \fn SeRequest::~SeRequest();
			 *
			 * \brief Destructor
			 */

			~SeRequest() {}

			/*!
			 * \fn std::vector<uint8_t> SeRequest::getAidToSelect();
			 *
			 * \brief @return the current AID set to select
			 *
			 * \return The aid to select.
			 */

			std::vector<uint8_t> getAidToSelect() { return aidToSelect; }

			/*!
			 * \fn std::list<ApduRequest>* SeRequest::getApduRequests();
			 *
			 * \brief @return the group of APDUs to be transmitted to the SE application for this instance of
			 * SeRequest
			 *
			 * \return Null if it fails, else the apdu requests.
			 */

			std::list<ApduRequest> getApduRequests() { return apduRequests; }

			/*!
			 * \fn bool SeRequest::askKeepChannelOpen();
			 *
			 * \brief @return whether or not to keep the channel open after exchanging the APDU requests with
			 * the SE
			 *
			 * \return True if channel need to be left open.
			 */

			bool askKeepChannelOpen() { return keepChannelOpen; }

		private:
			/*! \brief AID’s vector of bytes of the SE application to select. 
			 *         In case the SE application is currently notselected, a logical channel is established 
			 *         and the corresponding SE application is selected by the SE reader, otherwise keep 
			 *         the current channel.
			 *         Could be missing when operating SE which don’t support the Select Application command
			 *         (as it is the case for CSM).
			 */
			std::vector<uint8_t>  aidToSelect;

			/*! \brief The final logical channel status: if true, the SE reader keep active the logical channel of 
			 *         the SE application after processing the group of APDU commands. If false, the SE reader will
			 *         close the logical channel of the SE application after processing the group of APDU commands 
			 *         (i.e. after the receipt of the last APDU response).
			 */
			bool keepChannelOpen;

			/*! \brief A group of APDUCommand to operate on the selected SE application by the SE reader. */
			std::list<ApduRequest> apduRequests;
		};
	} // of namespace seproxy
} // of namespace keyplesdk

#endif
