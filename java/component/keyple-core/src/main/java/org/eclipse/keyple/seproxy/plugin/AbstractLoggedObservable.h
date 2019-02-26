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

#include "../../util/Configurable.h"
#include "../../util/Nameable.h"
#include "../../util/Observable.h"
#include "../event/ReaderEvent.h"
#include "../event/PluginEvent.h"
#include "../exception/KeypleBaseException.h"
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <memory>

/* Common */
#include "Logger.h"
#include "LoggerFactory.h"

/* Core */
#include "Observable.h"

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
                        AbstractLoggedObservable(const std::string &name) : name(name)
                        {
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
                         * @param observer Observer to notify
                         */
                        virtual void addObserver(
                            std::shared_ptr<org::eclipse::keyple::util::Observer<T>> observer) override
                        {

                            logger->trace("[AbstractLoggedObservable::addObserver] observer: %p\n",
                                          observer.get());

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
                            /*
 * Alex: commented out, shared_from_this is not handled properly yet
                            if (std::dynamic_pointer_cast<AbstractObservableReader>(shared_from_this()) != nullptr) {
                                logger->trace("[{}] removeObserver => Deleting a reader observer", this->getName());
                            }
                            else if (std::dynamic_pointer_cast<AbstractObservablePlugin>(shared_from_this()) != nullptr) {
                                logger->trace("[{}] removeObserver => Deleting a plugin observer", this->getName());
                            }
 */
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
                            /*
 * Alex: commented out, shared_from_this is not handled properly yet
                            if (std::dynamic_pointer_cast<AbstractObservableReader>(shared_from_this()) != nullptr) {
                                logger->trace("[{}] AbstractObservableReader => Notifying a reader event. EVENTNAME = {}", this->getName(), (std::static_pointer_cast<ReaderEvent>(event_Renamed))->getEventType().getName());
                            }
                            else if (std::dynamic_pointer_cast<AbstractObservablePlugin>(shared_from_this()) != nullptr) {
                                logger->trace("[{}] AbstractObservableReader => Notifying a plugin event. EVENTNAME = {} ", this->getName(), (std::static_pointer_cast<PluginEvent>(event_Renamed))->getEventType().getName());
                            }
 */
                            /*
 * Alex: where does that function come from?
                            setChanged();
 */
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
                        void setParameters(std::unordered_map<std::string, std::string> &parameters) override
                        {
                            for (auto en : parameters)
                            {
                                setParameter(en.first, en.second);
                            }
                        }

                      protected:
                        std::shared_ptr<AbstractLoggedObservable> shared_from_this()
                        {
                            return std::static_pointer_cast<AbstractLoggedObservable>(
                                util::Observable<T>::shared_from_this());
                        }
                    };
                } // namespace plugin
            }     // namespace seproxy
        }         // namespace keyple
    }             // namespace eclipse
} // namespace org
