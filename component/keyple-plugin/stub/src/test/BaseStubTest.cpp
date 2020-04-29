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

#include "BaseStubTest.h"
#include "StubPlugin.h"
#include "StubPluginFactory.h"
#include "KeypleReaderException.h"
#include "StubPluginImpl.h"
#include "ObservablePlugin.h"

namespace keyple {
namespace plugin {
namespace stub {

using Logger                = keyple::common::Logger;
using LoggerFactory         = keyple::common::LoggerFactory;
using namespace std::chrono; // nanoseconds, system_clock, seconds
using namespace keyple::core::seproxy::event;

const std::shared_ptr<Logger> BaseStubTest::logger =
    LoggerFactory::getLogger( typeid(BaseStubTest) );

const std::string BaseStubTest::PLUGIN_NAME = "stub1";

void BaseStubTest::setUp()
{
    logger->info("------------------------------\n");
    logger->info("Test %s\n", typeid(name).name() );//->getMethodName());
    logger->info("------------------------------\n");

    logger->info("setUp, assert stubplugin is empty\n");
    stubPlugin = dynamic_cast<StubPluginImpl*>(
                    &((new StubPluginFactory(PLUGIN_NAME))
                           ->getPluginInstance()));

    logger->info("Stubplugin readers size %d\n",
                 stubPlugin->getReaders().size());
    ASSERT_EQ(0, stubPlugin->getReaders().size());

    logger->info("Stubplugin observers size %d\n",
                 stubPlugin->AbstractThreadedObservablePlugin::countObservers());
    ASSERT_EQ(0, stubPlugin->AbstractThreadedObservablePlugin::countObservers());

    // add a sleep to play with thread monitor timeout
    sleep(1);
}

void BaseStubTest::tearDown()
{
    logger->info("---------\n");
    logger->info("TearDown \n");
    logger->info("---------\n");

    stubPlugin->unplugStubReaders(stubPlugin->getReaderNames(), true);

    //AbstractObservableReader reader = stubPlugin->getReaders;
    {
        //logger->info("Stubplugin unplugStubReader {}", reader.getName());
        //stubPlugin->unplugStubReader(reader.getName(), true);
        sleep(100);
        logger->debug("Stubplugin readers size %d\n",
                      stubPlugin->getReaders().size());
    }

    stubPlugin->clearObservers();
}

void BaseStubTest::TestBoby()
{
}

}
}
}
