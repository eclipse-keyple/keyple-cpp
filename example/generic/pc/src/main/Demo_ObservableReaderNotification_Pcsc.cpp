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

/* Example */
#include "ObservableReaderNotificationEngine.h"

/* Core */
#include "SeProxyService.h"
#include "ReaderPlugin.h"
#include "ObservablePlugin.h"

/* Plugin PC/SC */
#include "PcscPlugin.h"
#include "PcscPluginFactory.h"

using namespace keyple::example::generic::common;
using namespace keyple::plugin::pcsc;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    std::shared_ptr<ObservableReaderNotificationEngine> demoEngine =
        std::make_shared<ObservableReaderNotificationEngine>();

    /* Get the instance of the SeProxyService (Singleton pattern) */
    SeProxyService& seProxyService = SeProxyService::getInstance();

    /* Assign PcscPlugin to the SeProxyService */
    seProxyService.registerPlugin(new PcscPluginFactory());

    /* Set observers */
    demoEngine->setPluginObserver();

    std::cout << "Wait for reader or SE insertion/removal" << std::endl;

    /* Wait indefinitely. CTRL-C to exit. */
    while (1);
}
