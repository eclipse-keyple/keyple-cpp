/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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

#include "AbstractPoCommandBuilder.h"
#include "AbstractPoResponseParser.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command::po;

/**
 * The PO command manager handles the AbstractPoCommandBuilder list updated by
 * the "prepare" methods of PoTransaction. It is used to keep builders between
 * the time the commands are created and the time their responses are parsed.
 * <p>
 * A flag (preparedCommandsProcessed) is used to manage the reset of the command
 * list. It allows the builders to be kept until the application creates a new
 * list of commands.
 * <p>
 * This flag is set by calling the method notifyCommandsProcessed and reset when
 * a new AbstractPoCommandBuilder is added or when a attempt
 */
class PoCommandManager final {
public:
    /**
     *
     */
    PoCommandManager();

    /**
     * Add a regular command to the builders and parsers list.
     * <p>
     * Handle the clearing of the list if needed.
     *
     * @param commandBuilder the command builder
     */
    void addRegularCommand(
        std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>
            commandBuilder);

    /**
     * Informs that the commands have been processed.
     * <p>
     * Just record the information. The initialization of the list of commands
     * will be done only the next time a command is added, this allows access to
     * the parsers contained in the list..
     */
    void notifyCommandsProcessed();

    /**
     * @return the current AbstractPoCommandBuilder list
     */
    const std::vector<std::shared_ptr<AbstractPoCommandBuilder<
        AbstractPoResponseParser>>> getPoCommandBuilders();

    /**
     * Returns the parser positioned at the indicated index
     *
     * @param commandIndex the index of the wanted parser
     * @return the parser
     */
    std::shared_ptr<AbstractApduResponseParser> getResponseParser(
        const int commandIndex) const;

private:
    /**
     * The list to contain the prepared commands and their parsers
     */
    std::vector<std::shared_ptr<AbstractPoCommandBuilder<
        AbstractPoResponseParser>>> mPoCommands;

    /**
     * The command index, incremented each time a command is added
     */
    bool mPreparedCommandsProcessed;
};

}
}
}
