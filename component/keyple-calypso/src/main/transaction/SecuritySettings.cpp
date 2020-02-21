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

/* Common */
#include "Byte.h"

/* Calypso */
#include "KeypleCalypsoSecureSessionUnauthorizedKvcException.h"

#include "SecuritySettings.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::transaction::exception;

SecuritySettings::SecuritySettings()
{
    keySettings.insert(std::pair<DefaultKeyInfo, Byte>(DefaultKeyInfo::SAM_DEFAULT_KIF_PERSO, Byte(DEFAULT_KIF_PERSO)));
    keySettings.insert(std::pair<DefaultKeyInfo, Byte>(DefaultKeyInfo::SAM_DEFAULT_KIF_LOAD, Byte(DEFAULT_KIF_LOAD)));
    keySettings.insert(std::pair<DefaultKeyInfo, Byte>(DefaultKeyInfo::SAM_DEFAULT_KIF_DEBIT, Byte(DEFAULT_KIF_DEBIT)));
    keySettings.insert(std::pair<DefaultKeyInfo, Byte>(DefaultKeyInfo::SAM_DEFAULT_KEY_RECORD_NUMBER, Byte(DEFAULT_KEY_RECORD_NUMER)));
}

char SecuritySettings::putKeyInfo(DefaultKeyInfo keyInfo, Byte value)
{
    char old = keySettings[keyInfo].byteValue();

    keySettings.insert(std::pair<DefaultKeyInfo, Byte>(keyInfo, value));

    return old;
}

char SecuritySettings::getKeyInfo(DefaultKeyInfo keyInfo)
{
    return keySettings[keyInfo].byteValue();
}

void SecuritySettings::setAuthorizedKvcList(std::vector<Byte> &authorizedKvcList)
{
    this->authorizedKvcList = authorizedKvcList;
}

bool SecuritySettings::isAuthorizedKvc(char kvc)
{
    return authorizedKvcList.empty() || std::find(authorizedKvcList.begin(), authorizedKvcList.end(), Byte(kvc)) != authorizedKvcList.end();
}

}
}
}
