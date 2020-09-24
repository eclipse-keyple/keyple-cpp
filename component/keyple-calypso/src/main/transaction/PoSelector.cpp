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

/* Core */
#include "SeProtocol.h"
#include "SeSelector.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::protocol;

using PoSelectorBuilder = PoSelector::PoSelectorBuilder;
using SeSelectorBuilder = SeSelector::SeSelectorBuilder;
/* PO SELECTOR -------------------------------------------------------------- */

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
    const std::shared_ptr<SeProtocol> seProtocol)
{
    return dynamic_cast<PoSelectorBuilder&>(
               SeSelectorBuilder::seProtocol(seProtocol));
}

PoSelectorBuilder& PoSelectorBuilder::atrFilter(
    const std::shared_ptr<AtrFilter> atrFilter)
{
    return dynamic_cast<PoSelectorBuilder&>(
               SeSelectorBuilder::atrFilter(atrFilter));
}

PoSelectorBuilder& PoSelectorBuilder::aidSelector(
    const std::shared_ptr<AidSelector> aidSelector)
{
    return dynamic_cast<PoSelectorBuilder&>(
               SeSelectorBuilder::aidSelector(aidSelector));
}

std::unique_ptr<SeSelector> PoSelectorBuilder::build()
{
    return std::unique_ptr<PoSelector>(new PoSelector(this));
}

}
}
}
