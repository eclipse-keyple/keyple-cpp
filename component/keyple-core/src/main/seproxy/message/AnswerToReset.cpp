#include "Arrays.h"
#include "AnswerToReset.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace message {

AnswerToReset::AnswerToReset(std::vector<char> &atrBytes) {
    this->atrBytes = atrBytes;
}

std::vector<char> AnswerToReset::getBytes() {
    return atrBytes;
}

bool AnswerToReset::equals(std::shared_ptr<void> o) {
    if (o == shared_from_this()) {
        return true;
    }
    if (!(std::static_pointer_cast<AnswerToReset>(o) != nullptr)) {
        return false;
    }

    std::shared_ptr<AnswerToReset> atr = std::static_pointer_cast<AnswerToReset>(o);
    return Arrays::equals(atr->getBytes(), this->atrBytes);
}

int AnswerToReset::hashCode() {
    int hash = 17;
    hash = 19 * hash + (atrBytes.empty() ? 0 : Arrays::hashCode(atrBytes));
    return hash;
}

void AnswerToReset::finalize()
{

}

}
}
}
}
}
}
