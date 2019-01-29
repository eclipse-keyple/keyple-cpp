
/* Core */
#include "SeSelector.h"
#include "ApduRequest.h"
#include "SeProtocol.h"
#include "ByteArrayUtils.h"
#include "SeRequest.h"

/* Common */
#include "LoggerFactory.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace transaction {

using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;
using SeRequest = org::eclipse::keyple::seproxy::message::SeRequest;
using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;

const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(SeSelector));

std::string SeSelector::getAtrRegex() {
    return atrRegex;
}

std::vector<char> &SeSelector::getAid() {
    return aid;
}

SeSelector::SelectMode SeSelector::getSelectMode() {
    return selectMode;
}

bool SeSelector::isSelectionByAid() {
    return selectionByAid;
}

SeSelector::SeSelector(const std::string &atrRegex, ChannelState channelState, std::shared_ptr<SeProtocol> protocolFlag, const std::string &extraInfo)
: channelState(channelState), protocolFlag(protocolFlag), atrRegex(atrRegex), aid(std::vector<char>()), selectMode(SelectMode::FIRST), selectionByAid(false), matchingClass(std::type_index(typeid(MatchingSe))), selectorClass(std::type_index(typeid(SeSelector))) {

    if (extraInfo != "") {
        this->extraInfo = extraInfo;
    }
    else {
        this->extraInfo = "";
    }
    if (logger->isTraceEnabled()) {
        logger->trace("ATR based selection: ATRREGEX = {}, KEEPCHANNELOPEN = {}, PROTOCOLFLAG = {}", atrRegex, "channelState", protocolFlag);
    }

    this->aid.clear();
}

SeSelector::SeSelector(std::vector<char> &aid, SelectMode selectMode, ChannelState channelState, std::shared_ptr<SeProtocol> protocolFlag, const std::string &extraInfo)
: channelState(channelState), protocolFlag(protocolFlag), atrRegex(""), aid(aid), selectMode(selectMode), selectionByAid(true), matchingClass(std::type_index(typeid(MatchingSe))), selectorClass(std::type_index(typeid(SeSelector))) {

    if (extraInfo != "") {
        this->extraInfo = extraInfo;
    }
    else {
        this->extraInfo = "";
    }
    if (logger->isTraceEnabled()) {
        logger->trace("AID based selection: AID = {}, KEEPCHANNELOPEN = {}, PROTOCOLFLAG = {}", ByteArrayUtils::toHex(aid), "channelState", protocolFlag);
    }
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
        seSelectionRequest = std::shared_ptr<SeRequest>(new SeRequest(std::shared_ptr<SeRequest::AtrSelector>(new SeRequest::AtrSelector(getAtrRegex())), seSelectionApduRequestList, channelState, protocolFlag, nullptr));
    }
    else {
        SeRequest::AidSelector *_aid = new SeRequest::AidSelector(getAid(), getSelectMode());
        std::shared_ptr<SeRequest::AidSelector> __aid = std::shared_ptr<SeRequest::AidSelector>(_aid);
        seSelectionRequest = std::shared_ptr<SeRequest>(new SeRequest(__aid, seSelectionApduRequestList, channelState, protocolFlag, selectApplicationSuccessfulStatusCodes));
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

std::type_index &SeSelector::getMatchingClass() {
    return matchingClass;
}

std::type_index &SeSelector::getSelectorClass() {
    return selectorClass;
}

}
}
}
}
