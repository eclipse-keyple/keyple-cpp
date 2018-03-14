/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

/*!
 * \file interfaces\ConfigurableReader.hpp.
 *
 * \brief Declares the ConfigurableReader class
 */

#ifndef KEYPLE_SEPROXY_CONFIGURABLE_READER
#define KEYPLE_SEPROXY_CONFIGURABLE_READER

#include <map>
#include <string>
#include <errno.h>

#include "ProxyReader.hpp"
#include "ExecutionStatus.hpp"

namespace keyple {
    namespace seproxy {

        /*!
		 * \class ConfigurableReader
		 *
		 * \brief The abstract class ConfigurableReader. This object is optionally proposed by plugins for readers
		 * 		  supporting the settings of proprietary parameters.
		 */

        class ConfigurableReader : virtual public ProxyReader {
          public:
            /*!
			 * \fn virtual std::map<std::string, std::string>* ConfigurableReader::getParameters() = 0;
			 *
			 * \brief Get the parameters/settings
			 *
			 * \return Null if it fails, else the parameters.
			 */

            virtual std::map<std::string, std::string> *getParameters() = 0;

            /*!
			 * \fn virtual ExecutionStatus ConfigurableReader::setAParameter(std::string inKey, std::string inValue) = 0;
			 *
			 * \brief Allows to define a single proprietary setting for a plugin (contactless protocols polling sequence,
			 * 		  baud rate, … etc.).
			 *
			 * \param inKey   The in key.
			 * \param inValue The in value.
			 *
			 * \return The ExecutionStatus.
			 */

            virtual ExecutionStatus setAParameter(std::string inKey, std::string inValue) = 0;

            /*!
			 * \fn virtual ExecutionStatus ConfigurableReader::setParameters(std::map<std::string, std::string>* inSettings) = 0;
			 *
			 * \brief Allows to define a set of proprietary settings for a plugin (contactless protocols polling sequence,
			 * 		  baud rate, … etc.).
			 *
			 * \param [in] inSettings If non-null, the in settings.
			 *
			 * \return The ExecutionStatus.
			 */

            virtual ExecutionStatus setParameters(std::map<std::string, std::string> *inSettings) = 0;
        };
    } // namespace seproxy
} // namespace keyple

#endif
