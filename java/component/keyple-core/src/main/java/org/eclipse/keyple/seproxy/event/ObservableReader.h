#pragma once

#include "../SeReader.h"
#include "../../util/Observable.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace @event { class ReaderEvent; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace transaction { class SelectionRequest; } } } }

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
                namespace event_Renamed {

                    using SeReader = org::eclipse::keyple::seproxy::SeReader;
                    using SelectionRequest = org::eclipse::keyple::transaction::SelectionRequest;
                    using Observable = org::eclipse::keyple::util::Observable;

                    class ObservableReader : public SeReader {
                    private:
                        class ReaderObserver : public Observable::Observer<std::shared_ptr<ReaderEvent>> {
                        };

                    public:
                        enum class NotificationMode {
                            ALWAYS,
                            MATCHED_ONLY
                        };

                    public:
                        virtual void addObserver(std::shared_ptr<ReaderObserver> observer) = 0;

                        virtual void removeObserver(std::shared_ptr<ReaderObserver> observer) = 0;

                        virtual void notifyObservers(std::shared_ptr<ReaderEvent> event_Renamed) = 0;

                        virtual void setDefaultSelectionRequest(std::shared_ptr<SelectionRequest> defaultSelectionRequest, NotificationMode notificationMode) = 0;
                    };

                }
            }
        }
    }
}
