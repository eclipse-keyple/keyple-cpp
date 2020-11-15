/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#include "CalypsoClassicTransactionEngine.h"

/* Common */
#include "LoggerFactory.h"
#include "IllegalStateException.h"

/* Plugin */
#include "PcscProtocolSetting.h"
#include "PcscReader.h"
#include "PcscPluginFactory.h"

/* Example */
#include "PcscReadersSettings.h"
#include "ReaderUtilities.h"

using namespace keyple::common;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::example::common;
using namespace keyple::example::calypso::common::transaction;
using namespace keyple::plugin::pcsc;

class Demo_CalypsoClassic_Pcsc {
};

std::shared_ptr<Logger> logger =
    LoggerFactory::getLogger(typeid(Demo_CalypsoClassic_Pcsc));

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    logger->setLoggerLevel(Logger::Level::logTrace);

    /* Get the instance of the SeProxyService (Singleton pattern) */
    SeProxyService& seProxyService = SeProxyService::getInstance();

    /* Assign PcscPlugin to the SeProxyService */
    seProxyService.registerPlugin(std::make_shared<PcscPluginFactory>());

    /* Setting up the transaction engine (implements Observer) */
    std::shared_ptr<CalypsoClassicTransactionEngine> transactionEngine =
        std::make_shared<CalypsoClassicTransactionEngine>();

    /*
     * Get PO and SAM readers. Apply regulars expressions to reader names to
     * select PO / SAM readers. Use the getReader helper method from the
     * transaction engine.
     */
    std::shared_ptr<SeReader> poReader = nullptr, samReader = nullptr;
    try {
        poReader = ReaderUtilities::getReaderByName(PcscReadersSettings::PO_READER_NAME_REGEX);
        samReader = ReaderUtilities::getReaderByName(PcscReadersSettings::SAM_READER_NAME_REGEX);
    } catch (const KeypleReaderNotFoundException& e) {
        logger->error("update - KeypleReaderNotFoundException: %\n", e);
    }

    /* Both readers are expected not null */
    if (poReader == samReader || poReader == nullptr || samReader == nullptr) {
        throw IllegalStateException("Bad PO/SAM setup");
    }

    logger->info("PO Reader  NAME = %\n", poReader->getName());
    logger->info("SAM Reader  NAME = %\n", samReader->getName());

    /* Set PcSc settings per reader */
    poReader->setParameter(PcscReader::SETTING_KEY_PROTOCOL, PcscReader::SETTING_PROTOCOL_T1);
    samReader->setParameter(PcscReader::SETTING_KEY_PROTOCOL, PcscReader::SETTING_PROTOCOL_T0);

    /*
     * PC/SC card access mode:
     *
     * The SAM is left in the SHARED mode (by default) to avoid automatic resets due to the limited
     * time between two consecutive exchanges granted by Windows.
     *
     * This point will be addressed in a coming release of the Keyple PcSc reader plugin.
     *
     * The PO reader is set to EXCLUSIVE mode to avoid side effects (on OS Windows 8+) during the
     * selection step that may result in session failures.
     *
     * See KEYPLE-CORE.PC.md file to learn more about this point.
     *
     */
    samReader->setParameter(PcscReader::SETTING_KEY_MODE, PcscReader::SETTING_MODE_SHARED);
    poReader->setParameter(PcscReader::SETTING_KEY_MODE, PcscReader::SETTING_MODE_SHARED);

    /* Set the PO reader protocol flag */
    poReader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_ISO14443_4,
        PcscProtocolSetting::PCSC_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO14443_4]);
    poReader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_B_PRIME,
        PcscProtocolSetting::PCSC_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_B_PRIME]);
    samReader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_ISO7816_3,
        PcscProtocolSetting::PCSC_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO7816_3]);

    /* Assign the readers to the Calypso transaction engine */
    transactionEngine->setReaders(poReader, samReader);

    /* Set terminal as Observer of the first reader */
    (std::dynamic_pointer_cast<ObservableReader>(poReader))->addObserver(transactionEngine);

    /* Set the default selection operation */
    (std::dynamic_pointer_cast<ObservableReader>(poReader))
        ->setDefaultSelectionRequest(transactionEngine->preparePoSelection(),
                                     ObservableReader::NotificationMode::MATCHED_ONLY,
                                     ObservableReader::PollingMode::REPEATING);

    while (1);
}
