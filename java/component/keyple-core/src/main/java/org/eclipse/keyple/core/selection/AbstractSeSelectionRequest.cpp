#include "AbstractSeSelectionRequest.h"
#include "SeSelector.h"
#include "ApduRequest.h"
#include "SeRequest.h"
#include "AbstractApduResponseParser.h"
#include "SeResponse.h"
#include "AbstractMatchingSe.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace selection {

using AbstractApduResponseParser = org::eclipse::keyple::core::command::AbstractApduResponseParser;
using ChannelState               = org::eclipse::keyple::core::seproxy::ChannelState;
using SeSelector                 = org::eclipse::keyple::core::seproxy::SeSelector;
using ApduRequest                = org::eclipse::keyple::core::seproxy::message::ApduRequest;
using SeRequest                  = org::eclipse::keyple::core::seproxy::message::SeRequest;
using SeResponse                 = org::eclipse::keyple::core::seproxy::message::SeResponse;

AbstractSeSelectionRequest::AbstractSeSelectionRequest(
                                         std::shared_ptr<SeSelector> seSelector,
                                         ChannelState channelState)
: seSelector(seSelector), channelState(channelState)
{
}

std::shared_ptr<SeRequest> AbstractSeSelectionRequest::getSelectionRequest()
{
    return std::make_shared<SeRequest>(seSelector, seSelectionApduRequestList, channelState);
}

std::shared_ptr<SeSelector> AbstractSeSelectionRequest::getSeSelector()
{
    return seSelector;
}

void AbstractSeSelectionRequest::addApduRequest(std::shared_ptr<ApduRequest> apduRequest)
{
    seSelectionApduRequestList.push_back(apduRequest);
}

std::shared_ptr<AbstractApduResponseParser>
AbstractSeSelectionRequest::getCommandParser(
                                         std::shared_ptr<SeResponse> seResponse,
                                         int commandIndex)
{
    (void)seResponse;
    (void)commandIndex;

    /* not yet implemented in keyple-core */
    // TODO add a generic command parser
    throw std::make_shared<IllegalStateException>("No parsers available for this request.");
}

}
}
}
}
}
