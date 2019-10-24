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

#include "CardTerminals.h"

namespace keyple {
namespace smartcardio {

using namespace keyple::common;

std::vector<CardTerminal> CardTerminals::list()
{
    LONG ret;
    char *pszReaderList = nullptr;
    DWORD len;
    char *pszReader;

    logger->debug("listing current readers\n");
    ret = SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &this->ctx);
    if (ret != SCARD_S_SUCCESS)
    {
        logger->debug("error establishing context\n");
        return {};
    }

    terminals.empty();

    ret = SCardListReaders(ctx, NULL, NULL, &len);
    if (ret != SCARD_S_SUCCESS)
    {
        logger->debug("error listing readers\n");
        return {};
    }

    pszReaderList = (char *)calloc(len, sizeof(char));
    ret           = SCardListReaders(ctx, NULL, pszReaderList, &len);
    if (ret != SCARD_S_SUCCESS)
    {
        logger->debug("error listing readers (2)\n");
        return {};
    }


    pszReader = pszReaderList;
    while (*pszReader)
    {
        std::string s(pszReader);
        logger->debug("adding reader '%s' to list\n", s);
        terminals.push_back(CardTerminal(this->ctx, s));
        pszReader += strlen(pszReader) + 1;
    }

    free(pszReaderList);


    return terminals;
}

CardTerminals::CardTerminals()
{
    logger->debug("constructor\n");
}

CardTerminals::~CardTerminals()
{
    logger->debug("destructor\n");
}

}
}
