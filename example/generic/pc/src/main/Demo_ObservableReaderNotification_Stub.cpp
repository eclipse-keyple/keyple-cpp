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

/* Example - Generic - Common */
#include "ObservableReaderNotificationEngine.h"

/* Core - Seproxy */
#include "SeProxyService.h"
#include "ReaderPlugin.h"

/* Plugin - Stub */
#include "StubPlugin.h"
#include "StubReader.h"
#include "StubSecureElement.h"
#include "StubSe1.h"
#include "StubSe2.h"

using namespace keyple::example::generic::common;
using namespace keyple::example::generic::pc::stub::se;
using namespace keyple::plugin::stub;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;

static const std::shared_ptr<void> waitBeforeEnd = nullptr;

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    std::shared_ptr<ObservableReaderNotificationEngine> demoEngine =
        std::make_shared<ObservableReaderNotificationEngine>();

    /*
     * Alex: diamond issue, casting PcscPlugin into ReaderPlugin can take two
     * routes:
     * - PcscPlugin -> AbstractThreadedObservablePlugin ->
     *   AbstractObservablePlugin -> ReaderPlugin
     * or
     * - PcscPlugin -> AbstractThreadedObservablePlugin -> ObservablePlugin ->
     *   ReaderPlugin
     *
     * Forcing conversion to ObservablePlugin for now but should be fixed or at
     * least validated.
     */
    SeProxyService seProxyService = SeProxyService::getInstance();
    std::set<std::shared_ptr<ReaderPlugin>> pluginsSet =
        std::set<std::shared_ptr<ReaderPlugin>>();

    StubPlugin& stubPlugin = StubPlugin::getInstance();
    stubPlugin.initReaders();
    pluginsSet.insert(std::dynamic_pointer_cast<ReaderPlugin>(
        std::dynamic_pointer_cast<ObservablePlugin>(
            std::make_shared<StubPlugin>(stubPlugin))));
    seProxyService.setPlugins(pluginsSet);

    // Set observers
    std::cout << "Set plugin observer." << std::endl;
    demoEngine->setPluginObserver();

    std::cout << "Wait a little to see the \"no reader available message\"."
              << std::endl;
    Thread::sleep(200);

    std::cout << "Plug reader 1." << std::endl;
    stubPlugin.plugStubReader("Reader1", true);

    Thread::sleep(100);

    std::cout << "Plug reader 2." << std::endl;
    stubPlugin.plugStubReader("Reader2", true);

    Thread::sleep(1000);

    std::shared_ptr<StubReader> reader1 =
        std::dynamic_pointer_cast<StubReader>(stubPlugin.getReader("Reader1"));

    std::shared_ptr<StubReader> reader2 =
        std::dynamic_pointer_cast<StubReader>(stubPlugin.getReader("Reader2"));

    /* Create 'virtual' Hoplink and SAM SE */
    std::shared_ptr<StubSecureElement> se1 = std::make_shared<StubSe1>();
    std::shared_ptr<StubSecureElement> se2 = std::make_shared<StubSe2>();

    std::cout << "Insert SE into reader 1." << std::endl;
    reader1->insertSe(se1);

    Thread::sleep(100);

    std::cout << "Insert SE into reader 2." << std::endl;
    reader2->insertSe(se2);

    Thread::sleep(100);

    std::cout << "Remove SE from reader 1." << std::endl;
    reader1->removeSe();

    Thread::sleep(100);

    std::cout << "Remove SE from reader 2." << std::endl;
    reader2->removeSe();

    Thread::sleep(100);

    std::cout << "Plug reader 1 again (twice)." << std::endl;
    stubPlugin.plugStubReader("Reader1", true);

    std::cout << "Unplug reader 1." << std::endl;
    stubPlugin.unplugStubReader("Reader1", true);

    Thread::sleep(100);

    std::cout << "Plug reader 1 again." << std::endl;
    stubPlugin.plugStubReader("Reader1", true);

    Thread::sleep(100);

    std::cout << "Unplug reader 1." << std::endl;
    stubPlugin.unplugStubReader("Reader1", true);

    Thread::sleep(100);

    std::cout << "Unplug reader 2." << std::endl;
    stubPlugin.unplugStubReader("Reader2", true);

    std::cout << "END." << std::endl;

    return 0;
}
