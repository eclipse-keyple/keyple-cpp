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

namespace keyple {
namespace calypso {
namespace transaction {

std::unique_ptr<ManagedSamResource> createSamResource(
    std::shared_ptr<SeReader> samReader)
{
    SeSelection samSelection;

    /* Prepare selector */
    samSelection.prepareSelection(
        std::make_shared<SamSelectionRequest>(
            SamSelector::builder()
                .seProtocol(SeCommonProtocols::PROTOCOL_ISO7816_3)
                .samIdentifier(SamIdentifier::builder()
                                   .samRevision(AUTO)
                                   .build())
                .build()));

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
        selectionsResult->getActiveMatchingSe();

    return std::unique_ptr<ManagedSamResource>(
               new ManagedSamResource(samReader, calypsoSam));
}

}
}
}
