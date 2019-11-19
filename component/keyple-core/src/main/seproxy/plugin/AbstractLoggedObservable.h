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

#pragma once

/* Core */
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <memory>
#include <type_traits>

/* Common */
#include "Logger.h"
#include "LoggerFactory.h"

/* Core */
#include "Observable.h"
#include "Configurable.h"
#include "Nameable.h"
#include "Observable.h"
#include "ReaderEvent_Import.h"
#include "PluginEvent.h"
#include "KeypleBaseException.h"

namespace keyple { namespace core { namespace seproxy { namespace plugin {
    class AbstractObservableReader; } } } }
namespace keyple { namespace core { namespace seproxy { namespace plugin {
    class AbstractObservablePlugin; } } } }

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

using namespace keyple::core::seproxy::exception;
using namespace keyple::core::util;
using namespace keyple::common;
using namespace keyple::core::seproxy::event;

/**
 * Intermediate observable class to handle the logging of
 * AbstractObservableReader and AbstractObservablePlugin
 */
template <typename T>
class AbstractLoggedObservable
: public Observable<T>, public virtual Nameable, public Configurable {
public:
    /**
     * Item constructor Force the definition of a name through the use of super
     * method.
     *
     * @param name name of the observed object
     */
    AbstractLoggedObservable(const std::string &name) : name(name)
    {
        const char *s = name.c_str();
        std::cout << s << std::endl;
        logger->debug("constructor (name: %s)\n", name.c_str());
    }

    /**
     * Gets the reader name
     *
     * @return the reader name string
     */
    std::string getName() override
    {
        return name;
    }

    /**
     * Add an observer. This will allow to be notified about all readers or
     * plugins events.
     *
     * @param observer Observer to notify
     */
    virtual void addObserver(std::shared_ptr<Observer<T>> observer) override
    {
        logger->trace("[%s][%s] addObserver => Adding an observer\n",
                      name.c_str(), this->getName().c_str());

        Observable<T>::addObserver(observer);
    }

    /**
     * Remove an observer.
     *
     * @param observer Observer to stop notifying
     */
    virtual void removeObserver(std::shared_ptr<Observer<T>> observer) override
    {
        /*
         * Alex: Downcasting on both AbstractObservableReader and
         *       AbstractObservablePlugin doesn't seem possible even though
         *       templates are used because somehow the compiler knows where
         *       that comes from and doesn't want to apply the cast.
         *       Using the type of the template to switch between one or the
         *       other.
         */
        if (std::is_same<T, ReaderEvent>::value) {
        //if (std::dynamic_pointer_cast<AbstractObservableReader>(shared_from_this()) != nullptr) {
            logger->trace("[%s] removeObserver => Deleting a reader observer\n",
                          this->getName().c_str());
        }
        else { //else if (std::reinterpret_pointer_cast<AbstractObservablePlugin>(shared_from_this()) != nullptr) {
            logger->trace("[%s] removeObserver => Deleting a plugin observer\n",
                          this->getName().c_str());
        }

        Observable<T>::removeObserver(observer);
    }

    /**
     * This method shall be called only from a SE Proxy plugin or reader
     * implementing AbstractObservableReader or AbstractObservablePlugin. Push a
     * ReaderEvent / PluginEvent of the selected AbstractObservableReader /
     * AbstractObservablePlugin to its registered Observer.
     *
     * @param event the event
     */
    // Alex: function was final in Java (problem in PcscPlugin.cpp)
    void notifyObservers(std::shared_ptr<T> event) override
    {
        /*
         * Alex: Downcasting on both AbstractObservableReader and
         *       AbstractObservablePlugin doesn't seem possible even though
         *       templates are used because somehow the compiler knows where
         *       that comes from and doesn't want to apply the cast.
         *       Using the type of the template to switch between one or the
         *       other.
         */
        if (std::is_same<T, ReaderEvent>::value) {
        //if (std::static_pointer_cast<AbstractObservableReader>(shared_from_this()) != nullptr) {
            logger->trace("[%s] AbstractObservableReader => Notifying a " \
                          "reader event. EVENTNAME = %s\n",
                          this->getName().c_str(),
                          (reinterpret_cast<ReaderEvent *>(event.get()))
                              ->getEventType().getName().c_str());
                                              
        }
        else { //else if (std::dynamic_pointer_cast<AbstractObservablePlugin>(shared_from_this()) != nullptr) {
            logger->trace("[%s] AbstractObservableReader => Notifying a " \
                          "plugin event. EVENTNAME = %s\n",
                          this->getName().c_str(),
                          (reinterpret_cast<PluginEvent *>(event.get()))
                               ->getEventType().getName().c_str());
        }
 
        Observable<T>::setChanged();
 
        Observable<T>::notifyObservers(event);
    }

    /**
     * Set a list of parameters on a reader.
     * <p>
     * See {@link #setParameter(String, String)} for more details
     *
     * @param parameters the new parameters
     * @throws KeypleBaseException This method can fail when disabling the
     *         exclusive mode as it's executed instantly
     */
    void setParameters(std::unordered_map<std::string, std::string> &parameters)
    override
    {
        for (auto en : parameters) {
            setParameter(en.first, en.second);
        }
    }

protected:
    /**
     *
     */
    std::shared_ptr<AbstractLoggedObservable> shared_from_this()
    {
        return std::static_pointer_cast<AbstractLoggedObservable>(
                   Observable<T>::shared_from_this());
    }

    /**
     * The item name (must be unique)
     */
    const std::string name;

private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(AbstractLoggedObservable));

};

}
}
}
}
