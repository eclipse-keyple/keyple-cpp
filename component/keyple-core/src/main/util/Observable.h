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

#include <iostream>
#include <ostream>
#include <memory>
#include <set>
#include <unordered_set>

/* Core */
#include "ReaderEvent.h"

/* Common */
#include "LoggerFactory.h"

namespace keyple {
namespace core {
namespace util {

using namespace keyple::common;

template <typename T> class Observer {
public:
    /**
     *
     */
    virtual ~Observer()
    {
    }

    /**
     *
     */
    virtual void update(std::shared_ptr<T> event)
    {
        (void)event;
    }
};

/**
 * Generic Observable class
 *
 * @param <T> Generic event
 *
 */
template <typename T>
class Observable : public std::enable_shared_from_this<Observable<T>> {
public:
    /**
     *
     */
    Observable() : changed(false)
    {
    }

    /**
     *
     */
    Observable(const Observable& o)
    : std::enable_shared_from_this<Observable<T>>(), changed(o.changed),
      observers(o.observers)
    {
    }

    /**
     *
     */
    virtual ~Observable()
    {
    }

    /**
     *
     */
    virtual void addObserver(std::shared_ptr<Observer<T>> observer)
    {
        logger->debug("addObserver\n");

        if (observer == nullptr) {
            logger->error("addObserver - observer is null, skipping it\n");
            return;
        }

        /* Multithread locking is not converted to native C++ */

        if (!observers.size())
            logger->debug("addObserver - observers empty, creating new set\n");

        logger->debug("addObserver - adding observer to set\n");

        observers.insert(observer);
    }

    /**
     *
     */
    virtual void removeObserver(std::shared_ptr<Observer<T>> observer)
    {
        logger->debug("removeObserver\n");

        if (observer == nullptr) {
            logger->debug("removeObserver -observer is null, skipping it\n");
            return;
        }

        /* Multithread locking is not converted to native C++ */
        if (observers.size()) {
            logger->debug("removeObserver - removing observer from set\n");
            observers.erase(observer);
        }
    }

    /**
     *
     */
    virtual void clearObservers()
    {
        logger->debug("clearObservers\n");

        if (observers.size()) {
            logger->debug("clearObservers - clearing observers set\n");
            this->observers.clear();
        }
    }

    /**
     *
     */
    virtual void setChanged()
    {
        this->changed = true;
    }

    /**
     *
     */
    virtual void clearChanged()
    {
        this->changed = false;
    }

    virtual bool hasChanged()
    {
        return this->changed;
    }

    /**
     *
     */
    virtual int countObservers()
    {
        logger->debug("countObservers\n");

        return observers.size();
    }

    /**
     *
     */
    virtual void notifyObservers()
    {
        logger->debug("notifyObservers - notify observers (no event)\n");

        notifyObservers(nullptr);
    }

    /**
     *
     */
    virtual void notifyObservers(std::shared_ptr<T> event)
    {
        logger->debug("notifyObservers\n");

        /* Multithread locking is not converted to native C++ */

        if (!observers.size())
            return;

        /* Alex: Not sure I need the copy */
        std::set<std::shared_ptr<Observer<T>>> observersCopy;
        observersCopy.insert(observers.begin(), observers.end());

        for (auto observer : observersCopy)
            observer->update(event);
    }

private:
    /**
     *
     */
    bool changed = false;

    /*
     * this object will be used to synchronize the access to the observers list
     * in order to be thread safe
     */
    const std::shared_ptr<void> SYNC = nullptr;

    /**
     *
     */
    std::set<std::shared_ptr<Observer<T>>> observers;

    /**
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(Observable));
};

}
}
}
