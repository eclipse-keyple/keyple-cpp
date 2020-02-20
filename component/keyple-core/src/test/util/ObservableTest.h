#pragma once

#include <string>
#include <memory>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "Observable.h"

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
        namespace keyple {
            namespace core {
                namespace util {

                using namespace testing;
                
                class ObservableTest : public std::enable_shared_from_this<ObservableTest> {
                public:
                    class Event : public std::enable_shared_from_this<Event> {
                    private:
                        const std::string name;

                    public:
                        Event(const std::string &name);

                        virtual std::string getName();

                        std::string toString();
                    };

                public:
                    class EventPlus : public Event {

                    public:
                        EventPlus(const std::string &name);

                        std::string toString();

protected:
                        std::shared_ptr<EventPlus> shared_from_this() {
                            return std::static_pointer_cast<EventPlus>(Event::shared_from_this());
                        }
                    };

                //public:
                //    class Observer : public std::enable_shared_from_this<Observer>, public Observer//<std::shared_ptr<Event>> 
                //    {
                //    private:
                //        int nbCalls = 0;

                //    public:
                //        virtual int getNbCalls();

                //        void update(std::shared_ptr<Event> arg) override;
                //    };

                public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void sample()
                    virtual void sample();
                };

            }
        }
    }
