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

using AbstractSamResponseParser = org::eclipse::keyple::calypso::command::sam::AbstractSamResponseParser;
using AbstractSeSelectionRequest = org::eclipse::keyple::core::selection::AbstractSeSelectionRequest;
using ChannelState = org::eclipse::keyple::core::seproxy::ChannelState;
using SeResponse = org::eclipse::keyple::core::seproxy::message::SeResponse;

SamSelectionRequest::SamSelectionRequest(std::shared_ptr<SamSelector> samSelector, ChannelState channelState) : org::eclipse::keyple::core::selection::AbstractSeSelectionRequest(samSelector, channelState) {
}

std::shared_ptr<CalypsoSam> SamSelectionRequest::parse(std::shared_ptr<SeResponse> seResponse) {
    return std::make_shared<CalypsoSam>(seResponse, seSelector->getSeProtocol()->getTransmissionMode(), seSelector->getExtraInfo());
    }

std::shared_ptr<AbstractSamResponseParser> SamSelectionRequest::getCommandParser(std::shared_ptr<SeResponse> seResponse, int commandIndex) {
    /* not yet implemented in keyple-calypso */
    // TODO add a generic command parser
    throw std::make_shared<IllegalStateException>("No parsers available for this request.");
}

}
}
}
}
}
