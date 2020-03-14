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

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "BaseStubTest.h"
#include "StubPluginTest.h"
#include "StubPlugin.h"
#include "StubPluginFactory.h"
#include "StubPluginImpl.h"
#include "SeProxyService.h"
#include "SeProtocol_Import.h"
#include "SeReader.h"
#include "StubReader.h"

using namespace testing;
using namespace keyple::plugin::stub;
using namespace keyple::core::seproxy;

TEST(StubPluginTest, testA_PlugOneReaderCount)
{
    const std::string pluginName = "test_A";

    /* Get the instance of the SeProxyService (Singleton pattern) */
    //SeProxyService& seProxyService = SeProxyService::getInstance();

    /* Get the instance of the Stub plugin */
    StubPluginImpl& stubplugin =
       dynamic_cast<StubPluginImpl&>(
           (new StubPluginFactory(pluginName))->getPluginInstance());

    (void)stubplugin;
    //std::shared_ptr<StubPlugin> shared_stub =
    //    std::make_shared<StubPlugin>(stubplugin);
    //ASSERT_NE( shared_stub, nullptr);
}

TEST( StubPluinTest, testX_essai )
{
    const std::string pluginName = "test_X";

    /* Get the instance of the SeProxyService (Singleton pattern) */
    //SeProxyService& seProxyService = SeProxyService::getInstance();

    /* Get the instance of the Stub plugin */
    StubPluginImpl& stubplugin =
       dynamic_cast<StubPluginImpl&>(
           (new StubPluginFactory(pluginName))->getPluginInstance());

    (void)stubplugin;
    //std::shared_ptr<StubPlugin> shared_stub =
    //    std::make_shared<StubPlugin>(stubPlugin);
    //ASSERT_NE( shared_stub, nullptr);

    /* Assign StubPlugin to the SeProxyService */
    //seProxyService.addPlugin(shared_stub);

    /* Plug the PO stub reader */
    //stubPlugin.plugStubReader("poReader", true);

    /* Plug the SAM stub reader. */
    //stubPlugin.plugStubReader("samReader", true);
}

TEST(StubPluginTest, testA_PlugOneReaderEvent)
{
    //std::shared_ptr<StubPluginTest> LocalTest =
    //    std::make_shared<StubPluginTest>();

    //LocalTest->testA_PlugOneReaderEvent();
}

TEST(StubPluginTest, testA_UnplugOneReaderCount)
{
    //std::shared_ptr<StubPluginTest> LocalTest =
    //    std::make_shared<StubPluginTest>();

    //LocalTest->testA_UnplugOneReaderCount();
}

TEST(StubPluginTest, testB_UnplugOneReaderEvent)
{
    //std::shared_ptr<StubPluginTest> LocalTest =
    //    std::make_shared<StubPluginTest>();

    //LocalTest->testB_UnplugOneReaderEvent();
}

int main(int argc, char **argv)
{
    /* Initialize GTest */
    ::InitGoogleTest(&argc, argv);

    /* Run */
    return RUN_ALL_TESTS();
}
