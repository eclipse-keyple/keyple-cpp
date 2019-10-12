
#include "CalypsoClassicTransactionEngine.h"
#include "KeypleReaderNotFoundException.h"
#include "Logger.h"
#include "LoggerFactory.h"
#include "ObservableReader_Import.h"
#include "ReaderPlugin.h"
#include "SeCommonProtocols_Import.h"
#include "SeProxyService.h"
#include "StubCalypsoClassic.h"
#include "StubSamCalypsoClassic.h"
#include "StubProtocolSetting_Import.h"
#include "StubPlugin.h"
#include "StubReader.h"

using namespace org::eclipse::keyple::example::calypso::common::transaction;
using namespace org::eclipse::keyple::example::calypso::pc::stub::se;
using namespace org::eclipse::keyple::plugin::stub;
using namespace org::eclipse::keyple::core::seproxy;
using namespace org::eclipse::keyple::core::seproxy::event;
using namespace org::eclipse::keyple::core::seproxy::exception;
using namespace org::eclipse::keyple::core::seproxy::protocol;

class Demo_CalypsoClassic_Stub {
};

std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(Demo_CalypsoClassic_Stub));

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    /* Get the instance of the SeProxyService (Singleton pattern) */
    SeProxyService seProxyService = SeProxyService::getInstance();

    std::set<std::shared_ptr<ReaderPlugin>> pluginsSet;


    StubPlugin& stubPlugin = StubPlugin::getInstance();
    stubPlugin.initReaders();

    /* Get the instance of the PcscPlugin (Singleton pattern) */
    pluginsSet.insert(std::make_shared<StubPlugin>(stubPlugin));

    /* Assign StubPlugin to the SeProxyService */
    seProxyService.setPlugins(pluginsSet);

    /* Setting up the transaction engine (implements Observer) */
    std::shared_ptr<CalypsoClassicTransactionEngine> transactionEngine = std::make_shared<CalypsoClassicTransactionEngine>();

    /*
     * Plug PO and SAM stub readers.
     */
    stubPlugin.plugStubReader("poReader", true);
    stubPlugin.plugStubReader("samReader", true);

    std::shared_ptr<StubReader> poReader = nullptr, samReader = nullptr;
    try {
        poReader = std::dynamic_pointer_cast<StubReader>(stubPlugin.getReader("poReader"));
        samReader = std::dynamic_pointer_cast<StubReader>(stubPlugin.getReader("samReader"));
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

    /* Set the PO reader protocol flag */
    poReader->addSeProtocolSetting(SeCommonProtocols::PROTOCOL_ISO14443_4,
                                   StubProtocolSetting::STUB_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO14443_4]);
    poReader->addSeProtocolSetting(SeCommonProtocols::PROTOCOL_B_PRIME,
                                   StubProtocolSetting::STUB_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_B_PRIME]);
    poReader->addSeProtocolSetting(SeCommonProtocols::PROTOCOL_ISO7816_3,
                                   StubProtocolSetting::STUB_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO7816_3]);

    /* Assign readers to the Hoplink transaction engine */
    transactionEngine->setReaders(poReader, samReader);

    /* Create 'virtual' Hoplink and SAM SE */
    std::shared_ptr<StubSecureElement> calypsoStubSe = std::make_shared<StubCalypsoClassic>();
    std::shared_ptr<StubSecureElement> samSE = std::make_shared<StubSamCalypsoClassic>();

    /* Insert the SAM into the SAM reader */
    logger->info("Insert stub SAM SE\n");
    samReader->insertSe(samSE);

    /* Set the default selection operation */
    (std::static_pointer_cast<ObservableReader>(poReader))->setDefaultSelectionRequest(transactionEngine->preparePoSelection(), ObservableReader::NotificationMode::MATCHED_ONLY);

    /* Set the transactionEngine as Observer of the PO reader */
    (std::static_pointer_cast<ObservableReader>(poReader))->addObserver(transactionEngine);

    logger->info("Insert stub PO SE\n");
    poReader->insertSe(calypsoStubSe);

    Thread::sleep(1000);

    /* Remove SE */
    logger->info("Remove stub SAM and PO SE\n");

    poReader->removeSe();
    samReader->removeSe();

    logger->info("END.");

    exit(0);
}

