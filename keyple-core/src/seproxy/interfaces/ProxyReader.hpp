/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

/*!
 * \file interfaces\ProxyReader.hpp.
 *
 * \brief Declares the ProxyReader class
 */

#ifndef KEYPLE_SEPROXY_PROXY_READER_H
#define KEYPLE_SEPROXY_PROXY_READER_H

#include <memory>
#include <map>
#include <string>
#include <errno.h>

#include "ExecutionStatus.hpp"
#include "SeRequest.hpp"
#include "SeResponse.hpp"

namespace keyple {
    namespace seproxy {

        /*!
		 * \class ProxyReader
		 *
		 * \brief ProxyReader abstract class.
		 */

        class ProxyReader {
          public:
            /*!
			 * \fn virtual std::string ProxyReader::getName() = 0;
			 *
			 * \brief returns the ‘unique’ name of the SE reader for the selected plugin.
			 *
			 * \return The name.
			 */

            virtual std::string getName() = 0;

            /*!
			 * \fn virtual keyple::containers::SeResponse ProxyReader::transmit(keyple::containers::SeRequest* inSeApplicationRequest) = 0;
			 *
			 * \brief Transmits a request to a SE application and get back the corresponding SE response
			 * 		  The usage of this method is conditioned to the presence of a SE in the selected reader, this
			 * 		  method could also fail in case of IO error or wrong card state some reader’s exceptions (SE
			 * 		  missing, IO error, wrong card state, timeout) have to be caught during the processing of the
			 * 		  SE request transmission.
			 * 
			 * Manages:
			 * 		ChannelStateReaderError InvalidApduReaderError IOReaderError TimeoutReaderError
			 * 		UnexpectedReaderError
			 *
			 * \param [in] inSeApplicationRequest If non-null, the SE application request.
			 *
			 * \return A keyple::containers::SeResponse.
			 */

            virtual keyple::containers::SeResponse
            transmit(keyple::containers::SeRequest *inSeApplicationRequest) = 0;

            /*!
			 * \fn virtual bool ProxyReader::isSePresent() = 0;
			 *
			 * \brief returns the SE presence
			 *
			 * \return true if a Secure Element is present in the reader
			 */

            virtual bool isSePresent() = 0;
        };
    } // namespace seproxy
} // namespace keyple

#endif
