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



                    /**
                     * Base Exception for all Keyple Checked Exception
                     */
                    class KeypleBaseException : public std::runtime_error, public std::enable_shared_from_this<KeypleBaseException> {
                    private:
                        static constexpr long long serialVersionUID = -500856379312027085LL;

                        /**
                         * New exception to be thrown
                         * 
                         * @param message : message to identify the exception and the context
                         */
                    public:
                        KeypleBaseException(const std::string &message);

                        /**
                         * Encapsulate a lower level exception (ie CardException, IOException, HostNotFoundException..)
                         * 
                         * @param message : message to identify the exception and the context
                         * @param cause : lower level exception
                         */
                        KeypleBaseException(const std::string &message, std::runtime_error cause);
                    };

                }
            }
        }
    }
}
