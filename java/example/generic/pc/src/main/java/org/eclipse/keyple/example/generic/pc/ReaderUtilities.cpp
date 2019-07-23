#include "ReaderUtilities.h"
#include "SeProxyService.h"
#include "SeReader.h"
#include "KeypleReaderException.h"
#include "KeypleReaderNotFoundException.h"
#include "KeypleBaseException.h"
#include "PcscReadersSettings.h"
#include "PcscReader_Import.h"
#include "PcscProtocolSetting_Import.h"
#include "ReaderPlugin.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace example {
namespace generic {
namespace pc {

using namespace org::eclipse::keyple::core::seproxy::protocol;
using namespace org::eclipse::keyple::core::seproxy;
using namespace org::eclipse::keyple::core::seproxy::exception;
using namespace org::eclipse::keyple::plugin::pcsc;

std::shared_ptr<SeReader> ReaderUtilities::getReaderByName(std::shared_ptr<SeProxyService> seProxyService, const std::string &pattern)
{
    Pattern* p = Pattern::compile(pattern);
    for (auto plugin : seProxyService->getPlugins()) {
        for (auto reader : *plugin->getReaders()) {
            Matcher *matcher = p->matcher(reader->getName());
            if (matcher->matches()) {
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
    reader->addSeProtocolSetting(SeCommonProtocols::PROTOCOL_ISO14443_4,
              PcscProtocolSetting::PCSC_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO14443_4]);

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

