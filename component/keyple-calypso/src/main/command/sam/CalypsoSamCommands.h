/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

#pragma once

#include <string>
#include <vector>
#include <typeinfo>

/* Core */
#include "CommandsTable.h"

/* Common */
#include "Export.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {

using namespace keyple::core::command;

class EXPORT CalypsoSamCommands final : public CommandsTable {
public:
    /** The sam select diversifier. */
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

    /** The sam digest authenticate. */
    static CalypsoSamCommands GIVE_RANDOM;

    /** The sam digest authenticate. */
    static CalypsoSamCommands CARD_GENERATE_KEY;

    /** The sam unlock. */
    static CalypsoSamCommands UNLOCK;

    /** The sam write key command. */
    static CalypsoSamCommands WRITE_KEY;

    /** */
    static CalypsoSamCommands READ_KEY_PARAMETERS;

    /** */
    static CalypsoSamCommands READ_EVENT_COUNTER;

    /** */
    static CalypsoSamCommands READ_CEILINGS;

    /**
     *
     */
    enum class InnerEnum {
        SELECT_DIVERSIFIER,
        GET_CHALLENGE,
        DIGEST_INIT,
        DIGEST_UPDATE,
        DIGEST_UPDATE_MULTIPLE,
        DIGEST_CLOSE,
        DIGEST_AUTHENTICATE,
        GIVE_RANDOM,
        CARD_GENERATE_KEY,
        UNLOCK,
        WRITE_KEY,
        READ_KEY_PARAMETERS,
        READ_EVENT_COUNTER,
        READ_CEILINGS
    };

    /**
     *
     */
    const InnerEnum innerEnumValue;

    /**
     * The generic constructor of CalypsoCommands.
     *
     * @param name the name
     * @param instructionByte the instruction byte
     * @param commandBuilderClass the command builder class
     * @param responseParserClass the response parser class
     */
    CalypsoSamCommands(const std::string& nameValue, InnerEnum innerEnum,
                       const std::string& name, char instructionByte,
                       const std::type_info& commandBuilderClass,
                       const std::type_info& responseParserClass);

    /**
     * Gets the name.
     *
     * @return the command name
     */
    virtual const std::string& getName() const;

    /**
     * Gets the instruction byte.
     *
     * @return the value of INS byte
     */
    virtual uint8_t getInstructionByte() const;

    /**
     * Gets the command builder class.
     *
     * @return the corresponding command builder class
     */
    virtual const std::type_info& getCommandBuilderClass() const;

    /**
     * Gets the response parser class.
     *
     * @return the corresponding response parser class
     */
    virtual const std::type_info& getResponseParserClass() const;

    /**
     *
     */
    bool operator==(const CalypsoSamCommands& other);

    /**
     *
     */
    bool operator!=(const CalypsoSamCommands& other);

    /**
     *
     */
    static std::vector<CalypsoSamCommands> values();

    /**
     *
     */
    int ordinal();

    /**
     *
     */
    std::string toString();

    /**
     *
     */
    static CalypsoSamCommands valueOf(const std::string& name);

private:
    /**
     *
     */
    static std::vector<CalypsoSamCommands> valueList;

    /**
     *
     */
    class StaticConstructor {
    public:
        StaticConstructor();
    };

    /**
     *
     */
    static StaticConstructor staticConstructor;

    /**
     *
     */
    const std::string nameValue;

    /**
     *
     */
    const int ordinalValue;

    /**
     *
     */
    static int nextOrdinal;

    /** The name. */
    const std::string name;

    /** The instruction byte. */
    const char instructionbyte;

    /** The command builder class. */
    const std::type_info& commandBuilderClass;

    /** The response parser class. */
    const std::type_info& responseParserClass;
};

}
}
}
}
