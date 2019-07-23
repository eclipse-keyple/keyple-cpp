#include "SamSelectionRequest.h"
#include "SeSelector.h"
#include "SeProtocol.h"
#include "CalypsoSam.h"
#include "SeResponse.h"
#include "AbstractSamResponseParser.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace transaction {

using namespace org::eclipse::keyple::calypso::command::sam;
using namespace org::eclipse::keyple::core::selection;
using namespace org::eclipse::keyple::core::seproxy;
using namespace org::eclipse::keyple::core::seproxy::message;

SamSelectionRequest::SamSelectionRequest(std::shared_ptr<SamSelector> samSelector, ChannelState channelState)
: AbstractSeSelectionRequest(samSelector, channelState)
{
}

std::shared_ptr<AbstractMatchingSe> SamSelectionRequest::parse(std::shared_ptr<SeResponse> seResponse)
{
    return std::dynamic_pointer_cast<AbstractMatchingSe>(
              std::make_shared<CalypsoSam>(seResponse, seSelector->getSeProtocol().getTransmissionMode(), seSelector->getExtraInfo()));
}

std::shared_ptr<AbstractApduResponseParser> SamSelectionRequest::getCommandParser(std::shared_ptr<SeResponse> seResponse, int commandIndex)
{
    (void)seResponse;
    (void)commandIndex;

    /* not yet implemented in keyple-calypso */
    // TODO add a generic command parser
    throw std::make_shared<IllegalStateException>("No parsers available for this request.");
}

}
}
}
}
}
