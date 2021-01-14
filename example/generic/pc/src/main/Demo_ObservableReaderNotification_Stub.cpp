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

/* Example */
#include "ObservableReaderNotificationEngine.h"

/* Core */
#include "SeProxyService.h"
#include "ReaderPlugin.h"

/* Plugin */
#include "StubPlugin.h"
#include "StubReader.h"
#include "StubSecureElement.h"
#include "StubSe1.h"
#include "StubSe2.h"
#include "StubPluginFactory.h"

using namespace keyple::example::common::calypso::stub;
using namespace keyple::example::generic::common;
using namespace keyple::plugin::stub;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;

static const std::shared_ptr<void> waitBeforeEnd = nullptr;

class Demo_ObservableReaderNotification_Stub {};

static const std::shared_ptr<Logger> logger =
    LoggerFactory::getLogger(typeid(Demo_ObservableReaderNotification_Stub));

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    ObservableReaderNotificationEngine demoEngine;

    /* Set Stub plugin */
    SeProxyService& seProxyService = SeProxyService::getInstance();

    const std::string STUB_PLUGIN_NAME = "stub1";
    const std::string READER1_NAME = "Reader1";
    const std::string READER2_NAME = "Reader2";

    /* Register Stub plugin in the platform */
    auto stubFactory = std::make_shared<StubPluginFactory>(STUB_PLUGIN_NAME);
    auto factory = std::dynamic_pointer_cast<PluginFactory>(stubFactory);
    std::shared_ptr<ReaderPlugin> readerPlugin = seProxyService.registerPlugin(factory);

    /* Set observers */
    logger->info("Set plugin observer\n");
    demoEngine.setPluginObserver();

    logger->info("Wait a little to see the \"no reader available message\n");
    Thread::sleep(200);

    logger->info("Plug reader 1\n");
    std::dynamic_pointer_cast<StubPlugin>(readerPlugin)->plugStubReader(READER1_NAME, true);

    Thread::sleep(100);

    logger->info("Plug reader 2\n");
    std::dynamic_pointer_cast<StubPlugin>(readerPlugin)->plugStubReader(READER2_NAME, true);

    Thread::sleep(1000);

    auto reader1 = std::dynamic_pointer_cast<StubReader>(readerPlugin->getReader(READER1_NAME));
    auto reader2 = std::dynamic_pointer_cast<StubReader>(readerPlugin->getReader(READER1_NAME));

    /* Create 'virtual' Hoplink and SAM SE */
    std::shared_ptr<StubSecureElement> se1 = std::make_shared<StubSe1>();
    std::shared_ptr<StubSecureElement> se2 = std::make_shared<StubSe2>();

    logger->info("Insert SE into reader 1\n");
    reader1->insertSe(se1);

    Thread::sleep(100);

    logger->info("Insert SE into reader 2\n");
    reader2->insertSe(se2);

    Thread::sleep(100);

    logger->info("Remove SE from reader 1\n");
    reader1->removeSe();

    Thread::sleep(100);

    logger->info("Remove SE from reader 2\n");
    reader2->removeSe();

    Thread::sleep(100);

    logger->info("Plug reader 1 again (twice)\n");
    std::dynamic_pointer_cast<StubPlugin>(readerPlugin)->plugStubReader("Reader1", true);

    logger->info("Unplug reader 1\n");
    std::dynamic_pointer_cast<StubPlugin>(readerPlugin)->unplugStubReader("Reader1", true);

    Thread::sleep(100);

    logger->info("Plug reader 1 again\n");
    std::dynamic_pointer_cast<StubPlugin>(readerPlugin)->plugStubReader("Reader1", true);

    Thread::sleep(100);

    logger->info("Unplug reader 1\n");
    std::dynamic_pointer_cast<StubPlugin>(readerPlugin)->unplugStubReader("Reader1", true);

    Thread::sleep(100);

    logger->info("Unplug reader 2\n");
    std::dynamic_pointer_cast<StubPlugin>(readerPlugin)->unplugStubReader("Reader2", true);

    logger->info("END\n");

    return 0;
}
