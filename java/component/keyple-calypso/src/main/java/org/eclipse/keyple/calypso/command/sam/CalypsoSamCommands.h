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
            namespace calypso {
                namespace command {
                    namespace sam {

                        using namespace org::eclipse::keyple::calypso::command::sam::builder::session;
                        using namespace org::eclipse::keyple::calypso::command::sam::parser::session;

                        class CalypsoSamCommands final : public CommandsTable {

                            /** The sam select diversifier. */
public:
                            static CalypsoSamCommands SELECT_DIVERSIFIER;

                            /** The sam get challenge. */
                            static CalypsoSamCommands GET_CHALLENGE;

                            /** The sam digest init. */
                            static CalypsoSamCommands DIGEST_INIT;

                            /** The sam digest update. */
                            static CalypsoSamCommands DIGEST_UPDATE;

                            /** The sam digest update multiple. */
                            static CalypsoSamCommands DIGEST_UPDATE_MULTIPLE;

                            /** The sam digest close. */
                            static CalypsoSamCommands DIGEST_CLOSE;

                            /** The sam digest authenticate. */
                            static CalypsoSamCommands DIGEST_AUTHENTICATE;

private:
                            static std::vector<CalypsoSamCommands> valueList;

                            class StaticConstructor {
                            public:
                                StaticConstructor();
                            };

                            static StaticConstructor staticConstructor;

public:
                            enum class InnerEnum {
                                SELECT_DIVERSIFIER,
                                GET_CHALLENGE,
                                DIGEST_INIT,
                                DIGEST_UPDATE,
                                DIGEST_UPDATE_MULTIPLE,
                                DIGEST_CLOSE,
                                DIGEST_AUTHENTICATE
                            };

                            const InnerEnum innerEnumValue;
private:
                            const std::string nameValue;
                            const int ordinalValue;
                            static int nextOrdinal;

                            /** The name. */
                        private:
                            const std::string name;

                            /** The instruction byte. */
                            const char instructionbyte;

                            /** The command builder class. */
                            const std::type_info commandBuilderClass;

                            /** The response parser class. */
                            const std::type_info responseParserClass;

                            /**
                             * The generic constructor of CalypsoCommands.
                             *
                             * @param name the name
                             * @param instructionByte the instruction byte
                             * @param commandBuilderClass the command builder class
                             * @param responseParserClass the response parser class
                             */
                        public:
                            CalypsoSamCommands(const std::string &name, InnerEnum innerEnum, const std::string &name, char instructionByte, std::type_info commandBuilderClass, std::type_info responseParserClass);

                            /**
                             * Gets the name.
                             *
                             * @return the command name
                             */
                            virtual std::string getName();

                            /**
                             * Gets the instruction byte.
                             *
                             * @return the value of INS byte
                             */
                            virtual char getInstructionByte();

                            /**
                             * Gets the command builder class.
                             *
                             * @return the corresponding command builder class
                             */
                            virtual std::type_info getCommandBuilderClass();

                            /**
                             * Gets the response parser class.
                             *
                             * @return the corresponding response parser class
                             */
                            virtual std::type_info getResponseParserClass();

public:
                            bool operator == (const CalypsoSamCommands &other);

                            bool operator != (const CalypsoSamCommands &other);

                            static std::vector<CalypsoSamCommands> values();

                            int ordinal();

                            std::string toString();

                            static CalypsoSamCommands valueOf(const std::string &name);
                        };

                    }
                }
            }
        }
    }
}
