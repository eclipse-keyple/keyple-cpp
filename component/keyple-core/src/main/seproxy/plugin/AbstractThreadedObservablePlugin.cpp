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
#include "KeypleReaderIOException.h"
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
    mLogger->trace("constructor (name: %)\n", name);
}

void AbstractThreadedObservablePlugin::finalize()
{
    mThread->end();
    mLogger->trace("[%] observable Plugin thread ended\n", getName());

    //AbstractPlugin::finalize();
}

void AbstractThreadedObservablePlugin::addObserver(
    const std::shared_ptr<ObservablePlugin::PluginObserver> observer)
{
    AbstractObservablePlugin::addObserver(observer);

    if (countObservers() == 1) {
        mLogger->trace("Start monitoring the plugin %\n", getName());
        mThread = std::make_shared<EventThread>(this, getName());
        mThread->start();
        /* Give some time to the thread */
        Thread::sleep(100);

    }
}

void AbstractThreadedObservablePlugin::removeObserver(
    const std::shared_ptr<ObservablePlugin::PluginObserver> observer)
{
    AbstractObservablePlugin::removeObserver(observer);

    if (countObservers() == 0) {
        mLogger->trace("Stop the plugin monitoring\n");
        if (mThread != nullptr) {
            mThread->end();
        }
    }
}

void AbstractThreadedObservablePlugin::clearObservers()
{
    AbstractObservablePlugin::clearObservers();

    if (mThread != nullptr) {
        mLogger->trace("Stop the plugin monitoring\n");
        mThread->end();
    }
}

bool AbstractThreadedObservablePlugin::isMonitoring()
{
    return mThread != nullptr && mThread->isAlive() && mThread->isMonitoring();
}

AbstractThreadedObservablePlugin::EventThread::EventThread(
    AbstractThreadedObservablePlugin* outerInstance,
    const std::string& pluginName)
: mOuterInstance(outerInstance), mPluginName(pluginName)
{
}

void AbstractThreadedObservablePlugin::EventThread::end()
{
    if (mOuterInstance && mOuterInstance->mLogger)
        mOuterInstance->mLogger->trace("stopping event thread\n");

    mRunning = false;
    this->interrupt();
}

void* AbstractThreadedObservablePlugin::EventThread::run()
{
    mOuterInstance->mLogger->trace("starting event thread\n");

    std::shared_ptr<std::set<std::string>> changedReaderNames =
        std::make_shared<std::set<std::string>>();

    try {
        while ((!isInterrupted()) && mRunning) {
            /* Retrieves the current readers names list */
            const std::set<std::string>& actualNativeReadersNames =
                mOuterInstance->fetchNativeReadersNames();

            /*
             * Checks if it has changed this algorithm favors cases where
             * nothing changes.
             */
            if (mOuterInstance->mNativeReadersNames != actualNativeReadersNames)
            {
                /*
                 * Parse the current readers list, notify for disappeared
                 * readers, update readers list
                 */

                /* build changed reader names list */
                changedReaderNames->clear();

                for (const auto& pair : mOuterInstance->mReaders) {
                    if (actualNativeReadersNames.find(pair.second->getName())
                        == actualNativeReadersNames.end()) {
                        changedReaderNames->insert(pair.second->getName());
                    }
                }

                /* notify disconnections if any and update the reader list */
                if (changedReaderNames->size() > 0) {
                    /* grouped notification */
                    mOuterInstance->notifyObservers(
                        std::make_shared<PluginEvent>(
                            mPluginName, changedReaderNames,
                            PluginEvent::EventType::READER_DISCONNECTED));

                    /* list update */
                    for (auto it = mOuterInstance->mReaders.begin();
                              it != mOuterInstance->mReaders.end(); ) {

                        if (actualNativeReadersNames.find(
                                (*it).second->getName()) ==
                            actualNativeReadersNames.end()) {
                            /*
                             * Removes any possible observers before removing
                             * the reader.
                             */
                            std::shared_ptr<ObservableReader>
                                observableR = std::dynamic_pointer_cast<
                                        ObservableReader>((*it).second);

                            if (observableR) {
                                observableR->clearObservers();

                                /* In case where Reader was detecting SE */
                                observableR->stopSeDetection();
                            }

                            mOuterInstance->mLogger->trace(
                                "[%][%] Plugin thread => Remove unplugged "
                                "reader from readers list\n", mPluginName,
                                (*it).second->getName());

                            /* remove reader name from the current list */
                            mOuterInstance->mNativeReadersNames.erase(
                                (*it).second->getName());

                            it = mOuterInstance->mReaders.erase(it);
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
                    if (mOuterInstance->mNativeReadersNames.find(readerName) !=
                        mOuterInstance->mNativeReadersNames.end()) {
                        std::shared_ptr<SeReader> reader =
                            mOuterInstance->fetchNativeReader(readerName);
                        mOuterInstance->mReaders.insert({reader->getName(),
                                                         reader});

                        /* add to the notification list */
                        changedReaderNames->insert(readerName);
                        mOuterInstance->mLogger->trace(
                            "[%][%] Plugin thread => Add plugged reader to "
                            "readers list\n", mPluginName, reader->getName());

                        /* add reader name to the current list */
                        mOuterInstance->mNativeReadersNames.insert(readerName);
                    }
                }

                /* notify connections if any */
                if (changedReaderNames->size() > 0) {
                    mOuterInstance->notifyObservers(
                        std::make_shared<PluginEvent>(
                            mPluginName, changedReaderNames,
                            PluginEvent::EventType::READER_CONNECTED));
                }
            }

            /* sleep for a while. */
            Thread::sleep((long)mOuterInstance->mThreadWaitTimeout);
        }

    } catch (const InterruptedException& e) {
        mOuterInstance->mLogger->warn("[%] An exception occurred while "
                                     "monitoring plugin: %\n", mPluginName, e);

        /* Restore interrupted state */
        //Thread::currentThread().interrupt();
    } catch (const KeypleReaderException& e) {
        mOuterInstance->mLogger->warn("[%] An exception occurred while "
                                     "monitoring plugin: %\n", mPluginName, e);
    }

    return nullptr;
}

bool AbstractThreadedObservablePlugin::EventThread::isAlive()
{
    return mAlive;
}

bool AbstractThreadedObservablePlugin::EventThread::isMonitoring()
{
    return mRunning;
}

}
}
}
}
