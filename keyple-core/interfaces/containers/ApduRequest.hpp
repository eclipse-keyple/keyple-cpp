/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

/*!
 * \file interfaces\containers\ApduRequest.hpp.
 *
 * \brief Declares the ApduRequest class
 */

#ifndef KEYPLE_SEPROXY_APDUREQUEST_H
#define KEYPLE_SEPROXY_APDUREQUEST_H


namespace keyple {
	namespace containers {

		/*!
		 * \class ApduRequest
		 *
		 * \brief A container class for a single APDU command request
		 */

		class ApduRequest {
		public:

			/*!
			 * \fn ApduRequest::ApduRequest()
			 *
			 * \brief Default constructor
			 */

			ApduRequest() { }

			/*!
			 * \fn ApduRequest::ApduRequest(std::vector<uint8_t> inApdu, bool inCase4 = false)
			 *
			 * \param inApdu  The APDU bytes.
			 * \param inCase4 (Optional) True if in case 4.
			 */

			ApduRequest(std::vector<uint8_t> inApdu, bool inCase4 = false) {
				this->apdu = inApdu;
				this->case4 = inCase4;
			}

			/*!
			 * \fn ApduRequest::~ApduRequest()
			 *
			 * \brief Destructor
			 */

			~ApduRequest() {}

			/*!
			 * \fn std::vector<uint8_t> ApduRequest::getBytes()
			 *
			 * \brief Gets the APDU bytes
			 *
			 * \return The bytes.
			 */

			std::vector<uint8_t> getBytes() { return this->apdu;  }

			/*!
			 * \fn bool ApduRequest::isCase4()
			 *
			 * \brief @return case4 the case4 flag.
			 *
			 * \return True if case 4, false if not.
			 */

			bool isCase4() { return this->case4; }

		private:

			/*!
			 * \brief a ‘case 4’ flag in order to explicitly specify, if it’s expected that the APDU command
			 * returns data.
			 */

			bool case4;

			/*! \brief The apdu */
			std::vector<uint8_t> apdu;
		};
	} // of namespace seproxy
} // of namespace keyple

#endif
