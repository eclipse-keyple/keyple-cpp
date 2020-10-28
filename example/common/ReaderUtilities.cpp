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

#include "ReaderUtilities.h"

/* Common */
#include "Pattern.h"

/* Core */
#include "KeypleReaderNotFoundException.h"
#include "KeypleException.h"
#include "ReaderPlugin.h"
#include "SeCommonProtocols.h"
#include "SeProxyService.h"
#include "SeReader.h"

/* Plugin */
#include "PcscReadersSettings.h"
#include "PcscReader.h"
#include "PcscProtocolSetting.h"

namespace keyple {
namespace example {
namespace common {

using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::exception;
using namespace keyple::plugin::pcsc;

ReaderUtilities::ReaderUtilities() {}

std::shared_ptr<SeReader> ReaderUtilities::getReaderByName(const std::string& pattern)
{
    Pattern* p = Pattern::compile(pattern);
    const auto& plugins = SeProxyService::getInstance().getPlugins();

    for (const auto& plugin_it : plugins) {
        for (const auto& reader_it : plugin_it.second->getReaders()) {
            if (p->matcher(reader_it.second->getName())->matches())
                return reader_it.second;
        }
    }
    throw KeypleReaderNotFoundException("Reader name pattern: " + pattern);
}

std::shared_ptr<SeReader> ReaderUtilities::getDefaultContactLessSeReader()
{
    std::shared_ptr<SeReader> seReader =
        ReaderUtilities::getReaderByName(PcscReadersSettings::PO_READER_NAME_REGEX);

    ReaderUtilities::setContactlessSettings(seReader);

    return seReader;
}

void ReaderUtilities::setContactlessSettings(std::shared_ptr<SeReader> reader)
{
    /* Contactless SE works with T1 protocol */
    reader->setParameter(PcscReader::SETTING_KEY_PROTOCOL, PcscReader::SETTING_PROTOCOL_T1);

    /*
     * PC/SC card access mode:
     *
     * The SAM is left in the SHARED mode (by default) to avoid automatic resets due to the limited
     * time between two consecutive exchanges granted by Windows.
     *
     * The PO reader is set to EXCLUSIVE mode to avoid side effects during the selection step that
     * may result in session failures.
     *
     * These two points will be addressed in a coming release of the Keyple PC/SC reader plugin.
     */
    reader->setParameter(PcscReader::SETTING_KEY_MODE, PcscReader::SETTING_MODE_SHARED);

    /* Set the PO reader protocol flag */
    reader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_ISO14443_4,
        PcscProtocolSetting::PCSC_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO14443_4]);
}

void ReaderUtilities::setContactsSettings(std::shared_ptr<SeReader> reader)
{
    /* Contactless SE works with T0 protocol */
    reader->setParameter(PcscReader::SETTING_KEY_PROTOCOL,
                         PcscReader::SETTING_PROTOCOL_T0);

    /*
     * PC/SC card access mode:
     *
     * The SAM is left in the SHARED mode (by default) to avoid automatic resets due to the limited
     * time between two consecutive exchanges granted by Windows.
     *
     * The PO reader is set to EXCLUSIVE mode to avoid side effects during the selection step that
     * may result in session failures.
     *
     * These two points will be addressed in a coming release of the Keyple PC/SC reader plugin.
     */
    reader->setParameter(PcscReader::SETTING_KEY_MODE, PcscReader::SETTING_MODE_SHARED);

    /* Set the SAM reader protocol flag */
    reader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_ISO7816_3,
        PcscProtocolSetting::PCSC_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO7816_3]);
}

}
}
}
