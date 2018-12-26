#include "SeSelector.h"
#include "../seproxy/message/ApduRequest.h"
#include "../seproxy/protocol/SeProtocol.h"
#include "../util/ByteArrayUtils.h"
#include "../seproxy/message/SeRequest.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace transaction {
                using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;
                using SeRequest = org::eclipse::keyple::seproxy::message::SeRequest;
                using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                //using org::slf4j::Logger;
                //using org::slf4j::LoggerFactory;
//const std::shared_ptr<org::slf4j::Logger> SeSelector::logger = org::slf4j::LoggerFactory::getLogger(SeSelector::typeid);

                std::string SeSelector::getAtrRegex() {
                    return atrRegex;
                }

                std::vector<char> SeSelector::getAid() {
                    return aid;
                }

                SeSelector::SelectMode SeSelector::getSelectMode() {
                    return selectMode;
                }

                bool SeSelector::isSelectionByAid() {
                    return selectionByAid;
                }

                SeSelector::SeSelector(const std::string &atrRegex, ChannelState channelState, std::shared_ptr<SeProtocol> protocolFlag, const std::string &extraInfo) : channelState(channelState), protocolFlag(protocolFlag), atrRegex(atrRegex), aid(aid.clear()), selectMode(SelectMode::FIRST), selectionByAid(false) {

                    matchingClass = MatchingSe::typeid;
                    selectorClass = SeSelector::typeid;

                    if (extraInfo != "") {
                        this->extraInfo = extraInfo;
                    }
                    else {
                        this->extraInfo = "";
                    }
//                    if (logger->isTraceEnabled()) {
//                        logger->trace("ATR based selection: ATRREGEX = {}, KEEPCHANNELOPEN = {}, PROTOCOLFLAG = {}", atrRegex, channelState, protocolFlag);
//                    }
                }

                SeSelector::SeSelector(std::vector<char> &aid, SelectMode selectMode, ChannelState channelState, std::shared_ptr<SeProtocol> protocolFlag, const std::string &extraInfo) : channelState(channelState), protocolFlag(protocolFlag), atrRegex(""), aid(aid), selectMode(selectMode), selectionByAid(true) {
                    matchingClass = MatchingSe::typeid;
                    selectorClass = SeSelector::typeid;

                    if (extraInfo != "") {
                        this->extraInfo = extraInfo;
                    }
                    else {
                        this->extraInfo = "";
                    }
//                    if (logger->isTraceEnabled()) {
//                        logger->trace("AID based selection: AID = {}, KEEPCHANNELOPEN = {}, PROTOCOLFLAG = {}", ByteArrayUtils::toHex(aid), channelState, protocolFlag);
//                    }
                }

                std::shared_ptr<SeProtocol> SeSelector::getProtocolFlag() {
                    return protocolFlag;
                }

                void SeSelector::setSelectionApduRequestList(std::vector<std::shared_ptr<ApduRequest>> &seSelectionApduRequestList) {
                    this->seSelectionApduRequestList = seSelectionApduRequestList;
                }

                std::shared_ptr<SeRequest> SeSelector::getSelectorRequest() {
                    std::shared_ptr<SeRequest> seSelectionRequest;
                    if (!isSelectionByAid()) {
                        seSelectionRequest = std::make_shared<SeRequest>(std::make_shared<SeRequest::AtrSelector>(getAtrRegex()), seSelectionApduRequestList, channelState, protocolFlag, nullptr);
                    }
                    else {
                        seSelectionRequest = std::make_shared<SeRequest>(std::make_shared<SeRequest::AidSelector>(getAid(), getSelectMode()), seSelectionApduRequestList, channelState, protocolFlag, selectApplicationSuccessfulStatusCodes);
                    }
                    return seSelectionRequest;
                }

                std::string SeSelector::getExtraInfo() {
                    return extraInfo;
                }

                void SeSelector::setMatchingClass(std::type_info matchingClass) {
                    this->matchingClass = matchingClass;
                }

                void SeSelector::setSelectorClass(std::type_info selectorClass) {
                    this->selectorClass = selectorClass;
                }

                std::type_info SeSelector::getMatchingClass() {
                    return matchingClass;
                }

                std::type_info SeSelector::getSelectorClass() {
                    return selectorClass;
                }
            }
        }
    }
}
