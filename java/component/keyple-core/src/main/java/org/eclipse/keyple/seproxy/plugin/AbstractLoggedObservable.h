#pragma once

#include "../../util/Configurable.h"
#include "../../util/Nameable.h"
#include "../../util/Observable.h"
#include "AbstractObservableReader.h"
#include "AbstractObservablePlugin.h"
#include "../event/ReaderEvent.h"
#include "../event/PluginEvent.h"
#include "../exception/KeypleBaseException.h"
#include <string>
#include <unordered_map>
#include <stdexcept>
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
            namespace seproxy {
                namespace plugin {


                    using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                    using Configurable = org::eclipse::keyple::util::Configurable;
                    using Nameable = org::eclipse::keyple::util::Nameable;
                    using Observable = org::eclipse::keyple::util::Observable;
                    using org::slf4j::Logger;
                    using org::slf4j::LoggerFactory;

                    /**
                     * Intermediate observable class to handle the logging of AbstractObservableReader and
                     * AbstractObservablePlugin
                     * 
                     */
                    template<typename T>
                    class AbstractLoggedObservable : public Observable<T>, public Nameable, public Configurable {
                    private:
//JAVA TO C++ CONVERTER TODO TASK: Native C++ does not allow initialization of static non-const/integral fields in their declarations - choose the conversion option for separate .h and .cpp files:
                        static const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(AbstractLoggedObservable::typeid);

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
                        AbstractLoggedObservable(const std::string &name) : name(name) {
                        }

                        /**
                         * Gets the reader name
                         *
                         * @return the reader name string
                         */
                        std::string getName() override {
                            return name;
                        }

                        /**
                         * Add an observer. This will allow to be notified about all readers or plugins events.
                         * 
                         * @param observer Observer to notify
                         */

                        virtual void addObserver(std::shared_ptr<AbstractLoggedObservable::Observer<T>> observer) {

                            logger->trace("[{}][{}] addObserver => Adding an observer.", this->getClass(), this->getName());

                            Observable<T>::addObserver(observer);
                        }

                        /**
                         * Remove an observer.
                         * 
                         * @param observer Observer to stop notifying
                         */

                        virtual void removeObserver(std::shared_ptr<AbstractLoggedObservable::Observer<T>> observer) {

                            if (std::dynamic_pointer_cast<AbstractObservableReader>(shared_from_this()) != nullptr) {
                                logger->trace("[{}] removeObserver => Deleting a reader observer", this->getName());
                            }
                            else if (std::dynamic_pointer_cast<AbstractObservablePlugin>(shared_from_this()) != nullptr) {
                                logger->trace("[{}] removeObserver => Deleting a plugin observer", this->getName());
                            }

                            Observable<T>::removeObserver(observer);
                        }



                        /**
                         * This method shall be called only from a SE Proxy plugin or reader implementing
                         * AbstractObservableReader or AbstractObservablePlugin. Push a ReaderEvent / PluginEvent of the
                         * selected AbstractObservableReader / AbstractObservablePlugin to its registered Observer.
                         * 
                         * @param event the event
                         */

                        void notifyObservers(T const event_Renamed) final override {

                            if (std::dynamic_pointer_cast<AbstractObservableReader>(shared_from_this()) != nullptr) {
                                logger->trace("[{}] AbstractObservableReader => Notifying a reader event. EVENTNAME = {}", this->getName(), (std::static_pointer_cast<ReaderEvent>(event_Renamed))->getEventType().getName());
                            }
                            else if (std::dynamic_pointer_cast<AbstractObservablePlugin>(shared_from_this()) != nullptr) {
                                logger->trace("[{}] AbstractObservableReader => Notifying a plugin event. EVENTNAME = {} ", this->getName(), (std::static_pointer_cast<PluginEvent>(event_Renamed))->getEventType().getName());
                            }

                            setChanged();

                            Observable<T>::notifyObservers(event_Renamed);
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

                }
            }
        }
    }
}
