/* Core */
#include "ContactlessProtocols_Import.h"
#include "KeypleBaseException.h"
#include "ObservableReader.h"
#include "SeProtocolSetting.h"
#include "SeProtocol.h"
#include "SeProxyService.h"
#include "SeReader.h"

/* Plugin */
#include "PcscPlugin.h"
#include "PcscReader.h"
#include "PcscProtocolSetting.h"

/* Example */
#include "CustomProtocolSetting.h"
#include "ReaderUtilities.h"
#include "SeProtocolDetectionEngine.h"
#include "PcscReadersSettings.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace generic {
                    namespace pc {

                        using CustomProtocolSetting =
                            org::eclipse::keyple::example::generic::common::CustomProtocolSetting;
                        using SeProtocolDetectionEngine =
                            org::eclipse::keyple::example::generic::common::SeProtocolDetectionEngine;
                        using PcscPlugin          = org::eclipse::keyple::plugin::pcsc::PcscPlugin;
                        using PcscProtocolSetting = org::eclipse::keyple::plugin::pcsc::PcscProtocolSetting;
                        using PcscReader          = org::eclipse::keyple::plugin::pcsc::PcscReader;
                        using PcscReadersSettings =
                            org::eclipse::keyple::example::generic::pc::PcscReadersSettings;
                        using SeProxyService   = org::eclipse::keyple::seproxy::SeProxyService;
                        using SeReader         = org::eclipse::keyple::seproxy::SeReader;
                        using ObservableReader = org::eclipse::keyple::seproxy::event::ObservableReader;
                        using KeypleBaseException =
                            org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                        using ContactlessProtocols =
                            org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                        using SeProtocol        = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                        using SeProtocolSetting = org::eclipse::keyple::seproxy::protocol::SeProtocolSetting;
                        using ReaderUtilities   = org::eclipse::keyple::example::generic::pc::ReaderUtilities;
                        using ReaderPlugin      = org::eclipse::keyple::seproxy::ReaderPlugin;

                        int main(int argc, char **argv) throw(std::invalid_argument, KeypleBaseException)
                        {
                            /* get the SeProxyService instance */
                            SeProxyService seProxyService = SeProxyService::getInstance();

                            /* add the PcscPlugin to the SeProxyService */
                            PcscPlugin pcscPlugin = PcscPlugin::getInstance();
                            seProxyService.addPlugin(std::make_shared<ReaderPlugin>(pcscPlugin));

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
                            std::shared_ptr<SeProtocolDetectionEngine> observer =
                                std::make_shared<SeProtocolDetectionEngine>();

                            observer->setReader(poReader);

                            /* configure reader */
                            poReader->setParameter(PcscReader::SETTING_KEY_PROTOCOL,
                                                   PcscReader::SETTING_PROTOCOL_T1);

                            // Protocol detection settings.
                            // add 8 expected protocols with three different methods:
                            // - using addSeProtocolSetting
                            // - using a custom enum
                            // - using a protocol map and addSeProtocolSetting
                            // A real application should use only one method.

                            // Method 1
                            // add protocols individually
                            poReader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(
                                PcscProtocolSetting::SETTING_PROTOCOL_MEMORY_ST25));

                            poReader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(
                                PcscProtocolSetting::SETTING_PROTOCOL_ISO14443_4));

                            // Method 2
                            // add all settings at once with setting enum
                            poReader->addSeProtocolSetting(
                                std::make_shared<SeProtocolSetting>(CustomProtocolSetting::values()));

                            // Method 3
                            // create and fill a protocol map
                            std::unordered_map<std::shared_ptr<SeProtocol>, std::string> protocolsMap;

                            protocolsMap.emplace(
                                ContactlessProtocols::PROTOCOL_MIFARE_CLASSIC,
                                PcscProtocolSetting::ProtocolSetting::REGEX_PROTOCOL_MIFARE_CLASSIC);

                            protocolsMap.emplace(
                                ContactlessProtocols::PROTOCOL_MIFARE_UL,
                                PcscProtocolSetting::ProtocolSetting::REGEX_PROTOCOL_MIFARE_UL);

                            // provide the reader with the map
                            poReader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(protocolsMap));

                            // Set terminal as Observer of the first reader
                            (std::static_pointer_cast<ObservableReader>(poReader))->addObserver(observer);

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
                    } // namespace pc
                }     // namespace generic
            }         // namespace example
        }             // namespace keyple
    }                 // namespace eclipse
} // namespace org
