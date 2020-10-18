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

/* Common */
#include "IllegalStateException.h"
#include "KeypleStd.h"

/* Core*/
#include "ApduResponse.h"
#include "SeResponse.h"
#include "SelectionStatus.h"

namespace keyple {
namespace core {
namespace selection {

using namespace keyple::common;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::protocol;

AbstractMatchingSe::AbstractMatchingSe(
    const std::shared_ptr<SeResponse> selectionResponse,
    const TransmissionMode& transmissionMode)
: mTransmissionMode(transmissionMode)
{
    /* /!\ C++ vs. Java: check getSelectionStatus() existence */

    if (selectionResponse) {
        const std::shared_ptr<SelectionStatus> selectionStatus =
            selectionResponse->getSelectionStatus();

        if (selectionStatus) {
            std::shared_ptr<ApduResponse> fci = selectionStatus->getFci();
            if (fci)
                mFciBytes = fci->getBytes();

            std::shared_ptr<AnswerToReset> atr = selectionStatus->getAtr();
            if (atr)
                mAtrBytes = atr->getBytes();
        }
    }
}

bool AbstractMatchingSe::hasFci() const
{
    return mFciBytes.size() > 0;
}

bool AbstractMatchingSe::hasAtr() const
{
    return mAtrBytes.size() > 0;
}

const std::vector<uint8_t>& AbstractMatchingSe::getFciBytes() const
{
    if (hasFci())
        return mFciBytes;

    throw IllegalStateException(
              "No FCI is available in this AbstractMatchingSe");
}

 const std::vector<uint8_t>& AbstractMatchingSe::getAtrBytes() const
 {
    if (hasAtr())
        return mAtrBytes;

    throw IllegalStateException(
              "No ATR is available in this AbstractMatchingSe");
}

const TransmissionMode& AbstractMatchingSe::getTransmissionMode() const
{
    return mTransmissionMode;
}

std::ostream& operator<<(std::ostream& os, const AbstractMatchingSe& ams)
{
    os << "ABSTRACTMATCHINGSE: {"
        << "TRANSMISSIONMODE = " << ams.mTransmissionMode << ", "
        << "FCI = " << ams.mFciBytes << ", "
        << "ATR = " << ams.mAtrBytes
        << "}";

    return os;
}

std::ostream& operator<<(std::ostream& os,
                         const std::shared_ptr<AbstractMatchingSe>& ams)
{
    if (ams)
        os << *ams.get();
    else
        os << "ABSTRACTMATCHINGSE = null";

    return os;
}

}
}
}
