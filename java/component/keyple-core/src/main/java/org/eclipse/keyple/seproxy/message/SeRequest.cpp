#include "SeRequest.h"
#include "../../util/ByteArrayUtils.h"
#include "ApduRequest.h"
#include "../protocol/SeProtocol.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace message {
                    using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                    using Protocol = org::eclipse::keyple::seproxy::protocol::Protocol;
                    using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                    using SeSelector = org::eclipse::keyple::transaction::SeSelector;
                    using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;

                    SeRequest::AidSelector::AidSelector(std::vector<char> &aidToSelect) {
                        if (aidToSelect.size() < AID_MIN_LENGTH || aidToSelect.size() > AID_MAX_LENGTH) {
                            throw std::invalid_argument(std::string::format("Bad AID length: %d", aidToSelect.size()));
                        }
                        this->aidToSelect = aidToSelect;
                    }

                    SeRequest::AidSelector::AidSelector(std::vector<char> &aidToSelect, SeSelector::SelectMode selectMode) : AidSelector(aidToSelect) {
                        this->selectMode = selectMode;
                    }

                    std::vector<char> SeRequest::AidSelector::getAidToSelect() {
                        return aidToSelect;
                    }

                    bool SeRequest::AidSelector::isSelectNext() {
                        return selectMode == SeSelector::SelectMode::NEXT;
                    }

                    std::string SeRequest::AidSelector::toString() {
                        return StringHelper::formatSimple("AID:%s", aidToSelect.empty() ? "null" : ByteArrayUtils::toHex(aidToSelect));
                    }

                    SeRequest::AtrSelector::AtrSelector(const std::string &atrRegex) {
                        this->atrRegex = atrRegex;
                    }

                    std::string SeRequest::AtrSelector::getAtrRegex() {
                        return atrRegex;
                    }

                    bool SeRequest::AtrSelector::atrMatches(std::vector<char> &atr) {
                        bool m;
                        if (atrRegex.length() != 0) {
                            std::shared_ptr<Pattern> p = Pattern::compile(atrRegex);
                            std::string atrString = ByteArrayUtils::toHex(atr);
                            m = p->matcher(atrString).matches();
                        }
                        else {
                            m = true;
                        }
                        return m;
                    }

                    std::string SeRequest::AtrSelector::toString() {
                        return StringHelper::formatSimple("ATR regex:%s", atrRegex.length() != 0 ? atrRegex : "empty");
                    }

                    SeRequest::SeRequest(std::shared_ptr<Selector> selector, std::vector<std::shared_ptr<ApduRequest>> &apduRequests, ChannelState channelState, std::shared_ptr<SeProtocol> protocolFlag, std::shared_ptr<Set<Integer>> successfulSelectionStatusCodes) : selector(selector) {
                        if (protocolFlag == nullptr) {
                            throw std::invalid_argument("¨protocolFlag can't be null");
                        }
                        this->apduRequests = apduRequests;
                        this->channelState = channelState;
                        this->protocolFlag = protocolFlag;
                        this->successfulSelectionStatusCodes = successfulSelectionStatusCodes;
                    }

                    SeRequest::SeRequest(std::vector<std::shared_ptr<ApduRequest>> &apduRequests, ChannelState channelState) : SeRequest(nullptr, apduRequests, channelState, Protocol::ANY, nullptr) {
                    }

                    std::shared_ptr<Selector> SeRequest::getSelector() {
                        return selector;
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

                    std::shared_ptr<Set<Integer>> SeRequest::getSuccessfulSelectionStatusCodes() {
                        return successfulSelectionStatusCodes;
                    }

                    std::string SeRequest::toString() {
                        return StringHelper::formatSimple("SeRequest:{REQUESTS = %s, SELECTOR = %s, KEEPCHANNELOPEN = %s}", getApduRequests(), getSelector(), channelState);
                    }
                }
            }
        }
    }
}
