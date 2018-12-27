#pragma once

#include <set>
#include <unordered_set>
#include <memory>

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
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace util {

                template<typename T>
                class Observer {
                public:
                    virtual void update(T event_Renamed) = 0;
                };

                /**
                 * Generic Observable class
                 *
                 * @param <T> Generic event
                 *
                 */

                template<typename T>
                class Observable {
                public:


                private:
                    bool changed = false;

                    /*
                     * this object will be used to synchronize the access to the observers list in order to be
                     * thread safe
                     */
                    const std::shared_ptr<void> SYNC = nullptr;

                    std::shared_ptr<std::set<Observer<T>>> observers;

                public:
                    virtual void addObserver(std::shared_ptr<Observer<T>> observer) {
                        if (observer == nullptr) {
                            return;
                        }

//JAVA TO C++ CONVERTER TODO TASK: Multithread locking is not converted to native C++ unless you choose one of the options on the 'Modern C++ Options' dialog:
//                        synchronized(SYNC) {
//                            if (observers == nullptr) {
//                                observers = std::unordered_set<Observer<T>>(1);
//                            }
//                            observers->add(observer);
//                        }
                    }

                    virtual void removeObserver(std::shared_ptr<Observer<T>> observer) {
                        if (observer == nullptr) {
                            return;
                        }

//JAVA TO C++ CONVERTER TODO TASK: Multithread locking is not converted to native C++ unless you choose one of the options on the 'Modern C++ Options' dialog:
//                        synchronized(SYNC) {
//                            if (observers != nullptr) {
//                                observers->remove(observer);
//                            }
//                        }
                    }

                    virtual void clearObservers() {
                        if (observers != nullptr) {
                            this->observers->clear();
                        }
                    }

                    virtual void setChanged() {
                        this->changed = true;
                    }

                    virtual void clearChanged() {
                        this->changed = false;
                    }

                    virtual bool hasChanged() {
                        return this->changed;
                    }

                    virtual int countObservers() {
                        return observers == nullptr ? 0 : observers->size();
                    }

                    virtual void notifyObservers() {
                        notifyObservers(nullptr);
                    }

                    virtual void notifyObservers(T const event_Renamed) {
                        std::shared_ptr<std::set<std::shared_ptr<Observer<T>>>> observersCopy;

//JAVA TO C++ CONVERTER TODO TASK: Multithread locking is not converted to native C++ unless you choose one of the options on the 'Modern C++ Options' dialog:
//                        synchronized(SYNC) {
//                            if (observers == nullptr) {
//                                return;
//                            }
//                            observersCopy = std::unordered_set<std::shared_ptr<Observer<T>>>(observers);
//                        }

                        for (auto observer : observersCopy) {
                            observer->update(event_Renamed);
                        }
                    }
                };

            }
        }
    }
}
