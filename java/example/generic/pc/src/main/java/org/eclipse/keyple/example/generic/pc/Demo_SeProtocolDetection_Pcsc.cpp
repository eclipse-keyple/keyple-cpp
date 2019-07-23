/* Core */
#include "KeypleBaseException.h"
#include "ObservableReader.h"
#include "SeProtocol.h"
#include "SeProxyService.h"
#include "SeReader.h"

/* Plugin */
#include "PcscPlugin.h"
#include "PcscReader_Import.h"
#include "PcscProtocolSetting_Import.h"

/* Example */
#include "ReaderUtilities.h"
#include "SeProtocolDetectionEngine.h"
#include "PcscReadersSettings.h"


using namespace org::eclipse::keyple::core::seproxy;
using namespace org::eclipse::keyple::core::seproxy::event;
using namespace org::eclipse::keyple::core::seproxy::exception;
using namespace org::eclipse::keyple::core::seproxy::protocol;
using namespace org::eclipse::keyple::example::generic::common;
using namespace org::eclipse::keyple::example::generic::pc;
using namespace org::eclipse::keyple::plugin::pcsc;

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    /* get the SeProxyService instance */
    SeProxyService seProxyService = SeProxyService::getInstance();

    /* add the PcscPlugin to the SeProxyService */
    PcscPlugin pcscPlugin = PcscPlugin::getInstance();
    pcscPlugin.initReaders();
    seProxyService.addPlugin(std::dynamic_pointer_cast<ReaderPlugin>(std::make_shared<PcscPlugin>(pcscPlugin)));

    /* attempt to get the SeReader (the right reader should be ready here) */
    std::shared_ptr<SeReader> poReader = ReaderUtilities::getReaderByName(
	std::make_shared<SeProxyService>(seProxyService),
	PcscReadersSettings::PO_READER_NAME_REGEX);

    if (poReader == nullptr)
    {
	throw std::make_shared<IllegalStateException>("Bad PO/SAM setup");
    }

    std::cout << "PO Reader  : " << poReader->getName() << std::endl;
    poReader->setParameter(PcscReader::SETTING_KEY_LOGGING, "true");

    /* create an observer class to handle the SE operations */
    std::shared_ptr<SeProtocolDetectionEngine> observer = std::make_shared<SeProtocolDetectionEngine>();

    observer->setReader(poReader);

    /* configure reader */
    poReader->setParameter(PcscReader::SETTING_KEY_PROTOCOL, PcscReader::SETTING_PROTOCOL_T1);

    // Protocol detection settings.
    // add 8 expected protocols with three different methods:
    // - using a custom enum
    // - addinf protocols individually
    // A real application should use only one method.

    // Method 1
    // add several settings at once with settings an unordered_set
    std::set<SeCommonProtocols> commonProtocols {SeCommonProtocols::PROTOCOL_MIFARE_CLASSIC, SeCommonProtocols::PROTOCOL_MIFARE_UL};
    std::unordered_map<SeProtocol, std::string> map;
    std::unordered_map<SeCommonProtocols, std::string> specificSettings = PcscProtocolSetting::getSpecificSettings(commonProtocols);
    for (auto pair : specificSettings)
         map.insert(std::pair<SeProtocol, std::string>(pair.first, pair.second));
    poReader->setSeProtocolSetting(map);

    // Method 2
    // add all settings at once with setting enum
    poReader->addSeProtocolSetting(SeCommonProtocols::PROTOCOL_MEMORY_ST25,
                                   PcscProtocolSetting::PCSC_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_MEMORY_ST25]);

    // regiex extended
    poReader->addSeProtocolSetting(SeCommonProtocols::PROTOCOL_ISO14443_4,
                                   PcscProtocolSetting::PCSC_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO14443_4] + "|3B8D.*");

    // Set terminal as Observer of the first reader
    (std::dynamic_pointer_cast<ObservableReader>(poReader))->addObserver(observer);

    // wait for Enter key to exit.
    std::cout << "Press Enter to exit" << std::endl;

    while (true)
    {
	char c = 0;
	try
	{
	    std::string input = "";
	    getline(std::cin, input);
	    if (input.length())
		c = input[0];

	} catch (const IOException &e)
	{
	    e.printStackTrace();
	}
	if (c == 0x0A)
	{
	    std::cout << "Exiting..." << std::endl;
	    exit(0);
	}
    }
}

