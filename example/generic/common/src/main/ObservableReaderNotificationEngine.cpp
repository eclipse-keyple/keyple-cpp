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
#include "KeyplePluginNotFoundException.h"
#include "KeypleReaderNotFoundException.h"
#include "PluginEvent.h"
#include "ReaderEvent.h"
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
    std::shared_ptr<SpecificReaderObserver> readerObserver =
        std::make_shared<SpecificReaderObserver>(this);

    logger->debug("instanciating pluginObserver\n");
    pluginObserver =
        std::make_shared<SpecificPluginObserver>(this, readerObserver);
}

void ObservableReaderNotificationEngine::setPluginObserver()
{
    logger->debug("\n");

    /*
     * We add an observer to each plugin (only one in this example) the readers
     * observers will be aded dynamically upon plugin notification (see
     * SpecificPluginObserver.update)
     */
    for (auto plugin : SeProxyService::getInstance().getPlugins()) {

        if (dynamic_cast<ObservablePlugin*>(plugin) != nullptr) {
            logger->info("add observer PLUGINNAME = %\n", plugin->getName());

            (dynamic_cast<ObservablePlugin*>(plugin))
                ->addObserver(this->pluginObserver);
        } else {
            logger->info("PLUGINNAME = % not observable\n", plugin->getName());
        }
    }
}

ObservableReaderNotificationEngine::SpecificReaderObserver
    ::SpecificReaderObserver(ObservableReaderNotificationEngine* outerInstance)
: outerInstance(outerInstance)
{
}

void ObservableReaderNotificationEngine::SpecificReaderObserver::update(
    ReaderEvent event)
{
    (void)event;
    /* Should not be used, compilation fix */
}

void ObservableReaderNotificationEngine::SpecificReaderObserver::update(
    std::shared_ptr<ReaderEvent> event)
{
    switch (event->getEventType().innerEnumValue) {
    case ReaderEvent::EventType::InnerEnum::SE_MATCHED:
        /*
         * Informs the underliying later of the end of the SE processing, in
         * order to manage the removal sequence.
         */
        try {
            std::shared_ptr<ObservableReader> observableReader =
                std::dynamic_pointer_cast<ObservableReader>(
                    SeProxyService::getInstance().getPlugin(
                        event->getPluginName())->getReader(
                            event->getReaderName()));
            observableReader->notifySeProcessed();
        } catch (KeypleReaderNotFoundException& e) {
            outerInstance->logger->debug(
                "update - KeypleReaderNotFoundException: %\n", e);
        } catch (KeyplePluginNotFoundException& e) {
            outerInstance->logger->debug(
                "update - KeyplePluginNotFoundException: %\n", e);
        }
        break;

    case ReaderEvent::EventType::InnerEnum::SE_INSERTED:
        /* End of the SE processing is automatically done */
        break;
    default:
        outerInstance->logger->debug("unhandled case %\n",
			                         event->getEventType());
    }

    /* just log the event */
    outerInstance->logger->info("event %\n", event);
}

ObservableReaderNotificationEngine::SpecificPluginObserver
    ::SpecificPluginObserver(
        ObservableReaderNotificationEngine* outerInstance,
        std::shared_ptr<SpecificReaderObserver> readerObserver)
: outerInstance(outerInstance)
{
    this->readerObserver = readerObserver;
}

void ObservableReaderNotificationEngine::SpecificPluginObserver::update(
    PluginEvent event)
{
    (void)event;
    /* Should not be used, compilation fix */
}

void ObservableReaderNotificationEngine::SpecificPluginObserver::update(
    std::shared_ptr<PluginEvent> event)
{
    for (auto readerName : event->getReaderNames()) {
        std::shared_ptr<SeReader> reader = nullptr;
        outerInstance->logger->info("event: %\n", event);

        /* We retrieve the reader object from its name. */
        try {
            reader = SeProxyService::getInstance()
                         .getPlugin(event->getPluginName())
                         ->getReader(readerName);
        } catch (KeyplePluginNotFoundException& e) {
            (void)e;
        } catch (KeypleReaderNotFoundException& e) {
            (void)e;
        }

        switch (event->getEventType().innerEnumValue) {
        case PluginEvent::EventType::InnerEnum::READER_CONNECTED:
            outerInstance->logger->info("new reader! READERNAME = %\n",
                                        reader->getName());

            /*
             * We are informed here of a disconnection of a reader.
             *
             * We add an observer to this reader if this is possible.
             */
            if (std::dynamic_pointer_cast<ObservableReader>(reader) !=
                nullptr) {
                if (readerObserver != nullptr) {
                    outerInstance->logger->info("add observer READERNAME = %\n",
                                                reader->getName());

                    (std::dynamic_pointer_cast<ObservableReader>(reader))
                        ->addObserver(readerObserver);
                } else {
                    outerInstance->logger->info("no observer to add " \
						                        "READERNAME = %\n",
                                                reader->getName());
                }
            }
            break;
        case PluginEvent::EventType::InnerEnum::READER_DISCONNECTED:
            /*
             * We are informed here of a disconnection of a reader.
             *
             * The reader object still exists but will be removed from the
             * reader list right after. Thus, we can properly remove the
             * observer attached to this reader before the list update.
             */
            outerInstance->logger->info("reader removed. READERNAME = %\n",
                                        readerName);

            if (std::dynamic_pointer_cast<ObservableReader>(reader) !=
                nullptr) {
                if (readerObserver != nullptr) {
                    outerInstance->logger->info("remove observer READERNAME "
                                                "= %\n", readerName);
                    (std::dynamic_pointer_cast<ObservableReader>(reader))
                        ->removeObserver(readerObserver);
                } else {
                    outerInstance->logger->info("unplugged reader READERNAME"
                                                "= % wasn't observed\n",
                                                readerName);
                }
            }
            break;
        default:
            outerInstance->logger->info("unexpected reader event: %\n",
                                        event->getEventType());
            break;
        }
    }
}
}
}
}
}
