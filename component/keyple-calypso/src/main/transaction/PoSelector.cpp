/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/


#include "PoSelector.h"
#include "SeProtocol.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::protocol;

/* PO SELECTOR -------------------------------------------------------------- */

const int PoSelector::SW_PO_INVALIDATED = 0x6283;

PoSelector::PoSelector(PoSelectorBuilder* builder)
: SeSelector(builder)
{
    if (builder->mInvalidatedPo == InvalidatedPo::ACCEPT)
        getAidSelector()->addSuccessfulStatusCode(SW_PO_INVALIDATED);
}

std::unique_ptr<PoSelectorBuilder> PoSelector::builder()
{
    return std::unique_ptr<PoSelectorBuilder>(new PoSelectorBuilder());
}

/* PO SELECTOR BUILDER ------------------------------------------------------ */

PoSelectorBuilder::PoSelectorBuilder()
: SeSelector::SeSelectorBuilder() {}

PoSelectorBuilder& PoSelectorBuilder::invalidatedPo(
    const InvalidatedPo invalidatedPo)
{
    mInvalidatedPo = invalidatedPo;
    return *this;
}

PoSelectorBuilder& PoSelectorBuilder::seProtocol(
    const std::shared_ptr<SeProtocol> seProtocol) override
{
    return SeSelector::SeSelectorBuilder::seProtocol(seProtocol);
}

PoSelectorBuilder& PoSelectorBuilder::atrFilter(
    const std::shared_ptr<AtrFilter> atrFilter) override
{
    return SeSelector::SeSelectorBuilder::atrFilter(atrFilter);
}

PoSelectorBuilder& PoSelectorBuilder::aidSelector(
    const std::shared_ptr<AidSelector> aidSelector) override
{
    return SeSelector::SeSelectorBuilder::aidSelector(aidSelector);
}

std::unique_ptr<PoSelector> PoSelectorBuilder::build() override
{
    return std::unique_ptr<PoSelectorBuilder>(new PoSelector(this));
}

}
}
}
