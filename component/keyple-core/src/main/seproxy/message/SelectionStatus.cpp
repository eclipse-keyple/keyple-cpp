#include "SelectionStatus.h"
#include "AnswerToReset.h"
#include "ApduResponse.h"
#include "Arrays.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace message {

SelectionStatus::SelectionStatus(std::shared_ptr<AnswerToReset> atr, std::shared_ptr<ApduResponse> fci, bool isMatching)
: atr(atr), fci(fci), isMatching(isMatching)
{
    if (atr == nullptr && fci == nullptr) {
        throw std::invalid_argument("Atr and Fci can't be null at the same time.");
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

bool SelectionStatus::hasMatched()
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

    std::shared_ptr<SelectionStatus> selectionStatus = std::static_pointer_cast<SelectionStatus>(o);

    return selectionStatus->getAtr() == nullptr ? this->atr == nullptr :
           selectionStatus->getAtr()->equals(this->atr) && selectionStatus->getFci() == nullptr ? this->fci == nullptr :
           selectionStatus->getFci()->equals(this->fci) && selectionStatus->hasMatched() == isMatching;
}

int SelectionStatus::hashCode()
{
    int hash = 17;
    hash = 19 * hash + (isMatching ? 0 : 1);
    hash = 31 * hash + (atr == nullptr ? 0 : Arrays::hashCode(atr->getBytes()));
    hash = 7 * hash + (fci == nullptr ? 0 : Arrays::hashCode(fci->getBytes()));
    return hash;
}


void SelectionStatus::finalize()
{

}

}
}
}
}
}
}
