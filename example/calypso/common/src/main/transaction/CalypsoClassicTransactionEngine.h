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
#include <iostream>
#include <stdexcept>
#include <memory>

#include "AbstractReaderObserverEngine.h"
#include "PoTransaction.h"
#include "SeSelection.h"

namespace keyple {
namespace example {
namespace calypso {
namespace common {
namespace transaction {

using namespace keyple::calypso::transaction;
using namespace keyple::example::generic::common;
using namespace keyple::core::selection;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::exception;
using namespace keyple::common;

/**
 * This Calypso demonstration code consists in:
 *
 * <ol>
 * <li>Setting up a two-reader configuration and adding an observer method
 * ({@link #update update})
 * <li>Starting a card operation when a PO presence is notified
 * ({@link #processSeMatch(SelectionResponse)} operateSeTransaction})
 * <li>Opening a logical channel with the SAM (C1 SAM is expected) see
 * ({@link CalypsoClassicInfo#SAM_C1_ATR_REGEX SAM_C1_ATR_REGEX})
 * <li>Attempting to open a logical channel with the PO with 3 options:
 * <ul>
 * <li>Selecting with a fake AID (1)
 * <li>Selecting with the Calypso AID and reading the event log file
 * <li>Selecting with a fake AID (2)
 * </ul>
 * <li>Display {@link SelectionResponse} data
 * <li>If the Calypso selection succeeded, do a Calypso transaction
 * ({doCalypsoReadWriteTransaction(PoTransaction, ApduResponse, boolean)}
 * doCalypsoReadWriteTransaction}).
 * </ol>
 *
 * <p>
 * The Calypso transactions demonstrated here shows the Keyple API in use with
 * Calypso SE (PO and SAM).
 *
 * <p>
 * Read the doc of each methods for further details.
 */
class CalypsoClassicTransactionEngine : public AbstractReaderObserverEngine {
public:
    /**
     * Constructor
     */
    CalypsoClassicTransactionEngine();

    /**
     *
     */
    virtual ~CalypsoClassicTransactionEngine()
    {
    }

    /**
     * Assign readers to the transaction engine
     */
    virtual void setReaders(std::shared_ptr<SeReader> poReader,
                            std::shared_ptr<SeReader> samReader);

    /**
     * Do a Calypso transaction
     * <p>
     * Nominal case (the previous transaction was ratified):
     * <ul>
     * <li>Process opening
     * <ul>
     * <li>Reading the event log file</li>
     * <li>Reading the contract list</li>
     * </ul>
     * </li>
     * <li>Process PO commands
     * <ul>
     * <li>Reading the 4 contracts</li>
     * </ul>
     * </li>
     * <li>Process closing
     * <ul>
     * <li>A new record is appended to the event log file</li>
     * <li>The session is closed in CONTACTLESS_MODE (a ratification command is
     * sent)</li>
     * </ul>
     * </li>
     * </ul>
     * <p>
     * Alternate case (the previous transaction was not ratified):
     * <ul>
     * <li>Process opening
     * <ul>
     * <li>Reading the event log file</li>
     * <li>Reading the contract list</li>
     * </ul>
     * </li>
     * <li>Process closing
     * <ul>
     * <li>The session is closed in CONTACTLESS_MODE (a ratification command is
     * sent)</li>
     * </ul>
     * </li>
     * </ul>
     * <p>
     * The PO logical channel is kept open or closed according to the
     * closeSeChannel flag
     *
     * @param poTransaction PoTransaction object
     * @param closeSeChannel flag to ask or not the channel closing at the end
     *        of the transaction
     * @throws KeypleReaderException reader exception (defined as public for
     *         purposes of javadoc)
     */
    virtual void
    doCalypsoReadWriteTransaction(std::shared_ptr<PoTransaction> poTransaction,
                                  bool closeSeChannel);

    /**
     *
     */
    virtual std::shared_ptr<AbstractDefaultSelectionsRequest>
    preparePoSelection();

    /**
     * Do the PO selection and possibly go on with Calypso transactions.
     */
    void processSeMatch(
        const std::shared_ptr<AbstractDefaultSelectionsResponse>
                  selectionResponse) override;

    /**
     *
     */
    void processSeInserted() override;

    /**
     *
     */
    void processSeRemoval() override;

    /**
     *
     */
    void processUnexpectedSeRemoval() override;

protected:
    /**
     *
     */
    std::shared_ptr<CalypsoClassicTransactionEngine> shared_from_this()
    {
        return std::static_pointer_cast<CalypsoClassicTransactionEngine>(
            AbstractReaderObserverEngine::shared_from_this());
    }

private:
    /**
     *
     */
    static std::shared_ptr<Logger> logger;

    /**
     * Define the SAM parameters to provide when creating PoTransaction
     */
    static const std::shared_ptr<SecuritySettings> securitySettings;

    /**
     *
     */
    std::shared_ptr<SeReader> poReader, samReader;

    /**
     *
     */
    std::shared_ptr<SeSelection> seSelection;

    /**
     *
     */
    std::shared_ptr<SamResource> samResource;

    /**
     *
     */
    bool samChannelOpen = false;
};

}
}
}
}
}
