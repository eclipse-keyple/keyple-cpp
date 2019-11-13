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

/* Core */
#include "KeyplePluginNotFoundException.h"
#include "KeypleReaderNotFoundException.h"
#include "PluginEvent.h"
#include "ReaderEvent_Import.h"
#include "SeProxyService.h"
#include "SeReader.h"

/* Example */
#include "ObservableReaderNotificationEngine.h"

namespace keyple {
namespace example {
namespace generic {
namespace common {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;

ObservableReaderNotificationEngine::ObservableReaderNotificationEngine()
{
    /* initializes observers */
    logger->debug("casting readerObserver\n");
    std::shared_ptr<SpecificReaderObserver> readerObserver = std::make_shared<SpecificReaderObserver>(this);

    logger->debug("instanciating pluginObserver\n");
    pluginObserver = std::make_shared<SpecificPluginObserver>(this, readerObserver);
}

void ObservableReaderNotificationEngine::setPluginObserver()
{
    logger->debug("\n");

    /*
     * We add an observer to each plugin (only one in this example) the readers observers will
     * be added dynamically upon plugin notification (see SpecificPluginObserver.update)
     */
    for (auto plugin : SeProxyService::getInstance().getPlugins())
    {

        if (std::dynamic_pointer_cast<ObservablePlugin>(plugin) != nullptr)
        {
            logger->info("add observer PLUGINNAME = %s\n", plugin->getName());
            logger->info("this->pluginObserver: %p\n", this->pluginObserver);
            (std::dynamic_pointer_cast<ObservablePlugin>(plugin))->addObserver(this->pluginObserver);
        }
        else
        {
            logger->info("PLUGINNAME = %s isn't observable\n", plugin->getName());
        }
    }
}

//JAVA TO C++ CONVERTER TODO TASK: No base class can be determined:
ObservableReaderNotificationEngine::SpecificReaderObserver::SpecificReaderObserver(
    ObservableReaderNotificationEngine *outerInstance)
    : outerInstance(outerInstance)
{
}

void ObservableReaderNotificationEngine::SpecificReaderObserver::update(ReaderEvent event)
{
    (void)event;
     /* Should not be used, compilation fix */
}

void ObservableReaderNotificationEngine::SpecificReaderObserver::update(std::shared_ptr<ReaderEvent> event)
{
    /* just log the event */
    outerInstance->logger->info(
        "event -> pluginname: %s, readername: %s, eventname: %s\n",
        event->getPluginName(), event->getReaderName(),
        event->getEventType().getName());
}

ObservableReaderNotificationEngine::SpecificPluginObserver::SpecificPluginObserver(
    ObservableReaderNotificationEngine *outerInstance,
    std::shared_ptr<SpecificReaderObserver> readerObserver)
    : outerInstance(outerInstance)
{
    this->readerObserver = readerObserver;
}

void ObservableReaderNotificationEngine::SpecificPluginObserver::update(PluginEvent event)
{
    (void)event;
     /* Should not be used, compilation fix */
}

void ObservableReaderNotificationEngine::SpecificPluginObserver::update(std::shared_ptr<PluginEvent> event)
{
    for (auto readerName : *event->getReaderNames()) {
        std::shared_ptr<SeReader> reader = nullptr;
        outerInstance->logger->info("PluginEvent: PLUGINNAME = %s, READERNAME = %s, EVENTTYPE = %s\n",
                                    event->getPluginName(), reader->getName(), event->getEventType());

        /* We retrieve the reader object from its name. */
        try {
            reader = SeProxyService::getInstance().getPlugin(event->getPluginName())->getReader(reader->getName());
        } catch (KeyplePluginNotFoundException &e) {

        } catch (KeypleReaderNotFoundException &e) {

        }

        switch (event->getEventType().innerEnumValue)
        {
        case PluginEvent::EventType::InnerEnum::READER_CONNECTED:
            outerInstance->logger->info("new reader! READERNAME = %s\n", reader->getName());

            /*
             * We are informed here of a disconnection of a reader.
             *
             * We add an observer to this reader if this is possible.
             */
            if (std::dynamic_pointer_cast<ObservableReader>(reader) != nullptr)
            {
                if (readerObserver != nullptr)
                {
                    outerInstance->logger->info("add observer READERNAME = %s\n", reader->getName());
                    (std::dynamic_pointer_cast<ObservableReader>(reader))->addObserver(readerObserver);
                }
                else
                {
                    outerInstance->logger->info("no observer to add READERNAME = %s\n", reader->getName());
                }
            }
            break;
        case PluginEvent::EventType::InnerEnum::READER_DISCONNECTED:
            /*
             * We are informed here of a disconnection of a reader.
             *
             * The reader object still exists but will be removed from the reader list right
             * after. Thus, we can properly remove the observer attached to this reader
             * before the list update.
             */
            outerInstance->logger->info("reader removed. READERNAME = %s\n", readerName);
            if (std::dynamic_pointer_cast<ObservableReader>(reader) != nullptr)
            {
                if (readerObserver != nullptr)
                {
                    outerInstance->logger->info("remove observer READERNAME = %s\n", readerName);
                    (std::dynamic_pointer_cast<ObservableReader>(reader))->removeObserver(readerObserver);
                }
                else
                {
                    outerInstance->logger->info( "unplugged reader READERNAME = %s wasn't observed\n", readerName);
                }
            }
            break;
        default:
            outerInstance->logger->info("unexpected reader event. EVENT = %s\n", event->getEventType().getName());
            break;
        }
    }
 }
}
}
}
}
