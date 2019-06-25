#include "AbstractMatchingSe.h"
#include "SeResponse.h"
#include "SelectionStatus.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace selection {

using SeResponse       = org::eclipse::keyple::core::seproxy::message::SeResponse;
using SelectionStatus  = org::eclipse::keyple::core::seproxy::message::SelectionStatus;
using TransmissionMode = org::eclipse::keyple::core::seproxy::protocol::TransmissionMode;

AbstractMatchingSe::AbstractMatchingSe(
                                 std::shared_ptr<SeResponse> selectionResponse,
                                 TransmissionMode transmissionMode,
                                 const std::string &extraInfo)
: selectionResponse(selectionResponse), transmissionMode(transmissionMode),
  selectionExtraInfo(extraInfo)
{
    if (selectionResponse != nullptr) {
        this->selectionStatus = selectionResponse->getSelectionStatus();
    }
    else {
        this->selectionStatus.reset();
    }
}

bool AbstractMatchingSe::isSelected() {
    bool isSelected;
    if (selectionStatus != nullptr) {
        isSelected = selectionStatus->hasMatched() && selectionResponse->isLogicalChannelOpen();
    }
    else {
        isSelected = false;
    }
    return isSelected;
}

std::shared_ptr<SelectionStatus> AbstractMatchingSe::getSelectionStatus() {
    return selectionStatus;
}

TransmissionMode AbstractMatchingSe::getTransmissionMode() {
    return transmissionMode;
}

std::string AbstractMatchingSe::getSelectionExtraInfo() {
    return selectionExtraInfo;
}
}
}
}
}
}
