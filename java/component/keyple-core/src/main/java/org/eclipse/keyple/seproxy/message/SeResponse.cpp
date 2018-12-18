#include "SeResponse.h"
#include "SelectionStatus.h"
#include "ApduResponse.h"
#include "../../util/ByteArrayUtils.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace message {
                    using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;

                    SeResponse::SeResponse(bool channelPreviouslyOpen, std::shared_ptr<SelectionStatus> selectionStatus, std::vector<std::shared_ptr<ApduResponse>> &apduResponses) throw(std::invalid_argument) : selectionStatus(selectionStatus) {
                        this->channelPreviouslyOpen = channelPreviouslyOpen;
                        this->apduResponses = apduResponses;
                    }

                    bool SeResponse::wasChannelPreviouslyOpen() {
                        return channelPreviouslyOpen;
                    }

                    std::shared_ptr<SelectionStatus> SeResponse::getSelectionStatus() {
                        return this->selectionStatus;
                    }

                    std::vector<std::shared_ptr<ApduResponse>> SeResponse::getApduResponses() {
                        return apduResponses;
                    }

                    std::string SeResponse::toString() {
                        /*
                         * getAtr() can return null, we must check it to avoid the call to getBytes() that would
                         * raise an exception. In case of a null value, String.format prints "null" in the string,
                         * the same is done here.
                         */
                        std::string string;
                        if (selectionStatus != nullptr) {
                            string = std::string::format("SeResponse:{RESPONSES = %s, ATR = %s, FCI = %s, HASMATCHED = %b CHANNELWASOPEN = %b}", getApduResponses(), selectionStatus->getAtr()->getBytes().empty() ? "null" : ByteArrayUtils::toHex(selectionStatus->getAtr()->getBytes()), ByteArrayUtils::toHex(selectionStatus->getFci()->getBytes()), selectionStatus->hasMatched(), wasChannelPreviouslyOpen());
                        }
                        else {
                            string = std::string::format("SeResponse:{RESPONSES = %s, ATR = null, FCI = null, HASMATCHED = false CHANNELWASOPEN = %b}", getApduResponses(), wasChannelPreviouslyOpen());
                        }
                        return string;
                    }

                    bool SeResponse::equals(std::shared_ptr<void> o) {
                        if (o == shared_from_this()) {
                            return true;
                        }
                        if (!(std::dynamic_pointer_cast<SeResponse>(o) != nullptr)) {
                            return false;
                        }

                        std::shared_ptr<SeResponse> seResponse = std::static_pointer_cast<SeResponse>(o);
                        return seResponse->getSelectionStatus()->equals(selectionStatus) && (seResponse->getApduResponses().empty() ? apduResponses.empty() : seResponse->getApduResponses().equals(apduResponses)) && seResponse->wasChannelPreviouslyOpen() == channelPreviouslyOpen;
                    }

                    int SeResponse::hashCode() {
                        int hash = 17;
                        hash = 31 * hash + (selectionStatus->getAtr() == nullptr ? 0 : selectionStatus->getAtr()->hashCode());
                        hash = 7 * hash + (apduResponses.empty() ? 0 : this->apduResponses.hashCode());
                        hash = 29 * hash + (this->channelPreviouslyOpen ? 1 : 0);
                        return hash;
                    }
                }
            }
        }
    }
}
