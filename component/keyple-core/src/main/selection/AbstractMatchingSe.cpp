/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
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

#include <iostream>

#include "AbstractMatchingSe.h"
#include "SeResponse.h"
#include "SelectionStatus.h"

namespace keyple {
namespace core {
namespace selection {

using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::protocol;

AbstractMatchingSe::AbstractMatchingSe(
  std::shared_ptr<SeResponse> selectionResponse,
  TransmissionMode transmissionMode, const std::string &extraInfo)
: selectionResponse(selectionResponse), transmissionMode(transmissionMode),
  selectionExtraInfo(extraInfo)
{
    if (selectionResponse != nullptr) {
        this->selectionStatus = selectionResponse->getSelectionStatus();
    }
    else {
        this->selectionStatus.reset();
    }
}

bool AbstractMatchingSe::isSelected()
{
    bool isSelected;

    if (selectionStatus != nullptr) {
        isSelected = selectionStatus->hasMatched() &&
                     selectionResponse->isLogicalChannelOpen();
    } else {
        isSelected = false;
    }

    return isSelected;
}

std::shared_ptr<SelectionStatus> AbstractMatchingSe::getSelectionStatus()
{
    return selectionStatus;
}

TransmissionMode AbstractMatchingSe::getTransmissionMode() const
{
    return transmissionMode;
}

std::string AbstractMatchingSe::getSelectionExtraInfo()
{
    return selectionExtraInfo;
}
}
}
}
