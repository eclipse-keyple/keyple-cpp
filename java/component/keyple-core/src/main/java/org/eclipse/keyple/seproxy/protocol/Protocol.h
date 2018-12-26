#pragma once

#include "SeProtocol.h"
#include <string>
#include <vector>

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
                namespace protocol {


                    class Protocol final : public SeProtocol {

                        // Unspecified protocol
public:
                        static Protocol ANY;

private:
                        static std::vector<Protocol> valueList;

                        class StaticConstructor {
                        public:
                            StaticConstructor();
                        };

                        static StaticConstructor staticConstructor;

public:
                        enum class InnerEnum {
                            ANY
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
                        Protocol(const std::string &name);

                        virtual std::string getName();

public:
                        bool operator == (const Protocol &other);

                        bool operator != (const Protocol &other);

                        static std::vector<Protocol> values();

                        int ordinal();

                        std::string toString();

                        static Protocol valueOf(const std::string &name);
                    };

                }
            }
        }
    }
}
