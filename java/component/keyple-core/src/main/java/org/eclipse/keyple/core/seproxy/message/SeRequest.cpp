#include "SeRequest.h"
#include "SeSelector.h"
#include "ApduRequest.h"
#include "SeProtocol.h"
#include "Pattern.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using ChannelState = org::eclipse::keyple::core::seproxy::ChannelState;
using SeSelector   = org::eclipse::keyple::core::seproxy::SeSelector;
using Protocol     = org::eclipse::keyple::core::seproxy::protocol::Protocol;
using SeProtocol   = org::eclipse::keyple::core::seproxy::protocol::SeProtocol;

SeRequest::SeRequest(std::shared_ptr<SeSelector> seSelector, std::vector<std::shared_ptr<ApduRequest>> &apduRequests, ChannelState channelState, std::shared_ptr<SeProtocol> protocolFlag) : seSelector(seSelector) {
    this->apduRequests = apduRequests;
    this->channelState = channelState;
    this->protocolFlag = protocolFlag;
}

SeRequest::SeRequest(std::vector<std::shared_ptr<ApduRequest>> &apduRequests, ChannelState channelState) {
    this->seSelector = nullptr;
    this->apduRequests = apduRequests;
    this->channelState = channelState;
    this->protocolFlag = std::dynamic_pointer_cast<SeProtocol>(std::make_shared<Protocol>(Protocol::ANY));
}

std::shared_ptr<SeSelector> SeRequest::getSeSelector() {
    return seSelector;
}

std::vector<std::shared_ptr<ApduRequest>> SeRequest::getApduRequests() {
    return apduRequests;
}

bool SeRequest::isKeepChannelOpen() {
    return channelState == ChannelState::KEEP_OPEN;
}

std::shared_ptr<SeProtocol> SeRequest::getProtocolFlag() {
    return protocolFlag;
}

std::string SeRequest::toString() {
    return StringHelper::formatSimple("SeRequest:{REQUESTS = %s, SELECTOR = %s, KEEPCHANNELOPEN = %s}", "to fix!" /*etApduRequests()*/, getSeSelector(), "to fix!" /*channelState*/);
}

}
}
}
}
}
}
