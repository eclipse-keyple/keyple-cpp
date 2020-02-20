/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "BaseStubTest.h"
#include "StubPluginTest.h"
#include "StubPlugin.h"
#include "SeProxyService.h"
#include "SeProtocol_Import.h"
#include "SeReader.h"
#include "StubReader.h"

using namespace testing;
using keyple::plugin::stub::StubPluginTest;
using keyple::plugin::stub::StubPlugin;
using keyple::plugin::stub::BaseStubTest;
using keyple::core::seproxy::SeProxyService;

TEST(StubPluginTest, testA_PlugOneReaderCount) 
{
    /* Get the instance of the SeProxyService (Singleton pattern) */
    SeProxyService seProxyService = SeProxyService::getInstance();

    /* Get the instance of the Stub plugin */
    StubPlugin& stubplugin = StubPlugin::getInstance();
    stubplugin.initReaders();

    std::shared_ptr<StubPlugin> shared_stub = std::make_shared<StubPlugin>(stubplugin);
    ASSERT_NE( shared_stub, nullptr);
}

TEST( StubPluinTest, testX_essai )
{
    /* Get the instance of the SeProxyService (Singleton pattern) */
    SeProxyService seProxyService = SeProxyService::getInstance();

    /* Get the instance of the Stub plugin */
    StubPlugin& stubPlugin = StubPlugin::getInstance();
    stubPlugin.initReaders();

    std::shared_ptr<StubPlugin> shared_stub = std::make_shared<StubPlugin>(stubPlugin);
    ASSERT_NE( shared_stub, nullptr);

    /* Assign StubPlugin to the SeProxyService */
    seProxyService.addPlugin(shared_stub);

    /* Plug the PO stub reader */
    stubPlugin.plugStubReader("poReader", true);

    /* Plug the SAM stub reader. */
    stubPlugin.plugStubReader("samReader", true);
}

TEST(StubPluginTest, testA_PlugOneReaderEvent) 
{
    //std::shared_ptr<StubPluginTest> LocalTest = std::make_shared<StubPluginTest>();

    //LocalTest->testA_PlugOneReaderEvent();
}

TEST(StubPluginTest, testA_UnplugOneReaderCount) 
{
    //std::shared_ptr<StubPluginTest> LocalTest = std::make_shared<StubPluginTest>();

    //LocalTest->testA_UnplugOneReaderCount();
}

TEST(StubPluginTest, testB_UnplugOneReaderEvent) 
{
    //std::shared_ptr<StubPluginTest> LocalTest = std::make_shared<StubPluginTest>();

    //LocalTest->testB_UnplugOneReaderEvent();
}

int main(int argc, char **argv)
{
    /* Initialize GTest */
    ::InitGoogleTest(&argc, argv);

    /* Run */
    return RUN_ALL_TESTS();
}
