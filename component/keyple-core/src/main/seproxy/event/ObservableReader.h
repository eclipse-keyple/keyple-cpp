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

#include <string>
#include <vector>
#include <memory>

/* Common */
#include "Export.h"

/* Core */
#include "AbstractDefaultSelectionsRequest.h"
#include "Observable.h"
#include "ReaderEvent_Import.h"
#include "SeReader.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace event {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;

class EXPORT ObservableReader : public virtual SeReader {
public:
    /**
     *
     */
    class ReaderObserver : public Observer<ReaderEvent> {
    public:
        /**
         *
         */
        virtual ~ReaderObserver() {}

        /**
         *
         */
        virtual void update(std::shared_ptr<ReaderEvent> event) = 0;
    };

    class EXPORT NotificationMode final {
    public:
        /**
         *
         */
        static NotificationMode ALWAYS;

        /**
         *
         */
        static NotificationMode MATCHED_ONLY;

        /**
         *
         */
        enum class InnerEnum {
            ALWAYS,
            MATCHED_ONLY
        };

        /*
         * Alex: removed 'const'
         *
         * Rationale: error: object of type 'org::eclipse::keyple::seproxy::event::ObservableReader::NotificationMode'
         * cannot be assigned because its copy assignment operator is implicitly deleted
         *   this->notificationMode = notificationMode;
         *                          ^
         * note: copy assignment operator of 'NotificationMode' is implicitly deleted because field 'innerEnumValue'
         * is of const-qualified type 'const org::eclipse::keyple::seproxy::event::ObservableReader::NotificationMode::InnerEnum'
         */
        InnerEnum innerEnumValue;

        /**
         *
         */
        NotificationMode(const std::string &nameValue, InnerEnum innerEnum,
                         const std::string &name);

        /**
         *
         */
        NotificationMode(const NotificationMode& o);

        /**
         *
         */
        virtual ~NotificationMode() {}

        /**
         *
         */
        virtual std::string getName();


        /**
         * This method can be used for reverse lookup purpose
         */
        static NotificationMode get(const std::string &name);

        /**
         *
         */
        bool operator==(const NotificationMode &other);

        /**
         *
         */
        bool operator!=(const NotificationMode &other);

        /**
         *
         */
        NotificationMode& operator=(NotificationMode o);

        /**
         *
         */
        static std::vector<NotificationMode> values();

        /**
         *
         */
        int ordinal();

        /**
         *
         */
        std::string toString();

        /**
         *
         */
        static NotificationMode valueOf(const std::string &name);

    protected:
        /**
         *
         */
        std::string name;

    private:
        /**
         *
         */
        static std::vector<NotificationMode> valueList;

        /*
         * Alex: removed 'const'
         *
         * Rationale: error: object of type 'org::eclipse::keyple::seproxy::event::ObservableReader::NotificationMode'
         * cannot be assigned because its copy assignment operator is implicitly deleted
         *   this->notificationMode = notificationMode;
         *                          ^
         * note: copy assignment operator of 'NotificationMode' is implicitly deleted becaus field 'nameValue' has no
         * copy assignment operator
         *   const std::string nameValue;
         *                     ^
         */
        std::string nameValue;

        /*
         * Alex: removed 'const'
         * Rationale: same as above
         */
        int ordinalValue;

        /**
         *
         */
        static int nextOrdinal;


        /**
         * Reverse Lookup Implementation
         * <p>
         * The purpose of the lookup Map and its associated method is to allow the serialization and
         * deserialization of the enum of the notification mode, especially in remote context.
         */
        static const std::unordered_map<std::string, NotificationMode> lookup;

        /**
         * Populating the lookup table on loading time
         */
        class StaticConstructor {
        public:
            /**
             *
             */
            StaticConstructor()
            {
                //                                    for (NotificationMode env : NotificationMode.values())
                //                                    {
                //                                        lookup.put(env.getName(), env);
                //                                    }
            }
        };

        /**
         *
         */
        static NotificationMode::StaticConstructor staticConstructor;
    };

    /**
     *
     */
    virtual ~ObservableReader() {}

    /**
     *
     */
    virtual void addObserver(std::shared_ptr<ReaderObserver> observer) = 0;

    /**
     *
     */
    virtual void removeObserver(std::shared_ptr<ReaderObserver> observer) = 0;

    /**
     *
     */
    virtual void notifyObservers(std::shared_ptr<ReaderEvent> event) { (void)event; }

    /**
     *
     */
    virtual void setDefaultSelectionRequest(std::shared_ptr<AbstractDefaultSelectionsRequest> defaultSelectionsRequest,
                                            NotificationMode notificationMode) = 0;
};

}
}
}
}
