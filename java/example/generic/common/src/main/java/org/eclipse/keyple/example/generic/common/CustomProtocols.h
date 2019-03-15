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

#pragma once

#include <string>
#include <vector>

/* Core */
#include "SeProtocol.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace generic {
                    namespace common {

                        using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;

                        /**
                         * Custom protocol definitions to illustrate the extension of the Keyple SDK definitions
                         */
                        class CustomProtocols final : public SeProtocol {
public:
                            static CustomProtocols CUSTOM_PROTOCOL_B_PRIME;

                            static CustomProtocols CUSTOM_PROTOCOL_MIFARE_DESFIRE;

private:
                            static std::vector<CustomProtocols> valueList;

                            class StaticConstructor {
                            public:
                                StaticConstructor();
                            };

                            static StaticConstructor staticConstructor;

public:
                            enum class InnerEnum {
                                CUSTOM_PROTOCOL_B_PRIME,
                                CUSTOM_PROTOCOL_MIFARE_DESFIRE
                            };

                            const InnerEnum innerEnumValue;
private:
                            const std::string nameValue;
                            const int ordinalValue;
                            static int nextOrdinal;

                            /** The protocol name. */
                        private:
                            std::string name;

                        public:
                            CustomProtocols(const std::string &nameValue, InnerEnum innerEnum, const std::string &name);

                            std::string getName() override;

public:
                            bool operator == (const CustomProtocols &other);

                            bool operator != (const CustomProtocols &other);

                            static std::vector<CustomProtocols> values();

                            int ordinal();

                            std::string toString();

                            static CustomProtocols valueOf(const std::string &name);
                        };

                    }
                }
            }
        }
    }
}
