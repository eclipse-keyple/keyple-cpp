/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

/*!
 * \file interfaces\SeProxyService.hpp.
 *
 * \brief Declares the SeProxyService class
 */

#ifndef KEYPLE_SEPROXY_SEPROXYSERVICE_H
#define KEYPLE_SEPROXY_SEPROXYSERVICE_H

#include "SeProxyService.hpp"
#include "ReadersPlugin.hpp"

#include <string>
#include <list>

namespace keyple {
    namespace seproxy {

        /*!
		 * \class SeProxyService
		 *
		 * \brief SeProxyService class
		 */

        class SeProxyService {
          public:
            /*!
			 * \fn virtual uint32_t SeProxyService::getVersion() = 0;
			 *
			 * \brief Returns the service version
			 *
			 * \return The version.
			 */

            virtual uint32_t getVersion() = 0;

            /*!
			 * \fn virtual void SeProxyService::setPlugins(std::list<ReadersPlugin*> plugins) = 0;
			 *
			 * \brief Sets the plugins.
			 *
			 * \param [in] plugins If non-null, the plugins.
			 */

            virtual void setPlugins(std::list<ReadersPlugin *> plugins) = 0;

            /*!
			 * \fn virtual std::list<ReadersPlugin*> SeProxyService::getPlugins() = 0;
			 *
			 * \brief Gets the plugins.
			 *
			 * \return Null if it fails, else the plugins.
			 */

            virtual std::list<ReadersPlugin *> getPlugins() = 0;
        };
    } // namespace seproxy
} // namespace keyple
#endif
