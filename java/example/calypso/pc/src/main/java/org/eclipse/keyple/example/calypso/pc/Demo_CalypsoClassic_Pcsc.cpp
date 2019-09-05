#include "KeypleBaseException.h"
#include "SeProxyService.h"
#include "PcscPlugin.h"
#include "CalypsoClassicTransactionEngine.h"
#include "SeReader.h"
#include "PcscReadersSettings.h"
#include "ReaderUtilities.h"
#include "KeypleReaderNotFoundException.h"
#include "Logger.h"
#include "LoggerFactory.h"
#include "PcscReader.h"
#include "PcscReaderSettings_Import.h"
#include "PcscReadersSettings.h"
#include "PcscProtocolSetting.h"
#include "PcscReadersSettings.h"
#include "ObservableReader.h"

using namespace org::eclipse::keyple::common;
using namespace org::eclipse::keyple::core::seproxy;
using namespace org::eclipse::keyple::core::seproxy::event;
using namespace org::eclipse::keyple::core::seproxy::exception;
using namespace org::eclipse::keyple::core::seproxy::protocol;
using namespace org::eclipse::keyple::example::calypso::common::transaction;
using namespace org::eclipse::keyple::example::calypso::pc;
using namespace org::eclipse::keyple::example::generic::pc;
using namespace org::eclipse::keyple::plugin::pcsc;

class Demo_CalypsoClassic_Pcsc {
};

std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(Demo_CalypsoClassic_Pcsc));

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

     /* Get the instance of the PC/SC plugin */
    PcscPlugin pcscPlugin = PcscPlugin::getInstance();
    pcscPlugin.initReaders();
    std::shared_ptr<PcscPlugin> shared_plugin = std::shared_ptr<PcscPlugin>(&pcscPlugin);

    /* Assign PcscPlugin to the SeProxyService */
    SeProxyService seProxyService = SeProxyService::getInstance();
    seProxyService.addPlugin(shared_plugin);

    /* Setting up the transaction engine (implements Observer) */
    std::shared_ptr<CalypsoClassicTransactionEngine> transactionEngine = std::make_shared<CalypsoClassicTransactionEngine>();

    /*
     * Get PO and SAM readers. Apply regulars expressions to reader names to select PO / SAM
     * readers. Use the getReader helper method from the transaction engine.
     */
    std::shared_ptr<SeReader> poReader = nullptr, samReader = nullptr;
    try {
        poReader = ReaderUtilities::getReaderByName(PcscReadersSettings::PO_READER_NAME_REGEX);
        samReader = ReaderUtilities::getReaderByName(PcscReadersSettings::SAM_READER_NAME_REGEX);
    }
    catch (const KeypleReaderNotFoundException &e) {
        e.printStackTrace();
    }

    /* Both readers are expected not null */
    if (poReader == samReader || poReader == nullptr || samReader == nullptr) {
        throw std::make_shared<IllegalStateException>("Bad PO/SAM setup");
    }

    logger->info("PO Reader  NAME = %s\n", poReader->getName());
    logger->info("SAM Reader  NAME = %s\n", samReader->getName());

    /* Set PcSc settings per reader */
    poReader->setParameter(PcscReaderSettings::SETTING_KEY_LOGGING, "true");
    poReader->setParameter(PcscReaderSettings::SETTING_KEY_PROTOCOL, PcscReaderSettings::SETTING_PROTOCOL_T1);
    samReader->setParameter(PcscReaderSettings::SETTING_KEY_LOGGING, "true");
    samReader->setParameter(PcscReaderSettings::SETTING_KEY_PROTOCOL, PcscReaderSettings::SETTING_PROTOCOL_T0);

    /*
     * PC/SC card access mode:
     *
     * The SAM is left in the SHARED mode (by default) to avoid automatic resets due to the
     * limited time between two consecutive exchanges granted by Windows.
     *
     * This point will be addressed in a coming release of the Keyple PcSc reader plugin.
     *
     * The PO reader is set to EXCLUSIVE mode to avoid side effects (on OS Windows 8+) during
     * the selection step that may result in session failures.
     *
     * See KEYPLE-CORE.PC.md file to learn more about this point.
     *
     */
    samReader->setParameter(PcscReader::SETTING_KEY_MODE, PcscReader::SETTING_MODE_SHARED);
    poReader->setParameter(PcscReader::SETTING_KEY_MODE, PcscReader::SETTING_MODE_SHARED);

    /* Set the PO reader protocol flag */
    poReader->addSeProtocolSetting(SeCommonProtocols::PROTOCOL_ISO14443_4,
                                   PcscProtocolSetting::PCSC_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO14443_4]);
    poReader->addSeProtocolSetting(SeCommonProtocols::PROTOCOL_B_PRIME,
                                   PcscProtocolSetting::PCSC_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_B_PRIME]);
    poReader->addSeProtocolSetting(SeCommonProtocols::PROTOCOL_ISO7816_3,
                                   PcscProtocolSetting::PCSC_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO7816_3]);

    /* Assign the readers to the Calypso transaction engine */
    transactionEngine->setReaders(poReader, samReader);

    /* Set the default selection operation */
    (std::dynamic_pointer_cast<ObservableReader>(poReader))->setDefaultSelectionRequest(transactionEngine->preparePoSelection(),
                                                                                       ObservableReader::NotificationMode::MATCHED_ONLY);

    /* Set terminal as Observer of the first reader */
    (std::dynamic_pointer_cast<ObservableReader>(poReader))->addObserver(transactionEngine);

    while(1);
}

