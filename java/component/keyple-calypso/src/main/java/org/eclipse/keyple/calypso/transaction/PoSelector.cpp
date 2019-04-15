#include "PoSelector.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace transaction {
                    using SeSelector = org::eclipse::keyple::seproxy::SeSelector;

                    PoSelector::PoSelector(std::shared_ptr<PoAidSelector> poAidSelector, std::shared_ptr<PoAtrFilter> poAtrFilter, const std::string &extraInfo) : org::eclipse::keyple::seproxy::SeSelector(poAidSelector, poAtrFilter, extraInfo) {
                    }

                    const std::shared_ptr<std::set<int>> PoSelector::PoAidSelector::successfulSelectionStatusCodes = std::make_shared<std::set<int>>();

                    PoSelector::PoAidSelector::PoAidSelector(std::vector<char> &aidToSelect, InvalidatedPo invalidatedPo, FileOccurrence fileOccurrence, FileControlInformation fileControlInformation) : org::eclipse::keyple::seproxy::SeSelector::AidSelector(aidToSelect, invalidatedPo == InvalidatedPo::ACCEPT ? successfulSelectionStatusCodes : nullptr, fileOccurrence, fileControlInformation) {
                    }

                    PoSelector::PoAidSelector::PoAidSelector(std::vector<char> &aidToSelect, InvalidatedPo invalidatedPo) : org::eclipse::keyple::seproxy::SeSelector::AidSelector(aidToSelect, invalidatedPo == InvalidatedPo::ACCEPT ? successfulSelectionStatusCodes : nullptr) {
                    }

                    PoSelector::PoAtrFilter::PoAtrFilter(const std::string &atrRegex) : org::eclipse::keyple::seproxy::SeSelector::AtrFilter(atrRegex) {
                    }
                }
            }
        }
    }
}
