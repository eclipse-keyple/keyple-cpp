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
using SeProtocol   = org::eclipse::keyple::core::seproxy::protocol::SeProtocol;

SeRequest::SeRequest(std::shared_ptr<SeSelector> seSelector, std::vector<std::shared_ptr<ApduRequest>> &apduRequests, ChannelState channelState)
: seSelector(seSelector)
{
    this->apduRequests = apduRequests;
    this->channelState = channelState;
}

SeRequest::SeRequest(std::vector<std::shared_ptr<ApduRequest>> &apduRequests, ChannelState channelState)
: seSelector(nullptr)
{
    this->apduRequests = apduRequests;
    this->channelState = channelState;
}

std::shared_ptr<SeSelector> SeRequest::getSeSelector()
{
    return seSelector;
}

std::vector<std::shared_ptr<ApduRequest>> SeRequest::getApduRequests()
{
    return apduRequests;
}

bool SeRequest::isKeepChannelOpen()
{
    return channelState == ChannelState::KEEP_OPEN;
}

std::string SeRequest::toString()
{
    return StringHelper::formatSimple("SeRequest:{REQUESTS = %s, SELECTOR = %s, KEEPCHANNELOPEN = %s}", "to fix!" /*getApduRequests()*/, getSeSelector(), "to fix!" /*channelState*/);
}

}
}
}
}
}
}
