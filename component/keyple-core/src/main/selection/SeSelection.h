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

#include <vector>
#include <memory>

#include "AbstractDefaultSelectionsRequest.h"
#include "AbstractDefaultSelectionsResponse.h"
#include "AbstractSeSelectionRequest.h"
#include "ChannelControl.h"
#include "MultiSeRequestProcessing.h"
#include "DefaultSelectionsResponse.h"
#include "KeypleReaderException.h"
#include "SelectionsResult.h"
#include "SeReader.h"
#include "SeRequest.h"

/* Common */
#include "KeypleCoreExport.h"
#include "LoggerFactory.h"

namespace keyple {
namespace core {
namespace selection {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::common;

/**
 * The SeSelection class handles the SE selection process.
 * <p>
 * It provides a way to do explicit SE selection or to post process a default SE selection. <br>
 * The channel is kept open by default, but can be closed after each selection cases (see
 * PrepareReleaseSeChannel).
 */
class KEYPLECORE_API SeSelection final
: public std::enable_shared_from_this<SeSelection> {
public:
    /**
     * Constructor.
     *
     * @param multiSeRequestProcessing the multi se processing mode
     */
    SeSelection(MultiSeRequestProcessing multiSeRequestProcessing);

    /**
     * Alternate constructor for standard usages.
     */
    SeSelection();

    /**
     * Prepare a selection: add the selection request from the provided selector to the selection
     * request set.
     * <p>
     *
     * @param seSelectionRequest the selector to prepare
     * @return the selection index giving the current selection position in the selection request.
     */
    int prepareSelection(
        std::shared_ptr<AbstractSeSelectionRequest<AbstractApduCommandBuilder>> seSelectionRequest);

    /**
     * Prepare to close the SE channel.<br>
     * If this command is called before a "process" selection command then the last transmission to
     * the PO will be associated with the indication CLOSE_AFTER in order to close the SE
     * channel.<br>
     * This makes it possible to chain several selections on the same SE if necessary.
     */
    void prepareReleaseSeChannel();

    /**
     * Parses the response to a selection operation sent to a SE and return a list of
     * keyple::core::selection::AbstractMatchingSe
     * <p>
     * Selection cases that have not matched the current SE are set to null.
     *
     * @param defaultSelectionsResponse the response from the reader to the
     *        keyple::core::seproxy::event::AbstractDefaultSelectionsRequest
     * @return the keyple::core::selection::SelectionsResult containing the result of all prepared
     *         selection cases, including keyple::core::selection::AbstractMatchingSe and
     *         keyple::core::seproxy::message::SeResponse.
     * @throw KeypleException if an error occurs during the selection process
     */
    std::shared_ptr<SelectionsResult> processDefaultSelection(
        std::shared_ptr<AbstractDefaultSelectionsResponse> defaultSelectionsResponse);

    /**
     * Execute the selection process and return a list of
     * keyple::core::selection::AbstractMatchingSe.
     * <p>
     * Selection requests are transmitted to the SE through the supplied SeReader.
     * <p>
     * The process stops in the following cases:
     * <ul>
     * <li>All the selection requests have been transmitted</li>
     * <li>A selection request matches the current SE and the keepChannelOpen
     * flag was true</li>
     * </ul>
     * <p>
     *
     * @param seReader the SeReader on which the selection is made
     * @return the keyple::core::selection::SelectionsResult containing the result of all prepared
     *         selection cases, including keyple::core::selection::AbstractMatchingSe and
     *         keyple::core::seproxy::message::SeResponse.
     * @throw KeypleReaderIOException if the communication with the reader or the SE has failed
     * @throw KeypleException if an error occurs during the selection process
     */
    std::shared_ptr<SelectionsResult> processExplicitSelection(std::shared_ptr<SeReader> seReader);

    /**
     * The SelectionOperation is the keyple::core::seproxy::event::AbstractDefaultSelectionsRequest
     * to process in ordered to select a SE among others through the selection process. This method
     * is useful to build the prepared selection to be executed by a reader just after a SE
     * insertion.
     *
     * @return the keyple::core::seproxy::event::AbstractDefaultSelectionsRequest previously
     *         prepared with prepareSelection
     */
    std::shared_ptr<AbstractDefaultSelectionsRequest> getSelectionOperation();

    /**
     *
     */
    friend KEYPLECORE_API std::ostream& operator<<(std::ostream& os, const SeSelection& ss);

    /**
     *
     */
    friend KEYPLECORE_API std::ostream& operator<<(std::ostream& os,
                                                   const std::unique_ptr<SeSelection>& ss);

    /**
     *
     */
    friend KEYPLECORE_API std::ostream& operator<<(std::ostream& os,
                                                   const std::shared_ptr<SeSelection>& ss);

private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(SeSelection));

    /*
     * list of selection requests used to build the AbstractMatchingSe list in
     * return of processSelection methods
     */
    std::vector<std::shared_ptr<AbstractSeSelectionRequest<AbstractApduCommandBuilder>>>
        mSeSelectionRequests;

    /**
     *
     */
    //int mSelectionIndex = 0; // not used

    /**
     *
     */
    const MultiSeRequestProcessing mMultiSeRequestProcessing;

    /**
     *
     */
    ChannelControl mChannelControl = ChannelControl::KEEP_OPEN;

    /**
     * Process the selection response either from a
     * {@link org.eclipse.keyple.core.seproxy.event.ReaderEvent} (default
     * selection) or from an explicit selection.
     * <p>
     * The responses from the List of {@link SeResponseSet} is parsed and
     * checked.
     * <p>
     * A keyple::core::selection::AbstractMatchingSe list is build and returned. Non matching SE
     * are signaled by a null element in the list
     *
     * @param defaultSelectionsResponse the selection response
     * @return the keyple::core::selection::SelectionsResult containing the result of all
     *         prepared selection cases, including keyple::core::selection::AbstractMatchingSe
     *         and keyple::core::seproxy::message::SeResponse.
     * @throws KeypleException if the selection process failed
     */
    std::shared_ptr<SelectionsResult> processSelection(
        std::shared_ptr<AbstractDefaultSelectionsResponse>
            defaultSelectionsResponse);
};

}
}
}
