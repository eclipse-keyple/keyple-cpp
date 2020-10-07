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

#include "PoCommandManager.h"

/* Common */
#include "IllegalArgumentException.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::common;

PoCommandManager::PoCommandManager() :  mPreparedCommandsProcessed(true) {}

void PoCommandManager::addRegularCommand(
    std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>
        commandBuilder) {
    /*
     * Reset the list if the preparation of the command is done after a previous
     * processing notified by notifyCommandsProcessed.
     *
     * However, the parsers have remained available until now.
     */
    if (mPreparedCommandsProcessed) {
        mPoCommands.clear();
        mPreparedCommandsProcessed = false;
    }

    mPoCommands.push_back(commandBuilder);
}

void PoCommandManager::notifyCommandsProcessed()
{
    mPreparedCommandsProcessed = true;
}

const std::vector<std::shared_ptr<AbstractPoCommandBuilder<
    AbstractPoResponseParser>>> PoCommandManager::getPoCommandBuilders()
{
    /*
     * Clear the list if no command has been added since the last call to a
     * process method
     */
    if (mPreparedCommandsProcessed) {
        mPoCommands.clear();
        mPreparedCommandsProcessed = false;
    }

    return mPoCommands;
}

std::shared_ptr<AbstractApduResponseParser> PoCommandManager::getResponseParser(
    const int commandIndex) const
{
    if (commandIndex < 0 ||
        commandIndex >= static_cast<int>(mPoCommands.size()))
        throw IllegalArgumentException(
                  "Bad command index: " \
                  "index = " + std::to_string(commandIndex) + ", " +
                  "number of commands = " + std::to_string(mPoCommands.size()));

    return nullptr;
}


}
}
}
