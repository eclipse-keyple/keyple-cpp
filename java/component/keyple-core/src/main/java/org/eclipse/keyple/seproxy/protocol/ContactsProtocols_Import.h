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

/* Common */
#include "Export.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace protocol {


                    class IMPORT ContactsProtocols final : public SeProtocol {

                        // Contact standard
public:
                        static ContactsProtocols PROTOCOL_ISO7816_3;

private:
                        static std::vector<ContactsProtocols> valueList;

                        class StaticConstructor {
                        public:
                            StaticConstructor();
                        };

                        static StaticConstructor staticConstructor;

public:
                        enum class InnerEnum {
                            PROTOCOL_ISO7816_3
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
                        ContactsProtocols(const std::string &nameValue, InnerEnum innerEnum, const std::string &name);

                        virtual std::string getName();

public:
                        bool operator == (const ContactsProtocols &other);

                        bool operator != (const ContactsProtocols &other);

                        static std::vector<ContactsProtocols> values();

                        int ordinal();

                        std::string toString();

                        static ContactsProtocols valueOf(const std::string &name);
                    };

                }
            }
        }
    }
}
