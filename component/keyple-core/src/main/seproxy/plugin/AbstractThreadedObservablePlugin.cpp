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
#include "AbstractThreadedObservablePlugin.h"
#include "AbstractObservableLocalReader.h"
#include "PluginEvent.h"
#include "KeypleReaderException.h"
#include "InterruptedException.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::plugin::local;

std::set<std::string> _set;
std::shared_ptr<std::set<std::string>> nativeReadersNames =
    std::make_shared<std::set<std::string>>(_set);

AbstractThreadedObservablePlugin::AbstractThreadedObservablePlugin(
    const std::string& name)
: AbstractObservablePlugin(name)
{
    logger->trace("constructor (name: %)\n", name);
}

void AbstractThreadedObservablePlugin::finalize()
{
    thread->end();
    logger->trace("[%] observable Plugin thread ended\n", getName());

    //AbstractPlugin::finalize();
}

void AbstractThreadedObservablePlugin::addObserver(
    const std::shared_ptr<ObservablePlugin::PluginObserver> observer)
{
    AbstractObservablePlugin::addObserver(observer);

    if (countObservers() == 1) {
        logger->trace("Start monitoring the plugin %\n", getName());
        thread = std::make_shared<EventThread>(this, getName());
        thread->start();
        /* Give some time to the thread */
        Thread::sleep(100);

    }
}

void AbstractThreadedObservablePlugin::removeObserver(
    const std::shared_ptr<ObservablePlugin::PluginObserver> observer)
{
    AbstractObservablePlugin::removeObserver(observer);

    if (countObservers() == 0) {
        logger->trace("Stop the plugin monitoring\n");
        if (thread != nullptr) {
            thread->end();
        }
    }
}

void AbstractThreadedObservablePlugin::clearObservers()
{
    AbstractObservablePlugin::clearObservers();

    if (thread != nullptr) {
        logger->trace("Stop the plugin monitoring\n");
        thread->end();
    }
}

bool AbstractThreadedObservablePlugin::isMonitoring()
{
    return thread != nullptr && thread->isAlive() && thread->isMonitoring();
}

AbstractThreadedObservablePlugin::EventThread::EventThread(
    AbstractThreadedObservablePlugin* outerInstance,
    const std::string& pluginName)
: outerInstance(outerInstance), pluginName(pluginName)
{
}

void AbstractThreadedObservablePlugin::EventThread::end()
{
    if (outerInstance && outerInstance->logger)
        outerInstance->logger->trace("stopping event thread\n");

    running = false;
    this->interrupt();
}

void* AbstractThreadedObservablePlugin::EventThread::run()
{
    outerInstance->logger->trace("starting event thread\n");

    std::shared_ptr<std::set<std::string>> changedReaderNames =
        std::make_shared<std::set<std::string>>();

    try {
        while ((!isInterrupted()) && running) {
            /* Retrieves the current readers names list */
            const std::set<std::string>& actualNativeReadersNames =
                outerInstance->fetchNativeReadersNames();

            /*
             * Checks if it has changed this algorithm favors cases where
             * nothing changes.
             */
            if (outerInstance->nativeReadersNames != actualNativeReadersNames) {
                /*
                 * Parse the current readers list, notify for disappeared
                 * readers, update readers list
                 */

                /* build changed reader names list */
                changedReaderNames->clear();

                for (auto it : outerInstance->readers) {
                    if (actualNativeReadersNames.find(it->getName()) ==
                        actualNativeReadersNames.end()) {
                        changedReaderNames->insert(it->getName());
                    }
                }

                /* notify disconnections if any and update the reader list */
                if (changedReaderNames->size() > 0) {
                    /* grouped notification */
                    outerInstance->notifyObservers(
                        std::make_shared<PluginEvent>(
                            this->pluginName, changedReaderNames,
                            PluginEvent::EventType::READER_DISCONNECTED));

                    /* list update */
                    for (auto it = outerInstance->readers.begin();
                              it != outerInstance->readers.end(); ) {

                        if (actualNativeReadersNames.find((*it)->getName()) ==
                            actualNativeReadersNames.end()) {
                            /*
                             * Removes any possible observers before removing
                             * the reader.
                             */
                            std::shared_ptr<AbstractObservableLocalReader> observableR =
                                std::dynamic_pointer_cast<AbstractObservableLocalReader>(*it);

                            if (observableR) {
                                observableR->clearObservers();

                                /* In case where Reader was detecting SE */
                                observableR->stopSeDetection();
                            }

                            outerInstance->logger->trace(
                                "[%][%] Plugin thread => Remove unplugged "
                                "reader from readers list\n", pluginName,
                                (*it)->getName());

                            /* remove reader name from the current list */
                            outerInstance->nativeReadersNames.erase(
                                (*it)->getName());

                            it = outerInstance->readers.erase(it);
                        } else {
                            it++;
                        }
                    }

                    /* clean the list for a possible connection notification */
                    changedReaderNames->clear();
                }

                /*
                 * Parse the new readers list, notify for readers appearance,
                 * update readers list
                 */
                for (auto readerName : actualNativeReadersNames) {
                    if (outerInstance->nativeReadersNames.find(readerName) !=
                        outerInstance->nativeReadersNames.end()) {
                        std::shared_ptr<SeReader> reader =
                            outerInstance->fetchNativeReader(readerName);
                        outerInstance->readers.insert(reader);

                        /* add to the notification list */
                        changedReaderNames->insert(readerName);
                        outerInstance->logger->trace(
                            "[%][%] Plugin thread => Add plugged reader to "
                            "readers list\n", pluginName, reader->getName());

                        /* add reader name to the current list */
                        outerInstance->nativeReadersNames.insert(readerName);
                    }
                }

                /* notify connections if any */
                if (changedReaderNames->size() > 0) {
                    outerInstance->notifyObservers(
                        std::make_shared<PluginEvent>(
                            this->pluginName, changedReaderNames,
                            PluginEvent::EventType::READER_CONNECTED));
                }
            }

            /* sleep for a while. */
            Thread::sleep((long)outerInstance->threadWaitTimeout);
        }

    } catch (const InterruptedException& e) {
        outerInstance->logger->warn("[%] An exception occurred while "
                                    "monitoring plugin: %\n", pluginName, e);

        /* Restore interrupted state */
        //Thread::currentThread().interrupt();
    } catch (const KeypleReaderException& e) {
        outerInstance->logger->warn("[%] An exception occurred while "
                                    "monitoring plugin: %\n", pluginName, e);
    }

    return nullptr;
}

bool AbstractThreadedObservablePlugin::EventThread::isAlive()
{
    return alive;
}

bool AbstractThreadedObservablePlugin::EventThread::isMonitoring()
{
    return running;
}

}
}
}
}
