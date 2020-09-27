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

/* SAM SELECTOR ------------------------------------------------------------- */

SamSelector::SamSelector(SamSelectorBuilder* builder)
: SeSelector(builder)
{
    std::string atrRegex;
    std::string snRegex;

    /* Check if serialNumber is defined */
    if (builder->serialNumber == "")
        /* Match all serial numbers */
        snRegex = ".{8}";
    else
        /* Match the provided serial number (could be a regex substring) */
        snRegex = builder->serialNumber;

    /*
     * Build the final Atr regex according to the SAM subtype and serial number
     * if any.
     *
     * The header is starting with 3B, its total length is 4 or 6 bytes (8 or
     * 10 hex digits)
     */
    switch (builder->samRevision) {
    case SamRevision::C1:
    case SamRevision::S1D:
    case SamRevision::S1E:
        atrRegex = "3B(.{6}|.{10})805A..80" +
                   builder->samRevision.getApplicationTypeMask() +
                   "20.{4}" +
                   snRegex +
                   "829000";
        break;
    case SamRevision::AUTO:
        /* Match any ATR */
        atrRegex = ".*";
        break;
    default:
        throw IllegalArgumentException("Unknown SAM subtype.");
    }

    getAtrFilter()->setAtrRegex(atrRegex);
}

std::shared_ptr<SamSelectorBuilder> SamSelector::builder()
{
    return std::shared_ptr<SamSelectorBuilder>(new SamSelectorBuilder());
}

/* SAM SELECTOR BUILDER ----------------------------------------------------- */

 SamSelectorBuilder::SamSelectorBuilder()
 : SeSelector::SeSelectorBuilder(), mAtrFiler(std::make_shared<AtrFilter>("")){}

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
    const SamIdentifier& samIdentifier)
{
    mSamRevision = samIdentifier->getSamRevision();
    mSerialNumber = samIdentifier->getSerialNumber();
    return this;
}

SamSelectorBuilder& SamSelectorBuilder::seProtocol(
    const std::shared_ptr<SeProtocol> seProtocol)
{
    return SeSelector::SeSelectorBuilder::seProtocol(seProtocol);
}

/**
 * {@inheritDoc}
 */
SamSelectorBuilder& SamSelectorBuilder::atrFilter(
    const std::shared_ptr<AtrFilter> atrFilter)
{
    return SeSelector::SeSelectorBuilder::atrFilter(atrFilter);
}

/**
 * {@inheritDoc}
 */
SamSelectorBuilder& SamSelectorBuilder::aidSelector(
    const std::shared_ptr<AidSelector> aidSelector)
{
    return SeSelector::SeSelectorBuilder::.aidSelector(aidSelector);
}

std::unique_ptr<SamSelector> SamSelectorBuilder::build()
{
    return std::unique_ptr<SamSelector>(new SamSelector(this));
}

}
}
}
