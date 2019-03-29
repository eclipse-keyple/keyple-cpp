
/* Core */
#include "SeSelector.h"
#include "ApduRequest.h"
#include "SeProtocol.h"
#include "ByteArrayUtils.h"
#include "SeRequest.h"

namespace org {
namespace eclipse {
namespace keyple {
            namespace seproxy {
using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;

                SeSelector::AidSelector::AidSelector(std::vector<char> &aidToSelect, std::shared_ptr<Set<Integer>> successfulSelectionStatusCodes, FileOccurrence fileOccurrence, FileControlInformation fileControlInformation) {
                    if (aidToSelect.empty() || aidToSelect.size() < AID_MIN_LENGTH || aidToSelect.size() > AID_MAX_LENGTH) {
                        throw std::invalid_argument("Bad AID value: must be between " + std::to_string(AID_MIN_LENGTH) + " and " + std::to_string(AID_MIN_LENGTH) + " bytes.");
}
                    this->aidToSelect = aidToSelect;
                    this->successfulSelectionStatusCodes = successfulSelectionStatusCodes;
                    this->fileOccurrence = fileOccurrence;
                    this->fileControlInformation = fileControlInformation;
}

                SeSelector::AidSelector::AidSelector(std::vector<char> &aidToSelect, std::shared_ptr<Set<Integer>> successfulSelectionStatusCodes) : AidSelector(aidToSelect, successfulSelectionStatusCodes, FileOccurrence::FIRST, FileControlInformation::FCI) {
}

                std::vector<char> SeSelector::AidSelector::getAidToSelect() {
                    return aidToSelect;
}

                bool SeSelector::AidSelector::isSelectNext() {
                    return fileOccurrence == FileOccurrence::NEXT;
                }

                std::shared_ptr<Set<Integer>> SeSelector::AidSelector::getSuccessfulSelectionStatusCodes() {
                    return successfulSelectionStatusCodes;
    }

                std::string SeSelector::AidSelector::toString() {
                    return StringHelper::formatSimple("AID:%s", aidToSelect.empty() ? "null" : ByteArrayUtils::toHex(aidToSelect));
    }

                SeSelector::AtrFilter::AtrFilter(const std::string &atrRegex) {
                    this->atrRegex = atrRegex;
    }

                void SeSelector::AtrFilter::setAtrRegex(const std::string &atrRegex) {
                    this->atrRegex = atrRegex;
}

                std::string SeSelector::AtrFilter::getAtrRegex() {
                    return atrRegex;
                }

                bool SeSelector::AtrFilter::atrMatches(std::vector<char> &atr) {
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

                std::string SeSelector::AtrFilter::toString() {
                    return StringHelper::formatSimple("ATR regex:%s", atrRegex.length() != 0 ? atrRegex : "empty");
}

                SeSelector::SeSelector(std::shared_ptr<AidSelector> aidSelector, std::shared_ptr<AtrFilter> atrFilter, const std::string &extraInfo) : aidSelector(aidSelector), atrFilter(atrFilter) {
                    if (extraInfo != "") {
                        this->extraInfo = extraInfo;
    }
    else {
                        this->extraInfo = "";
    }
                    if (logger->isTraceEnabled()) {
                        logger->trace("Selection data: AID = {}, ATRREGEX = {}, EXTRAINFO = {}", this->aidSelector == nullptr ? "null" : ByteArrayUtils::toHex(this->aidSelector->getAidToSelect()), this->atrFilter == nullptr ? "null" : this->atrFilter->getAtrRegex(), extraInfo);
}
}

                std::shared_ptr<AidSelector> SeSelector::getAidSelector() {
                    return aidSelector;
}

                std::shared_ptr<AtrFilter> SeSelector::getAtrFilter() {
                    return atrFilter;
}

                std::string SeSelector::getExtraInfo() {
                    return extraInfo;
}

                std::string SeSelector::toString() {
                    return "SeSelector: AID_SELECTOR = "
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                            + (aidSelector == nullptr ? "null" : aidSelector->toString()) + ", ATR_FILTER " + "= "
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                            + (atrFilter == nullptr ? "null" : atrFilter->toString());
}
}
}
}
}
