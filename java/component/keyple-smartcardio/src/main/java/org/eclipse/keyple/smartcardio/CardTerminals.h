#pragma once

#include <vector>

#if defined(WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include <winscard.h>
#else
#include <PCSC/winscard.h>
#include <PCSC/wintypes.h>
#endif

#include "CardException.h"

class CardTerminals {

  public:
    /**
	 * Returns an unmodifiable list of all available terminals.
	 *
	 * @return an unmodifiable list of all available terminals.
	 *
	 * @throws CardException if the card operation failed
	 */
    std::vector<CardTerminal> list() throw(CardException)
    {
        LONG ret;
        char *pszReaderList = nullptr;
        DWORD len;
        char *pszReader;

        logger->debug("[CardTerminal::list] listing current readers\n");
        ret = SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &ctx);
        if (ret != SCARD_S_SUCCESS)
        {
            logger->debug("[CardTerminal::list] error establishing context\n");
            return {};
        }

        terminals.empty();

#ifdef SCARD_AUTOALLOCATE
        len = SCARD_AUTOALLOCATE;

        ret = SCardListReaders(ctx, NULL, (LPTSTR)&pszReaderList, &len);
        if (ret != SCARD_S_SUCCESS)
        {
            logger->debug("[CardTerminal::list] error listing readers (%x)\n", ret);
            throw(CardException("error listing readers", std::runtime_error("SCARD_E_NO_READERS_AVAILABLE")));
        }
#else
        ret = SCardListReaders(ctx, NULL, NULL, &len);
        if (ret != SCARD_S_SUCCESS)
        {
            logger->debug("[CardTerminal::list] error listing readers\n");
            return {};
        }

        pszReaderList = (char *)calloc(len, sizeof(char));
        ret           = SCardListReaders(ctx, NULL, pszReaderList, &len);
        if (ret != SCARD_S_SUCCESS)
        {
            logger->debug("[CardTerminal::list] error listing readers (2)\n");
            return {};
        }
#endif

        pszReader = pszReaderList;
        while (*pszReader)
        {
            std::string s(pszReader);
            logger->debug("[CardTerminal::list] adding reader '%s' to list\n", s);
            terminals.push_back(CardTerminal(s));
            pszReader += strlen(pszReader) + 1;
        }

#ifdef SCARD_AUTOALLOCATE
        SCardFreeMemory(ctx, pszReaderList);
#else
        free(pszReaderList);
#endif

        return terminals;
    }

    /**
	 * constructor
	 *
	 * Constructs a new CardTerminals object.
	 *
	 * <p>This constructor is called by subclasses only. Application should
     * call {@linkplain TerminalFactory#terminals} to obtain a CardTerminals
	 * object.
     */
    CardTerminals()
    {
        logger->debug("[CardTerminal::CardTerminal]\n");
    }

  protected:
    /**
	 *
	 */
    SCARDCONTEXT ctx;

  private:
    /**
	 *
	 */
    std::vector<CardTerminal> terminals;

    /**
	 *
	 */
    std::unique_ptr<Logger> logger = LoggerFactory::getLogger(typeid(this));
};
