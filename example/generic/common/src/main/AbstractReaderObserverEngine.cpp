/* Examples */
#include "AbstractReaderObserverEngine.h"

/* Core */
#include "ReaderEvent_Import.h"
#include "AbstractDefaultSelectionsResponse.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace example {
namespace generic {
namespace common {

using namespace org::eclipse::keyple::core::seproxy::event;

void AbstractReaderObserverEngine::update(std::shared_ptr<ReaderEvent> event)
{
    logger->debug("update\n");

    if (event->getEventType() != ReaderEvent::EventType::SE_INSERTED &&
	logger->isInfoEnabled())
    {
	logger->info("%s\n", event->getReaderName());
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
}
}

