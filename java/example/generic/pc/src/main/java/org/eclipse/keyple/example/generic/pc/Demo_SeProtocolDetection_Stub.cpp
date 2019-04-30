/* Core */
#include "ContactlessProtocols_Import.h"
#include "CustomProtocolSetting.h"
#include "KeypleReaderNotFoundException.h"
#include "ObservableReader.h"
#include "ReaderPlugin.h"
#include "SeProxyService.h"
#include "SeProtocolSetting.h"
#include "SeProtocol.h"
#include "SeReader.h"
#include "StubReader.h"

/* Stub plugin */
#include "StubPlugin.h"
#include "StubProtocolSetting_Import.h"

/* Example */
#include "SeProtocolDetectionEngine.h"
#include "StubMifareClassic.h"
#include "StubMifareDesfire.h"
#include "StubMifareUL.h"

/* Common */
#include "Thread.h"

using CustomProtocolSetting         = org::eclipse::keyple::example::generic::common::CustomProtocolSetting;
using SeProtocolDetectionEngine     = org::eclipse::keyple::example::generic::common::SeProtocolDetectionEngine;
using StubPlugin                    = org::eclipse::keyple::plugin::stub::StubPlugin;
using StubProtocolSetting           = org::eclipse::keyple::plugin::stub::StubProtocolSetting;
using StubReader                    = org::eclipse::keyple::plugin::stub::StubReader;
using ReaderPlugin                  = org::eclipse::keyple::seproxy::ReaderPlugin;
using SeProxyService                = org::eclipse::keyple::seproxy::SeProxyService;
using SeReader                      = org::eclipse::keyple::seproxy::SeReader;
using ObservableReader              = org::eclipse::keyple::seproxy::event::ObservableReader;
using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
using ContactlessProtocols          = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
using SeProtocol                    = org::eclipse::keyple::seproxy::protocol::SeProtocol;
using SeProtocolSetting             = org::eclipse::keyple::seproxy::protocol::SeProtocolSetting;
using StubMifareClassic             = org::eclipse::keyple::example::generic::pc::stub::se::StubMifareClassic;
using StubMifareDesfire             = org::eclipse::keyple::example::generic::pc::stub::se::StubMifareDesfire;
using StubMifareUL                  = org::eclipse::keyple::example::generic::pc::stub::se::StubMifareUL;

void main(int argc, char **argv)
{
    /* get the SeProxyService instance */
    SeProxyService seProxyService = SeProxyService::getInstance();

    /* add the PcscPlugin to the SeProxyService */
    std::set<std::shared_ptr<ReaderPlugin>> pluginsSet;

    StubPlugin stubPlugin = StubPlugin::getInstance();

    pluginsSet.insert(std::make_shared<StubPlugin>(stubPlugin));

    seProxyService.setPlugins(pluginsSet);

    /* create an observer class to handle the SE operations */
    std::shared_ptr<SeProtocolDetectionEngine> observer = std::make_shared<SeProtocolDetectionEngine>();

    /*
        * Plug PO reader.
        */
    stubPlugin.plugStubReader("poReader", true);

    Thread::sleep(200);

    std::shared_ptr<StubReader> poReader = nullptr, samReader = nullptr;
    try
    {
        poReader = std::dynamic_pointer_cast<StubReader>(stubPlugin.getReader("poReader"));
    } catch (const KeypleReaderNotFoundException &e)
    {
        e.printStackTrace();
    }

    observer->setReader(poReader);

    // Protocol detection settings.
    // add 8 expected protocols with three different methods:
    // - using addSeProtocolSetting
    // - using a custom enum
    // - using a protocol map and addSeProtocolSetting
    // A real application should use only one method.

    // Method 1
    // add protocols individually
    poReader->addSeProtocolSetting(std::dynamic_pointer_cast<SeProtocolSetting>(std::make_shared <StubProtocolSetting>(StubProtocolSetting::SETTING_PROTOCOL_MEMORY_ST25)));

    poReader->addSeProtocolSetting(std::dynamic_pointer_cast<SeProtocolSetting>(std::make_shared <StubProtocolSetting>(StubProtocolSetting::SETTING_PROTOCOL_ISO14443_4)));

    // Method 2
    // add all settings at once with setting enum
    poReader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(CustomProtocolSetting::values()));

    // Method 3
    // create and fill a protocol map
    std::unordered_map<std::shared_ptr<SeProtocol>, std::string> protocolsMap;

    protocolsMap.emplace(std::pair<std::shared_ptr<SeProtocol>, std::string>(std::make_shared<SeProtocol>(ContactlessProtocols::PROTOCOL_MIFARE_CLASSIC), StubProtocolSetting::ProtocolSetting::REGEX_PROTOCOL_MIFARE_CLASSIC));

    protocolsMap.emplace(std::pair<std::shared_ptr<SeProtocol>, std::string>(std::make_shared<SeProtocol>(ContactlessProtocols::PROTOCOL_MIFARE_UL), StubProtocolSetting::ProtocolSetting::REGEX_PROTOCOL_MIFARE_UL));

    // provide the reader with the map
    poReader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(protocolsMap));

    // Set terminal as Observer of the first reader
    (std::static_pointer_cast<ObservableReader>(poReader))->addObserver(observer);

    // poReader.insertSe(new StubCalypsoClassic());
    //
    // Thread.sleep(300);
    //
    // poReader.removeSe();

    // Thread.sleep(100);
    //
    // poReader.insertSe(new StubCalypsoBPrime());

    Thread::sleep(300);

    poReader->removeSe();

    Thread::sleep(100);

    poReader->insertSe(std::make_shared<StubMifareClassic>());

    Thread::sleep(300);

    poReader->removeSe();

    Thread::sleep(100);

    /* insert Mifare UltraLight */
    poReader->insertSe(std::make_shared<StubMifareUL>());

    Thread::sleep(300);

    poReader->removeSe();

    Thread::sleep(100);

    /* insert Mifare Desfire */
    poReader->insertSe(std::make_shared<StubMifareDesfire>());

    Thread::sleep(300);

    poReader->removeSe();

    Thread::sleep(100);
}
