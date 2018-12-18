#pragma once

#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.h"
#include <string>
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
            namespace plugin {
                namespace remotese {
                    namespace transport {

                        using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;

                        class KeypleRemoteException : public KeypleBaseException {

                        public:
                            KeypleRemoteException(const std::string &message, std::runtime_error cause);

                            KeypleRemoteException(const std::string &message);


protected:
                            std::shared_ptr<KeypleRemoteException> shared_from_this() {
                                return std::static_pointer_cast<KeypleRemoteException>(org.eclipse.keyple.seproxy.exception.KeypleBaseException::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
