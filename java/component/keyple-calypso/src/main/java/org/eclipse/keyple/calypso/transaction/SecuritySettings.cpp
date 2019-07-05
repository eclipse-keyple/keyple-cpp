/* Common */
#include "byte.h"

/* Calypso */
#include "KeypleCalypsoSecureSessionUnauthorizedKvcException.h"

#include "SecuritySettings.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace transaction {

using KeypleCalypsoSecureSessionUnauthorizedKvcException = org::eclipse::keyple::calypso::transaction::exception::KeypleCalypsoSecureSessionUnauthorizedKvcException;

SecuritySettings::SecuritySettings() {
    keySettings->put(DefaultKeyInfo::SAM_DEFAULT_KIF_PERSO, DEFAULT_KIF_PERSO);
    keySettings->put(DefaultKeyInfo::SAM_DEFAULT_KIF_LOAD, DEFAULT_KIF_LOAD);
    keySettings->put(DefaultKeyInfo::SAM_DEFAULT_KIF_DEBIT, DEFAULT_KIF_DEBIT);
    keySettings->put(DefaultKeyInfo::SAM_DEFAULT_KEY_RECORD_NUMBER, DEFAULT_KEY_RECORD_NUMER);
}

char SecuritySettings::putKeyInfo(DefaultKeyInfo keyInfo, Byte value) {
    return keySettings->put(keyInfo, value);
}

char SecuritySettings::getKeyInfo(DefaultKeyInfo keyInfo) {
    return keySettings->get(keyInfo);
}

void SecuritySettings::setAuthorizedKvcList(std::vector<Byte> &authorizedKvcList) {
    this->authorizedKvcList = authorizedKvcList;
}

bool SecuritySettings::isAuthorizedKvc(char kvc) {
    return authorizedKvcList.empty() || std::find(authorizedKvcList.begin(), authorizedKvcList.end(), kvc) != authorizedKvcList.end();
}

}
}
}
}
}
