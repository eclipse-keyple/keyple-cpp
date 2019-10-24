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

#include "KeypleReaderNotFoundException.h"
#include "KeypleBaseException.h"
#include "PcscReadersSettings.h"
#include "PcscReader_Import.h"
#include "PcscProtocolSetting_Import.h"
#include "ReaderPlugin.h"
#include "ReaderUtilities.h"
#include "SeCommonProtocols_Import.h"
#include "SeProxyService.h"
#include "SeReader.h"

namespace keyple {
namespace example {
namespace generic {
namespace pc {

using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::exception;
using namespace keyple::plugin::pcsc;

std::shared_ptr<SeReader> ReaderUtilities::getReaderByName(const std::string &pattern)
{
    Pattern* p = Pattern::compile(pattern);
    for (auto plugin : SeProxyService::getInstance().getPlugins()) {
        for (auto reader : *plugin->getReaders()) {
            Matcher *matcher = p->matcher(reader->getName());
            if (matcher->matches()) {
                return reader;
            }
        }
    }
    throw std::make_shared<KeypleReaderNotFoundException>("Reader name pattern: " + pattern);
}

std::shared_ptr<SeReader> ReaderUtilities::getDefaultContactLessSeReader()
{
    std::shared_ptr<SeReader> seReader = ReaderUtilities::getReaderByName(PcscReadersSettings::PO_READER_NAME_REGEX);
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

    /* Set the SAM reader protocol flag */
    reader->addSeProtocolSetting(SeCommonProtocols::PROTOCOL_ISO7816_3,
                                 PcscProtocolSetting::PCSC_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO7816_3]);
}

}
}
}
}

