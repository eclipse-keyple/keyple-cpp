/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

 /*!
 * \file interfaces\ReadersPlugin.hpp.
 *
 * \brief Declares the ReadersPlugin class
 */

#ifndef KEYPLE_SEPROXY_READERS_PLUGIN_H
#define KEYPLE_SEPROXY_READERS_PLUGIN_H

#include "ProxyReader.hpp"

#include <string>
#include <list>

namespace keyple {
	namespace seproxy {
		/*!
		 * \class ReadersPlugin
		 *
		 * \brief ReadersPlugin abstract class
		 */

		class ReadersPlugin {
		public:

			/*!
			 * \fn virtual void ReadersPlugin::destroy() = 0;
			 *
			 * \brief Destroys this object
			 */

			virtual void destroy() = 0;

			/*!
			 * \fn virtual std::string ReadersPlugin::getName() = 0;
			 *
			 * \brief Returns the unique name of the readers’ plugin
			 *
			 * \return The name.
			 */

			virtual std::string getName() = 0;

			/*!
			 * \fn virtual std::list<ProxyReader*> ReadersPlugin::getReaders() = 0;
			 *
			 * \brief Returns a list of available connected SE readers for the plugin
			 *
			 * \return Null if it fails, else the readers.
			 */

			virtual std::list<ProxyReader*> getReaders() = 0;
		};
	}
}

#endif