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

#include "SelectionStatus.h"
#include "AnswerToReset.h"
#include "ApduResponse.h"
#include "Arrays.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

SelectionStatus::SelectionStatus(std::shared_ptr<AnswerToReset> atr,
                                 std::shared_ptr<ApduResponse> fci,
                                 bool isMatching)
: atr(atr), fci(fci), isMatching(isMatching)
{
    if (atr == nullptr && fci == nullptr) {
        throw std::invalid_argument(
            "Atr and Fci can't be null at the same time.");
    }
}

std::shared_ptr<AnswerToReset> SelectionStatus::getAtr()
{
    return atr;
}

std::shared_ptr<ApduResponse> SelectionStatus::getFci()
{
    return fci;
}

bool SelectionStatus::hasMatched() const
{
    return isMatching;
}

bool SelectionStatus::equals(std::shared_ptr<void> o)
{
    if (o == shared_from_this()) {
        return true;
    }

    if (!(std::static_pointer_cast<SelectionStatus>(o) != nullptr)) {
        return false;
    }

    std::shared_ptr<SelectionStatus> selectionStatus =
        std::static_pointer_cast<SelectionStatus>(o);

    return selectionStatus->getAtr() == nullptr
               ? this->atr == nullptr
               : selectionStatus->getAtr()->equals(this->atr) &&
                         selectionStatus->getFci() == nullptr
                     ? this->fci == nullptr
                     : selectionStatus->getFci()->equals(this->fci) &&
                           selectionStatus->hasMatched() == isMatching;
}

int SelectionStatus::hashCode()
{
    int hash = 17;
    hash     = 19 * hash + (isMatching ? 0 : 1);
    hash = 31 * hash + (atr == nullptr ? 0 : Arrays::hashCode(atr->getBytes()));
    hash = 7 * hash + (fci == nullptr ? 0 : Arrays::hashCode(fci->getBytes()));
    return hash;
}

void SelectionStatus::finalize()
{
}

std::ostream& operator<<(std::ostream& os, const SelectionStatus& s)
{
    os << "SELECTIONSTATUS: {"
       << "ATR = " << s.atr << ", "
       << "FCI = " << s.fci << ", "
       << "HASMATCHED = " << s.isMatching << "}";

    return os;
}

std::ostream& operator<<(std::ostream& os,
                         const std::shared_ptr<SelectionStatus>& s)
{
    if (s)
        os << *(s.get());
    else
		os << "SELECTIONSTATUS: null";

    return os;
}

}
}
}
}
