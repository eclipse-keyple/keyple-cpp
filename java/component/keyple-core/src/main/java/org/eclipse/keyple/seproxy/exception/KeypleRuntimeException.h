#pragma once

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
            namespace seproxy {
                namespace exception {

                    class KeypleRuntimeException : public std::runtime_error, public std::enable_shared_from_this<KeypleRuntimeException> {

                    public:
                        KeypleRuntimeException(const std::string &message);

                        KeypleRuntimeException(const std::string &message, std::runtime_error cause);
                    };

                }
            }
        }
    }
}
