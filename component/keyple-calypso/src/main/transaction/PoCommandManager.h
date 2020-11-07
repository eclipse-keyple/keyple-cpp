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

#pragma once

/* Calypso */
#include "AbstractPoCommandBuilder.h"
#include "AbstractPoResponseParser.h"
#include "PoTransaction.h"

/* Common */
#include "LoggerFactory.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command::po;
using namespace keyple::common;

using Operation = PoTransaction::SvSettings::Operation;

/**
 * The PO command manager handles the AbstractPoCommandBuilder list updated by the "prepare" methods
 * of PoTransaction. It is used to keep builders between the time the commands are created and the
 * time their responses are parsed.
 * <p>
 * A flag (preparedCommandsProcessed) is used to manage the reset of the command list. It allows the
 * builders to be kept until the application creates a new list of commands.
 * <p>
 * This flag is set by calling the method notifyCommandsProcessed and reset when a new
 * AbstractPoCommandBuilder is added or when a attempt
 */
class PoCommandManager final {
public:
    /**
     * (package-private)<br>
     * Constructor
     */
    PoCommandManager();

    /**
     * (package-private)<br>
     * Add a regular command to the builders and parsers list.
     *
     * @param commandBuilder the command builder
     */
    void addRegularCommand(
        std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>> commandBuilder);

    /**
     * (package-private)<br>
     * Add a StoredValue command to the builders and parsers list.
     * <p>
     * Set up a mini state machine to manage the scheduling of Stored Value commands.
     * <p>
     * The {@link PoTransaction.SvSettings.Operation} and {@link PoTransaction.SvSettings.Action}
     * are also used to check the consistency of the SV process.
     * <p>
     * The svOperationPending flag is set when an SV operation (Reload/Debit/Undebit) command is
     * added.
     *
     * @param commandBuilder the StoredValue command builder
     * @param svOperation the type of the current SV operation (Realod/Debit/Undebit)
     * @throw IllegalStateException if the provided command is not an SV command
     * @throw CalypsoPoTransactionIllegalStateException if the SV API is not properly used.
     */
    void addStoredValueCommand(
        std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>> commandBuilder,
        const Operation& svOperation);

    /**
     * (package-private)<br>
     * Informs that the commands have been processed.
     * <p>
     * Just record the information. The initialization of the list of commands
     * will be done only the next time a command is added, this allows access to
     * the parsers contained in the list..
     */
    void notifyCommandsProcessed();

    /**
     * (package-private)<br>
     *
     * @return the current AbstractPoCommandBuilder list
     */
    const std::vector<std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>>
        getPoCommandBuilders();

    /**
     * (package-private)<br>
     *
     * @return true if the {@link PoCommandManager} has commands
     */
    bool hasCommands() const;

    /**
     * (package-private)<br>
     * Indicates whether an SV Operation has been completed (Reload/Debit/Undebit requested) <br>
     * This method is dedicated to triggering the signature verification after an SV transaction has
     * been executed. It is a single-use method, as the flag is systematically reset to false after
     * it is called.
     *
     * @return true if a reload or debit command has been requested
     */
    bool isSvOperationCompleteOneTime();

private:
    /**
     * The list to contain the prepared commands
     */
    std::vector<std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>> mPoCommands;

    /**
     *
     */
    const std::shared_ptr<Logger> mLogger = LoggerFactory::getLogger(typeid(PoCommandManager));

    /**
     *
     */
    std::shared_ptr<CalypsoPoCommand> mSvLastCommand;

    /**
     *
     */
    Operation mSvOperation = Operation::NONE;

    /**
     *
     */
    bool mSvOperationComplete = false;
};

}
}
}
