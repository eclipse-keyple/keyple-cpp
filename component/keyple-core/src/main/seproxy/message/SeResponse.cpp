#include "SeResponse.h"
#include "SelectionStatus.h"
#include "ApduResponse.h"
#include "ByteArrayUtil.h"
#include "stringhelper.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using ByteArrayUtil = org::eclipse::keyple::core::util::ByteArrayUtil;

SeResponse::SeResponse(bool logicalChannelIsOpen, bool channelPreviouslyOpen, std::shared_ptr<SelectionStatus> selectionStatus,
                       std::vector<std::shared_ptr<ApduResponse>> &apduResponses)
: logicalChannelIsOpen(logicalChannelIsOpen), selectionStatus(selectionStatus)
{
    this->channelPreviouslyOpen = channelPreviouslyOpen;
    this->apduResponses = apduResponses;
}

bool SeResponse::wasChannelPreviouslyOpen()
{
    return channelPreviouslyOpen;
}

bool SeResponse::isLogicalChannelOpen()
{
    return logicalChannelIsOpen;
}

std::shared_ptr<SelectionStatus> SeResponse::getSelectionStatus()
{
    return this->selectionStatus;
}

std::vector<std::shared_ptr<ApduResponse>> SeResponse::getApduResponses()
{
    return apduResponses;
}

std::string SeResponse::toString()
{
    /*
        * getAtr() can return null, we must check it to avoid the call to getBytes() that would
        * raise an exception. In case of a null value, String.format prints "null" in the string,
        * the same is done here.
        */
    std::string string;
    if (selectionStatus != nullptr) {
        string = StringHelper::formatSimple("SeResponse:{RESPONSES = %s, ATR = %s, FCI = %s, HASMATCHED = %b CHANNELWASOPEN = %b}", "to fix!" /*getApduResponses()*/, selectionStatus->getAtr()->getBytes().empty() ? "null" : ByteArrayUtil::toHex(selectionStatus->getAtr()->getBytes()), ByteArrayUtil::toHex(selectionStatus->getFci()->getBytes()), selectionStatus->hasMatched(), wasChannelPreviouslyOpen());
    }
    else {
        string = StringHelper::formatSimple("SeResponse:{RESPONSES = %s, ATR = null, FCI = null, HASMATCHED = false CHANNELWASOPEN = %b}", "to fix!" /*getApduResponses()*/, wasChannelPreviouslyOpen());
    }
    return string;
}

bool SeResponse::equals(std::shared_ptr<void> o)
{
    if (o == shared_from_this()) {
        return true;
    }
    if (!(std::static_pointer_cast<SeResponse>(o) != nullptr)) {
        return false;
    }

    std::shared_ptr<SeResponse> seResponse = std::static_pointer_cast<SeResponse>(o);

    return seResponse->getSelectionStatus() == (selectionStatus) &&
           (seResponse->getApduResponses().empty() ? apduResponses.empty() : seResponse->getApduResponses() == (apduResponses)) &&
           seResponse->isLogicalChannelOpen() == logicalChannelIsOpen && seResponse->wasChannelPreviouslyOpen() == channelPreviouslyOpen;
}

int SeResponse::hashCode()
{
    int hash = 17;
    hash = 31 * hash + (selectionStatus->getAtr() == nullptr ? 0 : selectionStatus->getAtr()->hashCode());
    hash = 7 * hash + (apduResponses.empty() ? 0 : 1);//this->apduResponses.hashCode());
    hash = 29 * hash + (this->channelPreviouslyOpen ? 1 : 0);
    hash = 37 * hash + (this->logicalChannelIsOpen ? 1 : 0);
    return hash;
}

void SeResponse::finalize()
{

}

}
}
}
}
}
}
