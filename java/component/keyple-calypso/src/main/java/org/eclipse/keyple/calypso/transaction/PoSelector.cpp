#include "PoSelector.h"
#include "SeProtocol.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace transaction {

using SeSelector = org::eclipse::keyple::core::seproxy::SeSelector;
using SeProtocol = org::eclipse::keyple::core::seproxy::protocol::SeProtocol;

PoSelector::PoSelector(std::shared_ptr<SeProtocol> seProtocol, std::shared_ptr<PoAtrFilter> poAtrFilter, std::shared_ptr<PoAidSelector> poAidSelector, const std::string &extraInfo) : org::eclipse::keyple::core::seproxy::SeSelector(seProtocol, poAtrFilter, poAidSelector, extraInfo) {
}

const std::shared_ptr<java::util::Set<Integer>> PoSelector::PoAidSelector::successfulSelectionStatusCodes = std::make_shared<HashSetAnonymousInnerClass>();

PoSelector::PoAidSelector::PoAidSelector(std::shared_ptr<IsoAid> aidToSelect, InvalidatedPo invalidatedPo, FileOccurrence fileOccurrence, FileControlInformation fileControlInformation) : AidSelector(aidToSelect, invalidatedPo == InvalidatedPo::ACCEPT ? successfulSelectionStatusCodes : nullptr, fileOccurrence, fileControlInformation) {
}

PoSelector::PoAidSelector::PoAidSelector(std::shared_ptr<IsoAid> aidToSelect, InvalidatedPo invalidatedPo) : AidSelector(aidToSelect, invalidatedPo == InvalidatedPo::ACCEPT ? successfulSelectionStatusCodes : nullptr) {
}

PoSelector::PoAtrFilter::PoAtrFilter(const std::string &atrRegex) : AtrFilter(atrRegex) {
}

}
}
}
}
}
