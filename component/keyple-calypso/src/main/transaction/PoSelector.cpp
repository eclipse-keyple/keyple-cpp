/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#include "PoSelector.h"
#include "SeProtocol_Import.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::protocol;

PoSelector::PoSelector(SeProtocol& seProtocol,
                       std::shared_ptr<PoAtrFilter> poAtrFilter,
                       std::shared_ptr<PoAidSelector> poAidSelector,
                       const std::string& extraInfo)
: SeSelector(seProtocol, poAtrFilter, poAidSelector, extraInfo)
{
}

const std::shared_ptr<std::set<int>>
    PoSelector::PoAidSelector::successfulSelectionStatusCodes =
        std::make_shared<std::set<int>>();

PoSelector::PoAidSelector::PoAidSelector(
    std::shared_ptr<IsoAid> aidToSelect, InvalidatedPo invalidatedPo,
    FileOccurrence fileOccurrence,
    FileControlInformation fileControlInformation)
: AidSelector(aidToSelect,
              invalidatedPo == InvalidatedPo::ACCEPT
                  ? successfulSelectionStatusCodes
                  : nullptr,
              fileOccurrence, fileControlInformation)
{
}

PoSelector::PoAidSelector::PoAidSelector(std::shared_ptr<IsoAid> aidToSelect,
                                         InvalidatedPo invalidatedPo)
: AidSelector(aidToSelect, invalidatedPo == InvalidatedPo::ACCEPT
                               ? successfulSelectionStatusCodes
                               : nullptr)
{
}

PoSelector::PoAtrFilter::PoAtrFilter(const std::string& atrRegex)
: AtrFilter(atrRegex)
{
}

}
}
}
