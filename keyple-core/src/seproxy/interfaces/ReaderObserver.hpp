/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

 /*!
 * \file interfaces\ReaderObserver.hpp.
 *
 * \brief Declares the ReaderObserver class
 */


#ifndef KEYPLE_SEPROXY_READER_OBSERVER_H
#define KEYPLE_SEPROXY_READER_OBSERVER_H


#include "ReaderEvent.hpp"
#include "ExecutionStatus.hpp"

namespace keyple {
	namespace seproxy {
	
		/*!
		 * \class ReaderObserver
		 *
		 * \brief ReaderObserver abstract class .
		 */

		class ReaderObserver {
		public:

			/*!
			 * \fn virtual ExecutionStatus ReaderObserver::notify(keyple::containers::ReaderEvent event) = 0;
			 *
			 * \brief This method is called when information about an Reader which was previously requested using
			 * 		  an asynchronous interface becomes available.
			 *
			 * \param event The event.
			 *
			 * \return The ExecutionStatus.
			 */

			virtual ExecutionStatus notify(keyple::containers::ReaderEvent event) = 0;
		};
	}
}

#endif