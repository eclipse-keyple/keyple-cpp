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

bool SelectionStatus::operator==(const SelectionStatus& o) const
{
    return /* Both pointers are null or equal */
           ((!this->atr && !o.atr) ||
            *(this->atr.get()) == *(o.atr.get())) &&
           /* Both pointers are null or equal */
           ((!this->fci && !o.fci) ||
            *(this->fci.get()) == *(o.fci.get())) &&
           this->isMatching == o.isMatching;
}

bool SelectionStatus::operator!=(const SelectionStatus& o) const
{
    return !(*this == o);
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
