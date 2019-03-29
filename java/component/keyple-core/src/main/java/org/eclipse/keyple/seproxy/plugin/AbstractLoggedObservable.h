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

#pragma once

/* Core */
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <memory>

/* Common */
#include "Logger.h"
#include "LoggerFactory.h"

/* Core */
#include "Observable.h"
#include "Configurable.h"
#include "Nameable.h"
#include "Observable.h"
#include "ReaderEvent.h"
#include "PluginEvent.h"
#include "KeypleBaseException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {
                    class AbstractObservableReader;
                }
            } // namespace seproxy
        }     // namespace keyple
    }         // namespace eclipse
} // namespace org

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {

                    using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                    using Configurable        = org::eclipse::keyple::util::Configurable;
                    using Nameable            = org::eclipse::keyple::util::Nameable;
                    using Logger              = org::eclipse::keyple::common::Logger;
                    using LoggerFactory = org::eclipse::keyple::common::LoggerFactory;

                    /**
                     * Intermediate observable class to handle the logging of AbstractObservableReader and
                     * AbstractObservablePlugin
                     *
                     */
                    template <typename T>
                    class AbstractLoggedObservable : public org::eclipse::keyple::util::Observable<T>,
                                                     public virtual Nameable,
                                                     public Configurable {
                      private:
                        /**
                         *
                         */
                        const std::shared_ptr<Logger> logger =
                            LoggerFactory::getLogger(typeid(AbstractLoggedObservable));

                        /**
                         * The item name (must be unique)
                         */
                      protected:
                        const std::string name;

                        /**
                         * Item constructor Force the definition of a name through the use of super method.
                         *
                         * @param name name of the observed object
                         */
                      public:
                        AbstractLoggedObservable(const std::string &name)
                            : name(name)
                        {
                            logger->debug("constructor (name: %s)\n", name);
                        }

                        ~AbstractLoggedObservable()
                        {
                            logger->debug("destructor (name: %s)\n", name);
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
                         * Add an observer. This will allow to be notified about all readers or plugins events.
                         *
                         * @param observer Observer to notiy
                         */
                        virtual void addObserver(
                            std::shared_ptr<org::eclipse::keyple::util::Observer<T>> observer) override
                        {

                            logger->trace("[%s][%s] addObserver => Adding an observer.", this->getClass(), this->getName());

                            org::eclipse::keyple::util::Observable<T>::addObserver(observer);
                        }

                        /**
                         * Remove an observer.
                         *
                         * @param observer Observer to stop notifying
                         */
                        virtual void removeObserver(
                            std::shared_ptr<org::eclipse::keyple::util::Observer<T>> observer) override
                        {
                            if (std::dynamic_pointer_cast<AbstractObservableReader>(shared_from_this()) != nullptr) {
                                logger->trace("[{}] removeObserver => Deleting a reader observer", this->getName());
                            }
                            else if (std::dynamic_pointer_cast<AbstractObservablePlugin>(shared_from_this()) != nullptr) {
                                logger->trace("[{}] removeObserver => Deleting a plugin observer", this->getName());
                            }

                            org::eclipse::keyple::util::Observable<T>::removeObserver(observer);
                        }

                        /**
                         * This method shall be called only from a SE Proxy plugin or reader implementing
                         * AbstractObservableReader or AbstractObservablePlugin. Push a ReaderEvent / PluginEvent of the
                         * selected AbstractObservableReader / AbstractObservablePlugin to its registered Observer.
                         *
                         * @param event the event
                         */
                        // Alex: function was final in Java (problem in PcscPlugin.cpp)
                        void notifyObservers(std::shared_ptr<T> event) override
                        {
                            if (std::dynamic_pointer_cast<AbstractObservableReader>(shared_from_this()) != nullptr) {
                                logger->trace("[{}] AbstractObservableReader => Notifying a reader event. EVENTNAME = {}", this->getName(), (std::static_pointer_cast<ReaderEvent>(event_Renamed))->getEventType().getName());
                            }
                            else if (std::dynamic_pointer_cast<AbstractObservablePlugin>(shared_from_this()) != nullptr) {
                                logger->trace("[{}] AbstractObservableReader => Notifying a plugin event. EVENTNAME = {} ", this->getName(), (std::static_pointer_cast<PluginEvent>(event_Renamed))->getEventType().getName());
                            }
 
                            setChanged();
 
                            org::eclipse::keyple::util::Observable<T>::notifyObservers(event);
                        }

                        /**
                         * Set a list of parameters on a reader.
                         * <p>
                         * See {@link #setParameter(String, String)} for more details
                         *
                         * @param parameters the new parameters
                         * @throws KeypleBaseException This method can fail when disabling the exclusive mode as it's
                         *         executed instantly
                         */
                        void setParameters(std::unordered_map<std::string, std::string> &parameters) throw(std::invalid_argument, KeypleBaseException) override {
                            for (auto en : parameters) {
                                setParameter(en.first, en.second);
                            }
                        }

                      protected:
                        std::shared_ptr<AbstractLoggedObservable> shared_from_this() {
                            return std::static_pointer_cast<AbstractLoggedObservable>(org.eclipse.keyple.util.Observable<T>::shared_from_this());
                        }
                    };
                } // namespace plugin
            }     // namespace seproxy
        }         // namespace keyple
    }             // namespace eclipse
} // namespace org
