#pragma once

#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
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

                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;

                        class KeypleRemoteReaderException : public KeypleReaderException {

                        public:
                            KeypleRemoteReaderException(const std::string &message, std::runtime_error cause);

                            KeypleRemoteReaderException(const std::string &message);


protected:
                            std::shared_ptr<KeypleRemoteReaderException> shared_from_this() {
                                return std::static_pointer_cast<KeypleRemoteReaderException>(org.eclipse.keyple.seproxy.exception.KeypleReaderException::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
