/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

 /*!
 * \file interfaces\containers\ReaderEvent.hpp.
 *
 * \brief Declares the ReaderEvent class
 */

#ifndef KEYPLE_SEPROXY_READER_EVENT_H
#define KEYPLE_SEPROXY_READER_EVENT_H

namespace keyple {
	namespace seproxy {
		/// Forward declaration of keyple::seproxy::ObservableReader
		class ObservableReader;
	}
}

namespace keyple {
	namespace containers {

		/*!
		 * \class ReaderEvent
		 *
		 * \brief A container class to transport the elements used to notify an 
		 * 		  eventType by a specific ObservableReader to its registered 
		 * 		  ReaderObservers.
		 */

		class ReaderEvent {
		public:

			/*!
			 * \enum EventType
			 *
			 * \brief Type of events raised by the reader.
			 */

			enum EventType {
				/*! \brief when an IO error occurs. */
				IO_ERROR,
				/*! \brief when a SE is inserted. */
				SE_INSERTED,
				/*! \brief when a SE is remove. */
				SE_REMOVAL
			};

			/*!
			 * \fn ReaderEvent::ReaderEvent(keyple::seproxy::ObservableReader* reader, EventType eventType)
			 *
			 * \brief Constructor
			 *
			 * \param [in] reader    The reader identifier.
			 * \param 	   eventType The eventType.
			 */

			ReaderEvent(keyple::seproxy::ObservableReader* reader, EventType eventType)
			{
				this->m_reader = reader;
				this->m_event = eventType;
			}

			/*!
			 * \fn keyple::seproxy::ObservableReader* ReaderEvent::getReader()
			 *
			 * \brief Gets the reader object
			 *
			 * \return Null if it fails, else the reader.
			 */

			keyple::seproxy::ObservableReader* getReader() { return this->m_reader; }

			/*!
			 * \fn ReaderEvent::EventType ReaderEvent::getEventType()
			 *
			 * \brief Gets eventType type
			 *
			 * \return The eventType type.
			 */

			ReaderEvent::EventType getEventType() { return this->m_event; }

		private:
			/*! \brief The reader */
			keyple::seproxy::ObservableReader* m_reader;
			/*! \brief The eventType */
			EventType m_event;
		};
	} // of namespace seproxy
} // of namespace keyple

#endif
