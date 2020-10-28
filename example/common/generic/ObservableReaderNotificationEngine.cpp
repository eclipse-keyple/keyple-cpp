/**************************************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                                                *
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

SpecificReaderObserver::SpecificReaderObserver(ObservableReaderNotificationEngine* outerInstance)
: mOuterInstance(outerInstance) {}

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
            const std::shared_ptr<SeReader> reader =
                SeProxyService::getInstance().getPlugin(event->getPluginName())
                                            ->getReader(event->getReaderName());
            auto observableReader = std::dynamic_pointer_cast<ObservableReader>(reader);
            observableReader->notifySeProcessed();
        } catch (KeypleReaderNotFoundException& e) {
            mOuterInstance->mLogger->debug("update - KeypleReaderNotFoundException: %\n", e);
        } catch (KeyplePluginNotFoundException& e) {
            mOuterInstance->mLogger->debug("update - KeyplePluginNotFoundException: %\n", e);
        }
        break;

    case ReaderEvent::EventType::InnerEnum::SE_INSERTED:
        /* End of the SE processing is automatically done */
        break;
    default:
        mOuterInstance->mLogger->debug("unhandled case %\n", event->getEventType());
    }

    /* Just log the event */
    mOuterInstance->mLogger->info("event %\n", event);
}

/* SPECIFIC PLUGIN OBSERVER --------------------------------------------------------------------- */

SpecificPluginObserver::SpecificPluginObserver(
    ObservableReaderNotificationEngine* outerInstance,
    std::shared_ptr<SpecificReaderObserver> readerObserver)
: mOuterInstance(outerInstance), mReaderObserver(readerObserver) {}

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
        mOuterInstance->mLogger->info("event: %\n", event);

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
            mOuterInstance->mLogger->info("new reader! READERNAME = %\n", reader->getName());

            /*
             * We are informed here of a disconnection of a reader.
             *
             * We add an observer to this reader if this is possible.
             */
            auto observable = std::dynamic_pointer_cast<ObservableReader>(reader);
            if (observable) {
                if (mReaderObserver) {
                    mOuterInstance->mLogger->info("add observer READERNAME = %\n",
                                                  reader->getName());
                    observable->addObserver(mReaderObserver);
                } else {
                    mOuterInstance->mLogger->info("no observer to add READERNAME = %\n",
                                                  reader->getName());
                }
            }
        } else if (event->getEventType() == PluginEvent::EventType::READER_DISCONNECTED) {
            /*
             * We are informed here of a disconnection of a reader.
             *
             * The reader object still exists but will be removed from the
             * reader list right after. Thus, we can properly remove the
             * observer attached to this reader before the list update.
             */
            mOuterInstance->mLogger->info("reader removed. READERNAME = %\n",
                                        readerName);

            if (std::dynamic_pointer_cast<ObservableReader>(reader) !=
                nullptr) {
                if (mReaderObserver != nullptr) {
                    mOuterInstance->mLogger->info("remove observer READERNAME "
                                                "= %\n", readerName);
                    (std::dynamic_pointer_cast<ObservableReader>(reader))
                        ->removeObserver(mReaderObserver);
                } else {
                    mOuterInstance->mLogger->info("unplugged reader READERNAME = % wasn't " \
                                                  "observed\n",
                                                  readerName);
                }
            }

        } else {
            mOuterInstance->mLogger->info("unexpected reader event: %\n", event->getEventType());
        }
    }
}

}
}
}
}
