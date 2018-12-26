#pragma once

#include "../../transport/RemoteMethodTx.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ObservableReader.h"
#include <string>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace transaction { class SelectionRequest; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace @event { class ObservableReader; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class KeypleDto; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class KeypleRemoteException; } } } } } }

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

                            using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::KeypleDto;
                            using KeypleRemoteException = org::eclipse::keyple::plugin::remotese::transport::KeypleRemoteException;
                            using RemoteMethodTx = org::eclipse::keyple::plugin::remotese::transport::RemoteMethodTx;
                            using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                            using SelectionRequest = org::eclipse::keyple::transaction::SelectionRequest;

                            class RmSetDefaultSelectionRequestTx : public RemoteMethodTx {

                            private:
                                std::shared_ptr<SelectionRequest> selectionRequest;
                                ObservableReader::NotificationMode notificationMode = static_cast<ObservableReader::NotificationMode>(0);


                            public:
                                RmSetDefaultSelectionRequestTx(std::shared_ptr<SelectionRequest> selectionRequest, ObservableReader::NotificationMode notificationMode, const std::string &nativeReaderName, const std::string &virtualReaderName, const std::string &sessionId, const std::string &clientNodeId);


                                std::shared_ptr<void> parseResponse(std::shared_ptr<KeypleDto> keypleDto) throw(KeypleRemoteException) override;

                                std::shared_ptr<KeypleDto> dto() override;

protected:
                                std::shared_ptr<RmSetDefaultSelectionRequestTx> shared_from_this() {
                                    return std::static_pointer_cast<RmSetDefaultSelectionRequestTx>(org.eclipse.keyple.plugin.remotese.transport.RemoteMethodTx::shared_from_this());
                                }
                            };

                        }
                    }
                }
            }
        }
    }
}
