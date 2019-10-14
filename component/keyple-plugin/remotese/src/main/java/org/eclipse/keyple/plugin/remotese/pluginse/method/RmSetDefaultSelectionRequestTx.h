#pragma once

#include "../../rm/RemoteMethodTx.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ObservableReader.h"
#include <string>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace @event { class DefaultSelectionRequest; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace @event { class ObservableReader; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace exception { class KeypleRemoteException; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace model { class KeypleDto; } } } } } } }

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
                    namespace pluginse {
                        namespace method {

                            using KeypleRemoteException = org::eclipse::keyple::plugin::remotese::exception::KeypleRemoteException;
                            using RemoteMethodTx = org::eclipse::keyple::plugin::remotese::rm::RemoteMethodTx;
                            using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::model::KeypleDto;
                            using DefaultSelectionRequest = org::eclipse::keyple::seproxy::event_Renamed::DefaultSelectionRequest;
                            using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;

                            class RmSetDefaultSelectionRequestTx : public RemoteMethodTx {

                            private:
                                std::shared_ptr<DefaultSelectionRequest> defaultSelectionRequest;
                                ObservableReader::NotificationMode notificationMode = static_cast<ObservableReader::NotificationMode>(0);


                            public:
                                RmSetDefaultSelectionRequestTx(std::shared_ptr<DefaultSelectionRequest> defaultSelectionRequest, ObservableReader::NotificationMode notificationMode, const std::string &nativeReaderName, const std::string &virtualReaderName, const std::string &sessionId, const std::string &clientNodeId);


                                std::shared_ptr<void> parseResponse(std::shared_ptr<KeypleDto> keypleDto) throw(KeypleRemoteException) override;

                                std::shared_ptr<KeypleDto> dto() override;

protected:
                                std::shared_ptr<RmSetDefaultSelectionRequestTx> shared_from_this() {
                                    return std::static_pointer_cast<RmSetDefaultSelectionRequestTx>(org.eclipse.keyple.plugin.remotese.rm.RemoteMethodTx::shared_from_this());
                                }
                            };

                        }
                    }
                }
            }
        }
    }
}
