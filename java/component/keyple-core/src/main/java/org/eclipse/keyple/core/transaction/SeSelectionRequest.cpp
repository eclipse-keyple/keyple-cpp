#include "SeSelectionRequest.h"
#include "SeSelector.h"
#include "ApduRequest.h"
#include "SeProtocol.h"
#include "SeRequest.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace transaction {

using ChannelState = org::eclipse::keyple::core::seproxy::ChannelState;
using SeSelector   = org::eclipse::keyple::core::seproxy::SeSelector;
using ApduRequest  = org::eclipse::keyple::core::seproxy::message::ApduRequest;
using SeRequest    = org::eclipse::keyple::core::seproxy::message::SeRequest;
using SeProtocol   = org::eclipse::keyple::core::seproxy::protocol::SeProtocol;

SeSelectionRequest::SeSelectionRequest(std::shared_ptr<SeSelector> seSelector, ChannelState channelState, std::shared_ptr<SeProtocol> protocolFlag)
: matchingClass(std::type_index(typeid(MatchingSe))), selectionClass(std::type_index(typeid(SeSelector))), channelState(channelState), protocolFlag(protocolFlag)
{
    this->seSelector = seSelector;
    /* Alex: removed 'if (logger-isTraceEnabled())', that check will be done in the trace function already */
    logger->trace("constructor\n");
}

std::shared_ptr<SeRequest> SeSelectionRequest::getSelectionRequest() {
    std::shared_ptr<SeRequest> seSelectionRequest = nullptr;
    seSelectionRequest = std::make_shared<SeRequest>(seSelector, seSelectionApduRequestList, channelState, protocolFlag);
    return seSelectionRequest;
}

std::shared_ptr<SeSelector> SeSelectionRequest::getSeSelector() {
    return seSelector;
}

void SeSelectionRequest::setMatchingClass(const std::type_info& matchingClass) {
    this->matchingClass = matchingClass;
}

void SeSelectionRequest::setSelectionClass(const std::type_info& selectionClass) {
    this->selectionClass = selectionClass;
}

std::type_index& SeSelectionRequest::getSelectionClass() {
    return selectionClass;
}

std::type_index& SeSelectionRequest::getMatchingClass() {
    return matchingClass;
}

void SeSelectionRequest::addApduRequest(std::shared_ptr<ApduRequest> apduRequest) {
    seSelectionApduRequestList.push_back(apduRequest);
}

std::string SeSelectionRequest::toString() {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    std::string request = "SeSelectionRequest: SELECTION_CLASS = ";
    request += selectionClass.name();
    request += ", MATCHING_CLASS = ";
    request += matchingClass.name();
    return request;
}

}
}
}
}
}
