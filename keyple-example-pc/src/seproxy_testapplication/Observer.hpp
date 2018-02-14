/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#ifndef OBSERVER_R
#define OBSERVER_H

#include "ReaderObserver.hpp"

/*!
 * \class Observer
 *
 * \brief An observer class to receive ReadersPlugin events.
 */

class Observer : public keyple::seproxy::ReaderObserver
{
public:

	/*!
	 * \fn ExecutionStatus Observer::notify(keyple::containers::ReaderEvent event);
	 *
	 * \brief Notifies the given event
	 *
	 * \param event The event.
	 *
	 * \return The ExecutionStatus.
	 */

	ExecutionStatus notify(keyple::containers::ReaderEvent event);
};

#endif