/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

/*!
 * \file interfaces\containers\ApduResponse.hpp.
 *
 * \brief Declares the ApduResponse class
 */

#ifndef KEYPLE_SEPROXY_APDURESPONSE_H
#define KEYPLE_SEPROXY_APDURESPONSE_H

#include <stdint.h>
#include <vector>

namespace keyple {
    namespace containers {

        /*!
		 * \class ApduResponse
		 *
		 * \brief A container class for a single APDU command response
		 */

        class ApduResponse {
          public:
            /*!
			 * \fn ApduResponse::ApduResponse()
			 *
			 * \brief Default constructor
			 */

            ApduResponse()
            {
            }

            /*!
			 * \fn ApduResponse::ApduResponse(std::vector<uint8_t> inApdu, bool inSuccessful)
			 *
			 * \brief Build the APDU response
			 *
			 * \param inApdu	   The response apdu.
			 * \param inSuccessful True if the operation was a success, false if it failed.
			 */

            ApduResponse(std::vector<uint8_t> inApdu, bool inSuccessful)
            {
                this->apdu       = inApdu;
                this->successful = inSuccessful;
            }

            /*!
			 * \fn ApduResponse::~ApduResponse();
			 *
			 * \brief Destructor
			 */

            ~ApduResponse()
            {
            }

            /*!
			 * \fn std::vector<uint8_t> ApduResponse::getBytes(); 
			 *
			 * \brief return the bytes of the APDU response.
			 *
			 * \return The bytes vector.
			 */

            std::vector<uint8_t> getBytes()
            {
                return apdu;
            }

            /*!
			 * \fn bool ApduResponse::isSuccessful();
			 *
			 * \brief @return status
			 *
			 * \return True if successful, false if not.
			 */

            bool isSuccessful()
            {
                return successful;
            }

          private:
            /*!
			 * \brief an array of the bytes of an APDU response (none structured, including the dataOut field
			 * and the status of the command).
			 */

            std::vector<uint8_t> apdu;

            /*! \brief the success result of the processed APDU command. */
            bool successful;
        };
    } // namespace containers
} // namespace keyple

#endif
