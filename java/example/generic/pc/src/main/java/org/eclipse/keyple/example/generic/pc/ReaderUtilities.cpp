#include "ReaderUtilities.h"
#include "SeProxyService.h"
#include "SeReader.h"
#include "KeypleReaderException.h"
#include "KeypleReaderNotFoundException.h"
#include "KeypleBaseException.h"
#include "PcscReadersSettings.h"
#include "PcscReader.h"
#include "SeProtocolSetting.h"
#include "PcscProtocolSetting.h"
#include "ReaderPlugin.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace generic {
                    namespace pc {
                        using PcscProtocolSetting = org::eclipse::keyple::plugin::pcsc::PcscProtocolSetting;
                        using PcscReader = org::eclipse::keyple::plugin::pcsc::PcscReader;
                        using ReaderPlugin = org::eclipse::keyple::seproxy::ReaderPlugin;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using SeReader = org::eclipse::keyple::seproxy::SeReader;
                        using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                        using SeProtocolSetting = org::eclipse::keyple::seproxy::protocol::SeProtocolSetting;

                        std::shared_ptr<SeReader> ReaderUtilities::getReaderByName(std::shared_ptr<SeProxyService> seProxyService, const std::string &pattern)
                        {
                            Pattern* p = Pattern::compile(pattern);
                            for (auto plugin : seProxyService->getPlugins()) {
                                for (auto reader : *plugin->getReaders()) {
                                    if (p->matcher(reader->getName())->matches()) {
                                        return reader;
                                    }
                                }
                            }
                            throw std::make_shared<KeypleReaderNotFoundException>("Reader name pattern: " + pattern);
                        }

                        std::shared_ptr<SeReader> ReaderUtilities::getDefaultContactLessSeReader(std::shared_ptr<SeProxyService> seProxyService)
                        {
                            std::shared_ptr<SeReader> seReader = ReaderUtilities::getReaderByName(seProxyService, PcscReadersSettings::PO_READER_NAME_REGEX);

                            ReaderUtilities::setContactlessSettings(seReader);

                            return seReader;
                        }

                        void ReaderUtilities::setContactlessSettings(std::shared_ptr<SeReader> reader)
                        {
                            /* Enable logging */
                            reader->setParameter(PcscReader::SETTING_KEY_LOGGING, "true");

                            /* Contactless SE works with T1 protocol */
                            reader->setParameter(PcscReader::SETTING_KEY_PROTOCOL, PcscReader::SETTING_PROTOCOL_T1);

                            /*
                             * PC/SC card access mode:
                             *
                             * The SAM is left in the SHARED mode (by default) to avoid automatic resets due to the
                             * limited time between two consecutive exchanges granted by Windows.
                             *
                             * The PO reader is set to EXCLUSIVE mode to avoid side effects during the selection step
                             * that may result in session failures.
                             *
                             * These two points will be addressed in a coming release of the Keyple PcSc reader plugin.
                             */
                            reader->setParameter(PcscReader::SETTING_KEY_MODE, PcscReader::SETTING_MODE_SHARED);

                            /* Set the PO reader protocol flag */
                            reader->addSeProtocolSetting(std::dynamic_pointer_cast<SeProtocolSetting>(std::make_shared<PcscProtocolSetting>(PcscProtocolSetting::SETTING_PROTOCOL_ISO14443_4)));

                        }

                        void ReaderUtilities::setContactsSettings(std::shared_ptr<SeReader> reader)
                        {
                            /* Enable logging */
                            reader->setParameter(PcscReader::SETTING_KEY_LOGGING, "true");

                            /* Contactless SE works with T0 protocol */
                            reader->setParameter(PcscReader::SETTING_KEY_PROTOCOL, PcscReader::SETTING_PROTOCOL_T0);

                            /*
                             * PC/SC card access mode:
                             *
                             * The SAM is left in the SHARED mode (by default) to avoid automatic resets due to the
                             * limited time between two consecutive exchanges granted by Windows.
                             *
                             * The PO reader is set to EXCLUSIVE mode to avoid side effects during the selection step
                             * that may result in session failures.
                             *
                             * These two points will be addressed in a coming release of the Keyple PcSc reader plugin.
                             */
                            reader->setParameter(PcscReader::SETTING_KEY_MODE, PcscReader::SETTING_MODE_SHARED);
                        }
                    }
                }
            }
        }
    }
}
