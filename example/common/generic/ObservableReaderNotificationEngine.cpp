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

using SpecificReaderObserver = ObservableReaderNotificationEngine::SpecificReaderObserver;
using SpecificPluginObserver = ObservableReaderNotificationEngine::SpecificPluginObserver;

/* OBSERVABLE READER NOTIFICATION ENGINE -------------------------------------------------------- */

ObservableReaderNotificationEngine::ObservableReaderNotificationEngine()
{
    /* initializes observers */
    auto readerObserver = std::make_shared<SpecificReaderObserver>();
    mPluginObserver = std::make_shared<SpecificPluginObserver>(readerObserver);
}

void ObservableReaderNotificationEngine::setPluginObserver()
{
    /*
     * We add an observer to each plugin (only one in this example) the readers
     * observers will be aded dynamically upon plugin notification (see
     * SpecificPluginObserver.update)
     */
    for (auto plugin_it : SeProxyService::getInstance().getPlugins()) {
        std::shared_ptr<ReaderPlugin> plugin = plugin_it.second;
        auto observable = std::dynamic_pointer_cast<ObservablePlugin>(plugin);
        if (observable != nullptr) {
            mLogger->info("add observer PLUGINNAME = %\n", plugin->getName());
            observable->addObserver(mPluginObserver);
        } else {
            mLogger->info("PLUGINNAME = % not observable\n", plugin->getName());
        }
    }
}

/* SPECIFIC READER OBSERVER --------------------------------------------------------------------- */

SpecificReaderObserver::SpecificReaderObserver() {}

void ObservableReaderNotificationEngine::SpecificReaderObserver::update(
    const std::shared_ptr<ReaderEvent> event)
{
    switch (event->getEventType().innerEnumValue) {
    case ReaderEvent::EventType::InnerEnum::SE_MATCHED:
        /*
         * Informs the underliying later of the end of the SE processing, in order to manage the
         * removal sequence.
         */
        try {
            std::shared_ptr<SeReader> reader = event->getReader();
            auto observable = std::dynamic_pointer_cast<ObservableReader>(reader);
            observable->finalizeSeProcessing();
        } catch (KeypleReaderNotFoundException& e) {
            mLogger->debug("update - KeypleReaderNotFoundException: %\n", e);
        } catch (KeyplePluginNotFoundException& e) {
            mLogger->debug("update - KeyplePluginNotFoundException: %\n", e);
        }
        break;

    case ReaderEvent::EventType::InnerEnum::SE_INSERTED:
        /* End of the SE processing is automatically done */
        break;
    default:
        mLogger->debug("unhandled case %\n", event->getEventType());
    }

    /* Just log the event */
    mLogger->info("event %\n", event);
}

/* SPECIFIC PLUGIN OBSERVER --------------------------------------------------------------------- */

SpecificPluginObserver::SpecificPluginObserver(
  std::shared_ptr<SpecificReaderObserver> readerObserver)
: mReaderObserver(readerObserver) {}

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
        mLogger->info("event: %\n", event);

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

        if (event->getEventType() == PluginEvent::EventType::READER_CONNECTED) {
            mLogger->info("new reader! READERNAME = %\n", reader->getName());

            /*
             * We are informed here of a disconnection of a reader.
             *
             * We add an observer to this reader if this is possible.
             */
            auto observable = std::dynamic_pointer_cast<ObservableReader>(reader);
            if (observable) {
                if (mReaderObserver) {
                    mLogger->info("add observer READERNAME = %\n", reader->getName());
                    observable->addObserver(mReaderObserver);
                } else {
                    mLogger->info("no observer to add READERNAME = %\n", reader->getName());
                }
            }
        } else if (event->getEventType() == PluginEvent::EventType::READER_DISCONNECTED) {
            /*
             * We are informed here of a disconnection of a reader.
             *
             * The reader object still exists but will be removed from the reader list right after.
             * Thus, we can properly remove the observer attached to this reader before the list
             * update.
             */
            mLogger->info("reader removed. READERNAME = %\n", readerName);

            if (std::dynamic_pointer_cast<ObservableReader>(reader) !=
                nullptr) {
                if (mReaderObserver != nullptr) {
                    mLogger->info("remove observer READERNAME = %\n", readerName);
                    (std::dynamic_pointer_cast<ObservableReader>(reader))
                        ->removeObserver(mReaderObserver);
                } else {
                    mLogger->info("unplugged reader READERNAME = % wasn't observed\n", readerName);
                }
            }

        } else {
            mLogger->info("unexpected reader event: %\n", event->getEventType());
        }
    }
}

}
}
}
}
