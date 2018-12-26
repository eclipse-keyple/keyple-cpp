#pragma once

#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ObservableReader.h"
#include <string>
#include <unordered_set>
#include <vector>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleBaseException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace transaction { class SelectionRequest; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace @event { class ObservableReader; } } } } }

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
            namespace plugin {
                namespace remotese {
                    namespace common {
                        namespace json {


                            using namespace org::eclipse::keyple::seproxy;
                            using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                            using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                            using namespace org::eclipse::keyple::seproxy::message;
                            using SelectionRequest = org::eclipse::keyple::transaction::SelectionRequest;

                            class SampleFactory : public std::enable_shared_from_this<SampleFactory> {

                            public:
                                static std::shared_ptr<KeypleBaseException> getAStackedKeypleException();

                                static std::shared_ptr<KeypleBaseException> getASimpleKeypleException();

                                static std::shared_ptr<SelectionRequest> getSelectionRequest();

                                static ObservableReader::NotificationMode getNotificationMode();

                                static std::shared_ptr<SeRequestSet> getASeRequestSet_ISO14443_4();


                                static std::shared_ptr<SeRequestSet> getASeRequestSet();

                                static std::shared_ptr<SeRequest> getASeRequest_ISO14443_4();

                                static std::shared_ptr<SeRequest> getASeRequest();

                                static std::shared_ptr<SeRequestSet> getCompleteRequestSet();

                                static std::shared_ptr<SeResponseSet> getCompleteResponseSet();



                            };

                        }
                    }
                }
            }
        }
    }
}
