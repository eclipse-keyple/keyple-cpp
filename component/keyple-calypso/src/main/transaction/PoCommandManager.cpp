/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#include "PoCommandManager.h"

/* Common */
#include "IllegalArgumentException.h"
#include "IllegalStateException.h"

/* Calypso */
#include "CalypsoPoCommand.h"
#include "CalypsoPoTransactionIllegalStateException.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command::po;
using namespace keyple::calypso::transaction::exception;
using namespace keyple::common::exception;

PoCommandManager::PoCommandManager() {}

void PoCommandManager::addRegularCommand(
    std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>> commandBuilder)
{
    mPoCommands.push_back(commandBuilder);
}

void PoCommandManager::addStoredValueCommand(
    std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>> commandBuilder,
    const Operation& svOperation)
{
    /* Check the logic of the SV command sequencing */
    auto calypso = std::dynamic_pointer_cast<CalypsoPoCommand>(commandBuilder->getCommandRef());
    CalypsoPoCommand command = *calypso;
    if (command == CalypsoPoCommand::SV_GET) {
        mSvOperation = svOperation;
    } else if (command == CalypsoPoCommand::SV_RELOAD ||
               command == CalypsoPoCommand::SV_DEBIT  ||
               command == CalypsoPoCommand::SV_UNDEBIT) {
        if (!mPoCommands.empty())
            throw CalypsoPoTransactionIllegalStateException("This SV command can only be placed " \
                                                            "in the first position in the list of" \
                                                            " prepared commands");

        if (*mSvLastCommand != CalypsoPoCommand::SV_GET)
            /* @see Calypso Layer ID 8.07/8.08 (200108) */
            throw IllegalStateException("This SV command must follow an SV Get command");

        /* Here, we expect the builder and the SV operation to be consistent */
        if (svOperation != mSvOperation) {
            mLogger->error("Sv operation = %, current command = %", mSvOperation, svOperation);
            throw CalypsoPoTransactionIllegalStateException("Inconsistent SV operation.");
        }

        mSvOperation = svOperation;
        mSvOperationComplete = true;
    } else {
        throw IllegalStateException("An SV command is expected.");
    }

    mSvLastCommand = calypso;

    mPoCommands.push_back(commandBuilder);
}

void PoCommandManager::notifyCommandsProcessed()
{
    mPoCommands.clear();
}

const std::vector<std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>>
    PoCommandManager::getPoCommandBuilders()
{
    return mPoCommands;
}

bool PoCommandManager::hasCommands() const
{
    return !mPoCommands.empty();
}

bool PoCommandManager::isSvOperationCompleteOneTime()
{
    bool flag = mSvOperationComplete;
    mSvOperationComplete = false;

    return flag;
}

}
}
}
