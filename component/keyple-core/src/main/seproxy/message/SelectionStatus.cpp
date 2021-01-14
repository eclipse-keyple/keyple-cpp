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
                                 const bool isMatching)
: mAtr(atr), mFci(fci), mIsMatching(isMatching)
{
    if (atr == nullptr && fci == nullptr) {
        throw std::invalid_argument(
            "Atr and Fci can't be null at the same time.");
    }
}

std::shared_ptr<AnswerToReset> SelectionStatus::getAtr() const
{
    return mAtr;
}

std::shared_ptr<ApduResponse> SelectionStatus::getFci() const
{
    return mFci;
}

bool SelectionStatus::hasMatched() const
{
    return mIsMatching;
}

bool SelectionStatus::operator==(const SelectionStatus& o) const
{
    return /* Both pointers are null or equal */
           ((!mAtr && !o.mAtr) ||
            *(mAtr.get()) == *(o.mAtr.get())) &&
           /* Both pointers are null or equal */
           ((!mFci && !o.mFci) ||
            *(mFci.get()) == *(o.mFci.get())) &&
           mIsMatching == o.mIsMatching;
}

bool SelectionStatus::operator!=(const SelectionStatus& o) const
{
    return !(*this == o);
}

std::ostream& operator<<(std::ostream& os, const SelectionStatus& s)
{
    os << "SELECTIONSTATUS: {"
       << "ATR = " << s.mAtr << ", "
       << "FCI = " << s.mFci << ", "
       << "HASMATCHED = " << s.mIsMatching << "}";

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
