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

/* Common */
#include "Export.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {

                    /* Class for the Calypso command: LEGACY for REV1 / BPRIME type PO, ISO for REV2/3 / B type */
                    class EXPORT PoClass final {

                      public:
                        static PoClass LEGACY;
                        static PoClass ISO;

                      private:
                        static std::vector<PoClass> valueList;

                        class StaticConstructor {
                          public:
                            StaticConstructor();
                        };

                        static StaticConstructor staticConstructor;

                      public:
                        enum class InnerEnum
                        {
                            LEGACY,
                            ISO
                        };

                        InnerEnum innerEnumValue;

                      private:
                        std::string nameValue;
                        int ordinalValue;
                        static int nextOrdinal;

                      private:
                        char cla;

                      public:
                        virtual char getValue();

                        PoClass(const std::string &name, InnerEnum innerEnum, char cla);

                      public:
                        bool operator==(const PoClass &other);

                        bool operator!=(const PoClass &other);

                        void operator=(const PoClass &other);

                        static std::vector<PoClass> values();

                        int ordinal();

                        std::string toString();

                        static PoClass valueOf(const std::string &name);
                    };

                } // namespace command
            }     // namespace calypso
        }         // namespace keyple
    }             // namespace eclipse
} // namespace org
