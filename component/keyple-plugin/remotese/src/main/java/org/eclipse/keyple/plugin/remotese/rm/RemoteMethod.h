#pragma once

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
            namespace plugin {
                namespace remotese {
                    namespace rm {


                        class RemoteMethod final {

public:
                            static RemoteMethod READER_TRANSMIT;
                            static RemoteMethod READER_CONNECT;
                            static RemoteMethod READER_DISCONNECT;
                            static RemoteMethod READER_EVENT;
                            static RemoteMethod DEFAULT_SELECTION_REQUEST;

private:
                            static std::vector<RemoteMethod> valueList;

public:
                            enum class InnerEnum {
                                READER_TRANSMIT,
                                READER_CONNECT,
                                READER_DISCONNECT,
                                READER_EVENT,
                                DEFAULT_SELECTION_REQUEST
                            };

                            const InnerEnum innerEnumValue;
private:
                            const std::string nameValue;
                            const int ordinalValue;
                            static int nextOrdinal;

                        private:
                            std::string name;

                        public:
                            RemoteMethod(const std::string &name, InnerEnum innerEnum, const std::string &name);

                            virtual std::string getName();


                            // ****** Reverse Lookup Implementation************//

                            // Lookup table
                        private:
                            static const std::unordered_map<std::string, RemoteMethod> lookup;

                            // Populate the lookup table on loading time
//JAVA TO C++ CONVERTER TODO TASK: Java to C++ Converter does not convert types within enums:
//                                                    private class StaticConstructor
                        //                            {
                        //                                public StaticConstructor()
                        //                                {
                        //            for (RemoteMethod env : RemoteMethod.values())
                        //            {
                        //                lookup.put(env.getName(), env);
                        //            }
                        //                                }
                        //                            }

                                                    static RemoteMethod::StaticConstructor staticConstructor;


                            // This method can be used for reverse lookup purpose
                        public:
                            static RemoteMethod get(const std::string &name);

public:
                            bool operator == (const RemoteMethod &other);

                            bool operator != (const RemoteMethod &other);

                            static std::vector<RemoteMethod> values();

                            int ordinal();

                            std::string toString();

                            static RemoteMethod valueOf(const std::string &name);
                        };

                    }
                }
            }
        }
    }
}
