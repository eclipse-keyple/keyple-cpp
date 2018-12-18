#pragma once

#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include <string>
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
            namespace calypso {
                namespace transaction {
                    namespace exception {

                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;

                        /**
                         * Thrown when the current PO has an unauthorized KVC
                         */
                        class KeypleCalypsoSecureSessionUnauthorizedKvcException : public KeypleReaderException {
                        public:
                            KeypleCalypsoSecureSessionUnauthorizedKvcException(const std::string &message);

protected:
                            std::shared_ptr<KeypleCalypsoSecureSessionUnauthorizedKvcException> shared_from_this() {
                                return std::static_pointer_cast<KeypleCalypsoSecureSessionUnauthorizedKvcException>(org.eclipse.keyple.seproxy.exception.KeypleReaderException::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
