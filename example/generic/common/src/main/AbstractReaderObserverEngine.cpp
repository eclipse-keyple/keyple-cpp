/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

/* Examples */
#include "AbstractReaderObserverEngine.h"

/* Core */
#include "ReaderEvent_Import.h"
#include "AbstractDefaultSelectionsResponse.h"

namespace keyple {
namespace example {
namespace generic {
namespace common {

using namespace keyple::core::seproxy::event;

void AbstractReaderObserverEngine::update(std::shared_ptr<ReaderEvent> event)
{
    logger->debug("update\n");

    if (event->getEventType() != ReaderEvent::EventType::SE_INSERTED &&
	logger->isInfoEnabled())
    {
	logger->info("%s\n", event->getReaderName().c_str());
	logger->info("Start the processing of the SE...\n");
    }

    ReaderEvent::EventType eventType = event->getEventType();
    if (eventType == ReaderEvent::EventType::SE_INSERTED)
    {
	currentlyProcessingSe = true;
	processSeInsertion(); // optional, to process alternative AID selection
	currentlyProcessingSe = false;
    }
    else if (eventType == ReaderEvent::EventType::SE_MATCHED)
    {
	currentlyProcessingSe = true;
	processSeMatch(
	    event->getDefaultSelectionsResponse()); // to process the selected
	// application
	currentlyProcessingSe = false;
    }
    else if (eventType == ReaderEvent::EventType::SE_REMOVAL)
    {
	if (currentlyProcessingSe)
	{
	    processUnexpectedSeRemoval(); // to clean current SE processing
	    logger->error("Unexpected SE Removal\n");
	}
	else
	{
	    processSeRemoval();
	    if (logger->isInfoEnabled())
	    {
		logger->info("Waiting for a SE...\n");
	    }
	}
	currentlyProcessingSe = false;
    }
    else
    {

	logger->error("IO Error\n");
    }
}

}
}
}
}

