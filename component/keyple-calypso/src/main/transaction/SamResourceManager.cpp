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

#include "SamResourceManager.h"

/* Calypso */
#include "CalypsoNoSamResourceAvailableException.h"
#include "SamIdentifier.h"
#include "SamSelectionRequest.h"
#include "SamSelector.h"

/* Core */
#include "SeCommonProtocols.h"
#include "SeSelection.h"


namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::exception;
using namespace keyple::core::selection;
using namespace keyple::core::seproxy::protocol;

using ManagedSamResource = SamResourceManager::ManagedSamResource;

std::unique_ptr<ManagedSamResource> SamResourceManager::createSamResource(
    std::shared_ptr<SeReader> samReader)
{
    SeSelection samSelection;

    /* Prepare selector */
    samSelection.prepareSelection(
        std::make_shared<SamSelectionRequest>(
            std::dynamic_pointer_cast<SamSelector> (
                SamSelector::builder()
                    ->seProtocol(SeCommonProtocols::PROTOCOL_ISO7816_3)
                    .samIdentifier(SamIdentifier::builder()
                                       ->samRevision(SamRevision::AUTO)
                                       .build())
                    .build())));

    std::shared_ptr<SelectionsResult> selectionsResult = nullptr;

    try {
        selectionsResult = samSelection.processExplicitSelection(samReader);
    } catch (const KeypleException& e) {
        throw CalypsoNoSamResourceAvailableException("Failed to select a SAM");
    }

    if (!selectionsResult->hasActiveSelection())
        throw CalypsoNoSamResourceAvailableException(
                  "Unable to open a logical channel for SAM!");

    std::shared_ptr<CalypsoSam> calypsoSam =
        std::dynamic_pointer_cast<CalypsoSam>(
            selectionsResult->getActiveMatchingSe());

    return std::unique_ptr<ManagedSamResource>(
               new ManagedSamResource(samReader, calypsoSam));
}

/* MANAGED SAM RESOURCE ----------------------------------------------------- */

ManagedSamResource::ManagedSamResource(
  std::shared_ptr<SeReader> seReader,
  std::shared_ptr<CalypsoSam> calypsoSam)
: SeResource<CalypsoSam>(seReader, calypsoSam),
  mSamResourceStatus(SamResourceStatus::FREE),
  mSamIdentifier(nullptr) {}

bool ManagedSamResource::isSamResourceFree() const
{
    return mSamResourceStatus == SamResourceStatus::FREE;
}

void ManagedSamResource::setSamIdentifier(
    std::shared_ptr<SamIdentifier> samIdentifier)
{
    mSamIdentifier = samIdentifier;
}

bool ManagedSamResource::isSamMatching(
    const std::shared_ptr<SamIdentifier> samIdentifier) const
{
    return samIdentifier->matches(mSamIdentifier);
}

void ManagedSamResource::setSamResourceStatus(
    const SamResourceStatus& samResourceStatus)
{
    mSamResourceStatus = samResourceStatus;
}

}
}
}
