/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

/*!
 * \file interfaces\ObservableReader.hpp.
 *
 * \brief Declares the observable reader class
 */

#ifndef KEYPLE_SEPROXY_OBSERVABLE_READER_H
#define KEYPLE_SEPROXY_OBSERVABLE_READER_H

#include <map>
#include <string>
#include <errno.h>

#include "ProxyReader.hpp"
#include "ReaderEvent.hpp"
#include "ReaderObserver.hpp"
#include "ExecutionStatus.hpp"

namespace keyple {
    namespace seproxy {

        /*!
		 * \class ObservableReader
		 *
		 * \brief Abstract class  ObservableReader. In order to notify a client application in case of specific
		 * 		  reader events, the SE Proxy implements the 'Observer' design pattern. The ObservableReader object
		 * 		  is optionally proposed by plugins for readers able to notify events in case of IO Error, SE
		 * 		  Insertion or removal.
		 */

        class ObservableReader : virtual public ProxyReader {
          public:
            /*!
			 * \fn virtual ExecutionStatus ObservableReader::attachObserver(keyple::seproxy::ReaderObserver *callBack) = 0;
			 *
			 * \brief This method shall be called only from a terminal application implementing ObservableReader
			 * 		  adds the ReaderObserver to the list of registered ReaderObserver for the selected
			 * 		  ObservableReader.
			 *
			 * \param [in] callBack If non-null, the call back.
			 *
			 * \return The ExecutionStatus.
			 */

            virtual ExecutionStatus attachObserver(keyple::seproxy::ReaderObserver *callBack) = 0;

            /*!
			 * \fn virtual ExecutionStatus ObservableReader::detachObserver(keyple::seproxy::ReaderObserver *callBack) = 0;
			 *
			 * \brief This method shall be called only from a terminal application implementing ObservableReader
			 * 		  Removes the ReaderObserver from the list of registered ReaderObserver for the selected ObservableReader.
			 *
			 * \param [in] callBack If non-null, the call back.
			 *
			 * \return The ExecutionStatus.
			 */

            virtual ExecutionStatus detachObserver(keyple::seproxy::ReaderObserver *callBack) = 0;

            /*!
			 * \fn virtual void ObservableReader::notifyObservers(keyple::containers::ReaderEvent event) = 0;
			 *
			 * \brief This method shall be called only from a SE Proxy plugin by a reader implementing ObservableReader
			 * 		  Push a ReaderEvent of the selected ObservableReader to its registered ReaderObserver.
			 *
			 * \param event The event.
			 */

            virtual void notifyObservers(keyple::containers::ReaderEvent event) = 0;
        };
    } // namespace seproxy
} // namespace keyple

#endif
