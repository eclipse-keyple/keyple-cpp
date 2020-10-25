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

#include "SamSelector.h"

/* Calypso */
#include "SamRevision.h"

/* Common */
#include "IllegalArgumentException.h"

/* Core */
#include "SeProtocol.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command::sam;
using namespace keyple::common;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::protocol;

using SamSelectorBuilder = SamSelector::SamSelectorBuilder;

/* SAM SELECTOR ------------------------------------------------------------- */

SamSelector::SamSelector(SamSelectorBuilder* builder)
: SeSelector(builder)
{
    std::string atrRegex;
    std::string snRegex;

    /* Check if serialNumber is defined */
    if (builder->mSerialNumber == "")
        /* Match all serial numbers */
        snRegex = ".{8}";
    else
        /* Match the provided serial number (could be a regex substring) */
        snRegex = builder->mSerialNumber;

    /*
     * Build the final Atr regex according to the SAM subtype and serial number
     * if any.
     *
     * The header is starting with 3B, its total length is 4 or 6 bytes (8 or
     * 10 hex digits)
     */
    if (builder->mSamRevision == SamRevision::C1 ||
        builder->mSamRevision ==  SamRevision::S1D ||
        builder->mSamRevision ==  SamRevision::S1E)
        atrRegex = "3B(.{6}|.{10})805A..80" +
                   builder->mSamRevision.getApplicationTypeMask() +
                   "20.{4}" +
                   snRegex +
                   "829000";
    else if (builder->mSamRevision == SamRevision::AUTO)
        /* Match any ATR */
        atrRegex = ".*";
    else
        throw IllegalArgumentException("Unknown SAM subtype.");

    getAtrFilter()->setAtrRegex(atrRegex);
}

std::shared_ptr<SamSelectorBuilder> SamSelector::builder()
{
    return std::shared_ptr<SamSelectorBuilder>(new SamSelectorBuilder());
}

/* SAM SELECTOR BUILDER ----------------------------------------------------- */

 SamSelectorBuilder::SamSelectorBuilder()
 : SeSelector::SeSelectorBuilder()
{
    mAtrFilter = std::make_shared<AtrFilter>("");
}

SamSelectorBuilder& SamSelectorBuilder::samRevision(
    const SamRevision samRevision)
{
    mSamRevision = samRevision;
    return *this;
}

SamSelectorBuilder& SamSelectorBuilder::serialNumber(
    const std::string& serialNumber)
{
    mSerialNumber = serialNumber;
    return *this;
}

SamSelectorBuilder& SamSelectorBuilder::samIdentifier(
    const std::shared_ptr<SamIdentifier> samIdentifier)
{
    mSamRevision = samIdentifier->getSamRevision();
    mSerialNumber = samIdentifier->getSerialNumber();
    return *this;
}

SamSelectorBuilder& SamSelectorBuilder::seProtocol(
    const std::shared_ptr<SeProtocol> seProtocol)
{
    return dynamic_cast<SamSelectorBuilder&>(
               SeSelector::SeSelectorBuilder::seProtocol(seProtocol));
}

/**
 * {@inheritDoc}
 */
SamSelectorBuilder& SamSelectorBuilder::atrFilter(
    const std::shared_ptr<AtrFilter> atrFilter)
{
    return dynamic_cast<SamSelectorBuilder&>(
               SeSelector::SeSelectorBuilder::atrFilter(atrFilter));
}

/**
 * {@inheritDoc}
 */
SamSelectorBuilder& SamSelectorBuilder::aidSelector(
    const std::shared_ptr<AidSelector> aidSelector)
{
    return dynamic_cast<SamSelectorBuilder&>(
               SeSelector::SeSelectorBuilder::aidSelector(aidSelector));
}

std::shared_ptr<SeSelector> SamSelectorBuilder::build()
{
    return std::make_shared<SamSelector>(this);
}

}
}
}
