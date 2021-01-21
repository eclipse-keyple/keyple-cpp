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

/* Core */
#include "ReaderPlugin.h"
#include "SeCommonProtocols.h"
#include "SeProxyService.h"
#include "SeProtocol.h"
#include "SeReader.h"
#include "StubReader.h"

/* Stub plugin */
#include "StubPlugin.h"
#include "StubProtocolSetting.h"

/* Example */
#include "SeProtocolDetectionEngine.h"
#include "StubMifareClassic.h"
#include "StubMifareDesfire.h"
#include "StubMifareUL.h"
#include "StubPluginFactory.h"

using namespace keyple::example::common::calypso::stub;
using namespace keyple::example::generic::common;
using namespace keyple::plugin::stub;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::protocol;

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    /* Get the SeProxyService instance */
    SeProxyService& seProxyService = SeProxyService::getInstance();

    const std::string STUB_PLUGIN_NAME = "stub1";

    /* Register Stub plugin in the platform */
    auto stubFactory = std::make_shared<StubPluginFactory>(STUB_PLUGIN_NAME);
    auto factory = std::dynamic_pointer_cast<PluginFactory>(stubFactory);
    std::shared_ptr<ReaderPlugin> readerPlugin = seProxyService.registerPlugin(factory);

    /* Create an observer class to handle the SE operations */
    std::shared_ptr<SeProtocolDetectionEngine> observer =
        std::make_shared<SeProtocolDetectionEngine>();

    /* Plug PO reader */
    std::dynamic_pointer_cast<StubPlugin>(readerPlugin)->plugStubReader("poReader", true);

    Thread::sleep(200);

    std::shared_ptr<StubReader> poReader = nullptr;
    try {
        poReader = std::dynamic_pointer_cast<StubReader>(readerPlugin->getReader("poReader"));
    } catch (const KeypleReaderNotFoundException& e) {
        (void)e;
        //e.printStackTrace();
    }

    observer->setReader(poReader);

    /*
     * Protocol detection settings.
     * add 8 expected protocols with three different methods:
     * - using a custom enum
     * - adding protocols individually
     * A real application should use only one method.
     */

    /*
     * Method 1
     * add several settings at once with setting an unordered set
     */
    std::set<std::shared_ptr<SeCommonProtocol>> commonProtocols = {
        SeCommonProtocols::PROTOCOL_MIFARE_CLASSIC,
        SeCommonProtocols::PROTOCOL_MIFARE_UL
    };

    std::map<std::shared_ptr<SeProtocol>, std::string> map;
    const std::map<std::shared_ptr<SeCommonProtocol>, std::string> specificSettings =
        StubProtocolSetting::getSpecificSettings(commonProtocols);

    for (auto pair : specificSettings)
        map.insert({pair.first, pair.second});
    poReader->setSeProtocolSetting(map);

    /*
     * Method 2
     * append protocols individually
     */
    poReader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_MEMORY_ST25,
        StubProtocolSetting::STUB_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_MEMORY_ST25]);

    /* regex extended */
    poReader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_ISO14443_4,
        StubProtocolSetting::STUB_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO14443_4] +
        "|3B8D.*");

    /* Set terminal as Observer of the first reader */
    (std::static_pointer_cast<ObservableReader>(poReader))->addObserver(observer);

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

    return 0;
}
