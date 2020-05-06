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

#include "SeProxyServiceTest.h"
#include "SeProxyService.h"
#include "ReaderPlugin.h"
#include "KeyplePluginNotFoundException.h"


namespace keyple {
namespace core {
namespace seproxy {

using KeyplePluginNotFoundException =
    keyple::core::seproxy::exception::KeyplePluginNotFoundException;

const std::shared_ptr<org::slf4j::Logger> SeProxyServiceTest::logger =
    org::slf4j::LoggerFactory::getLogger(SeProxyServiceTest::typeid);
std::string SeProxyServiceTest::PLUGIN_NAME = "plugin1";

void SeProxyServiceTest::setupBeforeEach()
{

    // init class to test
    proxyService = SeProxyService::getInstance();
}

void SeProxyServiceTest::testGetInstance()
{
    // test
    assertNotNull(proxyService);
    // assertNull(proxyService);
}

void SeProxyServiceTest::testGetVersion()
{
    // test that version follows semver guidelines
    std::string regex = "^([0-9]+)\\.([0-9]+)\\.([0-9]+)(?:-([0-9A-Za-z-]+(?:"
                        "\\.[0-9A-Za-z-]+)*))?(?:\\+[0-9A-Za-z-]+)?$";
    std::string version = SeProxyService::getInstance()->getVersion();
    logger->info("Version of SeProxyService {}", version);
    std::cout << "Version of SeProxyService " << version << std::endl;
    assertTrue(std::regex_match(version, std::regex(regex)));
}

void SeProxyServiceTest::testGetSetPlugins()
{
    // init
    std::shared_ptr<ConcurrentSkipListSet<std::shared_ptr<ReaderPlugin>>>
        plugins = getPluginList();

    // test
    proxyService->setPlugins(plugins);
    assertArrayEquals(plugins,
                      proxyService->getPlugins());
}

void SeProxyServiceTest::testGetPlugin() throw(KeyplePluginNotFoundException)
{
    // init

    std::shared_ptr<ConcurrentSkipListSet<std::shared_ptr<ReaderPlugin>>>
        plugins = getPluginList();

    proxyService->setPlugins(plugins);

    // test
    assertEquals(plugin1, proxyService->getPlugin(PLUGIN_NAME));
}

void SeProxyServiceTest::testGetPluginFail() throw(std::runtime_error)
{

    // init
    std::shared_ptr<ConcurrentSkipListSet<std::shared_ptr<ReaderPlugin>>>
        plugins = getPluginList();
    proxyService->setPlugins(plugins);

    // test
    proxyService->getPlugin("unknown"); // Throw exception
}

std::shared_ptr<ConcurrentSkipListSet<std::shared_ptr<ReaderPlugin>>>
SeProxyServiceTest::getPluginList()
{

    // ReaderPlugin plugin2 = Mockito.mock(ReaderPlugin.class);
    // when(plugin2.getName()).thenReturn(PLUGIN_NAME_2);

    when(plugin1->getName()).thenReturn(PLUGIN_NAME);
    std::shared_ptr<ConcurrentSkipListSet<std::shared_ptr<ReaderPlugin>>>
        plugins = std::make_shared<
            ConcurrentSkipListSet<std::shared_ptr<ReaderPlugin>>>();

    plugins->add(plugin1);
    // plugins.add(plugin2);

    assertEquals(1, plugins->size()); // impossible to add 2 ReaderPlugin mocks

    return plugins;
}
}
}
}
}
