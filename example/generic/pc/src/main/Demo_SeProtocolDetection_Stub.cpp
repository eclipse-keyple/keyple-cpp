/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

/* Core */
#include "KeypleReaderNotFoundException.h"
#include "ObservableReader_Import.h"
#include "ReaderPlugin.h"
#include "SeCommonProtocols_Import.h"
#include "SeProxyService.h"
#include "SeProtocol_Import.h"
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
#include "StubPluginFactory.h"

/* Common */
#include "Thread.h"

using namespace keyple::example::generic::common;
using namespace keyple::plugin::stub;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::example::generic::pc::stub::se;

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    /* get the SeProxyService instance */
    SeProxyService& seProxyService = SeProxyService::getInstance();

    const std::string stubPluginName = "stub1";
    std::unique_ptr<StubPluginFactory> stubPluginFactory =
        std::unique_ptr<StubPluginFactory>(
            new StubPluginFactory(stubPluginName));

    /* Register Stub plugin in the platform */
    seProxyService.registerPlugin(stubPluginFactory.get());
    ReaderPlugin* stubPlugin = seProxyService.getPlugin(stubPluginName);

    /* create an observer class to handle the SE operations */
    std::shared_ptr<SeProtocolDetectionEngine> observer =
        std::make_shared<SeProtocolDetectionEngine>();

    /* Plug PO reader */
    (dynamic_cast<StubPlugin*>(stubPlugin))->plugStubReader("poReader", true);

    Thread::sleep(200);

    std::shared_ptr<StubReader> poReader = nullptr, samReader = nullptr;
    try {
        poReader = std::dynamic_pointer_cast<StubReader>(
            stubPlugin->getReader("poReader"));
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
    std::set<SeCommonProtocols> commonProtocols{
        SeCommonProtocols::PROTOCOL_MIFARE_CLASSIC,
        SeCommonProtocols::PROTOCOL_MIFARE_UL};
    std::map<SeProtocol, std::string> map;
    std::map<SeCommonProtocols, std::string> specificSettings =
        StubProtocolSetting::getSpecificSettings(commonProtocols);
    for (auto pair : specificSettings)
        map.insert(std::pair<SeProtocol, std::string>(pair.first, pair.second));
    poReader->setSeProtocolSetting(map);

    /*
     * Method 2
     * append protocols individually
     */
    poReader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_MEMORY_ST25,
        StubProtocolSetting::STUB_PROTOCOL_SETTING
            [SeCommonProtocols::PROTOCOL_MEMORY_ST25]);

    /* regex extended */
    poReader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_ISO14443_4,
        StubProtocolSetting::STUB_PROTOCOL_SETTING
                [SeCommonProtocols::PROTOCOL_ISO14443_4] +
            "|3B8D.*");

    /* Set terminal as Observer of the first reader */
    (std::static_pointer_cast<ObservableReader>(poReader))
        ->addObserver(observer);

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

    return 0;
}
