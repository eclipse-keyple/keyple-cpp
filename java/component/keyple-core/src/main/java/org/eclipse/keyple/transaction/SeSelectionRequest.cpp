#include "SeSelectionRequest.h"
#include "../seproxy/SeSelector.h"
#include "../seproxy/message/ApduRequest.h"
#include "../seproxy/protocol/SeProtocol.h"
#include "../seproxy/message/SeRequest.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace transaction {
                using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                using SeSelector = org::eclipse::keyple::seproxy::SeSelector;
                using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;
                using SeRequest = org::eclipse::keyple::seproxy::message::SeRequest;
                using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                using org::slf4j::Logger;
                using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> SeSelectionRequest::logger = org::slf4j::LoggerFactory::getLogger(SeSelectionRequest::typeid);

                SeSelectionRequest::SeSelectionRequest(std::shared_ptr<SeSelector> seSelector, ChannelState channelState, std::shared_ptr<SeProtocol> protocolFlag) : channelState(channelState), protocolFlag(protocolFlag) {
                    this->seSelector = seSelector;
                    if (logger->isTraceEnabled()) {
                        logger->trace("SeSelection");
                    }
                }

                std::shared_ptr<SeRequest> SeSelectionRequest::getSelectionRequest() {
                    std::shared_ptr<SeRequest> seSelectionRequest = nullptr;
                    seSelectionRequest = std::make_shared<SeRequest>(seSelector, seSelectionApduRequestList, channelState, protocolFlag);
                    return seSelectionRequest;
                }

                std::shared_ptr<SeSelector> SeSelectionRequest::getSeSelector() {
                    return seSelector;
                }

                void SeSelectionRequest::setMatchingClass(std::type_info matchingClass) {
                    this->matchingClass = matchingClass;
                }

                void SeSelectionRequest::setSelectionClass(std::type_info selectionClass) {
                    this->selectionClass = selectionClass;
                }

                std::type_info SeSelectionRequest::getSelectionClass() {
                    return selectionClass;
                }

                std::type_info SeSelectionRequest::getMatchingClass() {
                    return matchingClass;
                }

                void SeSelectionRequest::addApduRequest(std::shared_ptr<ApduRequest> apduRequest) {
                    seSelectionApduRequestList.push_back(apduRequest);
                }

                std::string SeSelectionRequest::toString() {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                    return "SeSelectionRequest: SELECTION_CLASS = " + selectionClass.toString() + ", MATCHING_CLASS = " + matchingClass.toString();
                }
            }
        }
    }
}
