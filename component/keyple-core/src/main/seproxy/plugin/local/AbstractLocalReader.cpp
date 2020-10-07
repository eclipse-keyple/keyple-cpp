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

#include <typeinfo>

#include "AbstractLocalReader.h"

/* Common */
#include "Arrays.h"
#include "IllegalArgumentException.h"
#include "IllegalStateException.h"

/* Core */
#include "AbstractDefaultSelectionsRequest.h"
#include "AbstractObservableLocalReader.h"
#include "ApduRequest.h"
#include "ApduResponse.h"
#include "ByteArrayUtil.h"
#include "DefaultSelectionsRequest.h"
#include "DefaultSelectionsResponse.h"
#include "KeypleReaderException.h"
#include "KeypleReaderIOException.h"
#include "ObservableReader.h"
#include "ReaderEvent.h"
#include "SeRequest.h"
#include "SeProtocol.h"
#include "SeResponse.h"
#include "SeRequest.h"
#include "SeSelector.h"
#include "SmartSelectionReader.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {
namespace local {

using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::util;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::seproxy::message;

using NotificationMode = ObservableReader::NotificationMode;

AbstractLocalReader::AbstractLocalReader(const std::string& pluginName,
                                         const std::string& readerName)
: AbstractReader(pluginName, readerName)
{
    /*
     * Provides an initial value for measuring the inter-exchange time. The
     * first measurement gives the time elapsed since the plugin was loaded.
     */
    mBefore = System::nanoTime();
}

bool AbstractLocalReader::isSePresent()
{
    return checkSePresence();
}

void AbstractLocalReader::setForceGetDataFlag(bool forceGetDataFlag)
{
    mForceGetDataFlag = forceGetDataFlag;
}

std::shared_ptr<SelectionStatus> AbstractLocalReader::openLogicalChannel(
    std::shared_ptr<SeSelector> seSelector)
{
    const std::vector<uint8_t>& atr = getATR();
    bool selectionHasMatched        = true;
    std::shared_ptr<SelectionStatus> selectionStatus;

    /* Perform ATR filtering if requested */
    if (seSelector->getAtrFilter() != nullptr) {
        if (atr.empty()) {
            throw KeypleReaderIOException("Didn't get an ATR from the SE");
        }

        mLogger->trace("[%] openLogicalChannel => ATR = %\n",
                      getName(), atr);

        if (!seSelector->getAtrFilter()->atrMatches(atr)) {
            mLogger->info("[%] openLogicalChannel => ATR didn't match. "
                         "SELECTOR = %, ATR = %\n", getName(), seSelector, atr);

            selectionHasMatched = false;
        }
    }

    /*
     * Perform application selection if requested and if ATR filtering matched
     * or was not requested
     */
    if (selectionHasMatched && seSelector->getAidSelector() != nullptr) {
        std::shared_ptr<ApduResponse> fciResponse;

        SmartSelectionReader* smartSelectionReader =
            dynamic_cast<SmartSelectionReader *>(this);

        if (smartSelectionReader) {
            fciResponse = smartSelectionReader->openChannelForAid(
                *(seSelector->getAidSelector().get()));
        } else {
            fciResponse = processExplicitAidSelection(
                *(seSelector->getAidSelector().get()));
        }

        if (fciResponse->isSuccessful() &&
            fciResponse->getDataOut().size() == 0) {
            /*
             * The selection didn't provide data (e.g. OMAPI), we get the FCI
             * using a Get Data command.
             *
             * The AID selector is provided to handle successful status word in
             * the Get Data command.
             */
            fciResponse =
                recoverSelectionFciData(*(seSelector->getAidSelector().get()));
        }

        /*
         * The ATR filtering matched or was not requested. The selection status
         * is determined by the answer to the select application command.
         */
        selectionStatus = std::make_shared<SelectionStatus>(
            std::make_shared<AnswerToReset>(atr), fciResponse,
            fciResponse->isSuccessful());
    } else {
        /*
         * The ATR filtering didn't match or no AidSelector was provided. The
         * selection status is determined by the ATR filtering.
         */
        std::vector<uint8_t> dummy;
        selectionStatus = std::make_shared<SelectionStatus>(
            std::make_shared<AnswerToReset>(atr),
            std::make_shared<ApduResponse>(dummy, nullptr),
            selectionHasMatched);
    }

    return selectionStatus;
}

std::shared_ptr<SelectionStatus>
AbstractLocalReader::openLogicalChannelAndSelect(
    std::shared_ptr<SeSelector> seSelector)
{

    std::shared_ptr<SelectionStatus> selectionStatus;

    if (seSelector == nullptr) {
        throw IllegalArgumentException(
                  "Try to open logical channel without selector.");
    }

    if (!isLogicalChannelOpen()) {
        /*
         * init of the physical SE channel: if not yet established, opening of a
         * new physical channel
         */
        if (!isPhysicalChannelOpen()) {
            openPhysicalChannel();
        }

        if (!isPhysicalChannelOpen()) {
            throw KeypleReaderIOException(
                "Fail to open physical channel.");
        }
    }

    selectionStatus = openLogicalChannel(seSelector);

    return selectionStatus;
}

bool AbstractLocalReader::isLogicalChannelOpen() const
{
    return mLogicalChannelIsOpen;
}

void AbstractLocalReader::closeLogicalChannel()
{
    mLogger->trace("[%] closeLogicalChannel => Closing of the logical "
                  "channel\n", getName());

    mLogicalChannelIsOpen = false;
    mAidCurrentlySelected.clear();
    mCurrentSelectionStatus.reset();
}

void AbstractLocalReader::addSeProtocolSetting(
    std::shared_ptr<SeProtocol> seProtocol, const std::string& protocolRule)
{
    mLogger->trace("setSeProcotolSetting - adding 1 protocol to map: % : %\n",
                  seProtocol->getName(), protocolRule);

    mProtocolsMap.emplace(seProtocol, protocolRule);
}

void AbstractLocalReader::setSeProtocolSetting(
    const std::map<std::shared_ptr<SeProtocol>, std::string>& protocolSetting)
{
    mLogger->trace("setSeProcotolSetting - adding % protocols to map\n",
                  protocolSetting.size());

    mProtocolsMap.insert(protocolSetting.begin(), protocolSetting.end());
}

std::vector<std::shared_ptr<SeResponse>> AbstractLocalReader::processSeRequests(
    const std::vector<std::shared_ptr<SeRequest>>& seRequests,
    const MultiSeRequestProcessing& multiSeRequestProcessing,
    const ChannelControl& channelControl)
{
    std::vector<bool> requestMatchesProtocol(seRequests.size());
    int requestIndex = 0, lastRequestIndex;

    /* Determine which requests are matching the current ATR */
    mLogger->trace("processSeRequestSet - determining which requests are "
                  "matching the ATR\n");

    for (auto request : seRequests) {
        std::shared_ptr<SeSelector> seSelector = request->getSeSelector();
        if (seSelector != nullptr) {
            requestMatchesProtocol[requestIndex] =
                protocolFlagMatches(request->getSeSelector()->getSeProtocol());
        } else {
            requestMatchesProtocol[requestIndex] = true;
        }

        requestIndex++;
    }

    /*
     * we have now an array of booleans saying whether the corresponding request
     * and the current SE match or not
     */
    lastRequestIndex = requestIndex;
    requestIndex     = 0;

    /*
     * The current request list is possibly made of several APDU command lists.
     *
     * If the requestMatchesProtocol is true we process the SeRequest.
     *
     * If the requestMatchesProtocol is false we skip to the next SeRequest.
     *
     * If keepChannelOpen is false, we close the physical channel for the last
     * request.
     */
    mLogger->debug("processSeRequestSet - processing requests set\n");

    std::vector<std::shared_ptr<SeResponse>> responses;
    bool stopProcess = false;

    for (auto request : seRequests) {
        if (!stopProcess) {
            if (requestMatchesProtocol[requestIndex]) {
                mLogger->debug("[%] processSeRequests => transmit %\n",
                              this->getName(), request);

                std::shared_ptr<SeResponse> response = nullptr;

                try {
                    response = processSeRequestLogical(request);
                } catch (KeypleReaderIOException& ex) {
                    /*
                     * The process has been interrupted. We launch a
                     *  KeypleReaderException with
                     * the responses collected so far.
                     */
                    /*
                     * Add the latest (and partial) SeResponse to the current
                     * list.
                     */
                    responses.push_back(ex.getSeResponse());

                    /* Build a SeResponseSet with the available data. */
                    ex.setSeResponses(responses);
                    mLogger->debug("[%] processSeRequests => transmit : " \
                                   "process interrupted, collect previous " \
                                   "responses %\n", getName(), responses);
                    throw ex;
                }

                responses.push_back(response);
                mLogger->debug("[%] processSeRequests => receive %\n",
                              this->getName(), response);
            } else {
                /*
                 * in case the protocolFlag of a SeRequest doesn't match the
                 * reader status, a null SeResponse is added to the
                 * SeResponseSet.
                 */
                responses.push_back(nullptr);
            }

            if (multiSeRequestProcessing ==
                MultiSeRequestProcessing::PROCESS_ALL) {
                /*
                 * Multi SeRequest case: just close the logical channel and go
                 * on with the next selection.
                 */
                closeLogicalChannel();
            } else {
                if (mLogicalChannelIsOpen) {
                    /*
                     * The current PO matches the selection case, we stop here
                     */
                    stopProcess = true;
                }
            }

            requestIndex++;
            if (lastRequestIndex == requestIndex &&
                channelControl != ChannelControl::KEEP_OPEN) {
                /* Close logical channel unconditionally */
                closeLogicalChannel();

                ObservableReader* observableReader =
                    dynamic_cast<ObservableReader *>(this);

                if (observableReader &&
                    observableReader->countObservers() == 0) {
                    /*
                     * Not observable/observed: close immediately the
                     * physical channel if requested.
                     */
                    closePhysicalChannel();
                }

                AbstractObservableLocalReader* abstractReader =
                    dynamic_cast<AbstractObservableLocalReader *>(this);

                if (abstractReader) {
                    /*
                     * Request the removal sequence when the reader is
                     * monitored by a thread.
                     */
                    abstractReader->startRemovalSequence();
                }
            }
        }
    }

    return responses;
}

std::shared_ptr<SeResponse>
AbstractLocalReader::processSeRequest(
    const std::shared_ptr<SeRequest> seRequest,
    const ChannelControl& channelControl)
{
    std::shared_ptr<SeResponse> seResponse = nullptr;

    /*
     * The SeRequest may be null when we just need to close the physical
     * channel
     */
    if (seRequest)
        seResponse = processSeRequestLogical(seRequest);

    if (channelControl != ChannelControl::KEEP_OPEN) {
        /* Close logical channel unconditionally */
        closeLogicalChannel();

        ObservableReader* observableReader =
                    dynamic_cast<ObservableReader *>(this);

        if (observableReader && observableReader->countObservers() == 0) {
            /*
             * Not observable/observed: close immediately the physical channel
             * if requested.
             */
            closePhysicalChannel();
        }

        AbstractObservableLocalReader* abstractReader =
                    dynamic_cast<AbstractObservableLocalReader *>(this);

        if (abstractReader) {
            /*
             * Request the removal sequence when the reader is
             * monitored by a thread.
             */
            abstractReader->startRemovalSequence();
        }
    }

    return seResponse;
}

std::shared_ptr<SeResponse> AbstractLocalReader::processSeRequestLogical(
    std::shared_ptr<SeRequest> seRequest)
{
    bool previouslyOpen                              = true;
    std::shared_ptr<SelectionStatus> selectionStatus = nullptr;

    std::vector<std::shared_ptr<ApduResponse>> apduResponses;

    mLogger->debug("[%] processSeRequest => Logical channel open = %\n",
                  getName(), isLogicalChannelOpen());

    /*
     * Unless the selector is null, we try to open a logical channel; if the
     * channel was open and the PO is still matching we won't redo the selection
     * and just use the current selection status
     */
    if (seRequest->getSeSelector() != nullptr) {
        /* check if AID changed if the channel is already open */
        if (isLogicalChannelOpen() &&
            seRequest->getSeSelector()->getAidSelector() != nullptr) {
            /*
             * AID comparison hack: we check here if the initial selection AID
             * matches the beginning of the AID provided in the SeRequest
             * (coming from FCI data and supposed to be longer than the
             * selection AID).
             *
             * The current AID (selector) length must be at least equal or
             * greater than the selection AID. All bytes of the selection AID
             * must match the beginning of the current AID.
             */
            if (mAidCurrentlySelected.empty())
                throw IllegalStateException(
                    "AID currently selected shouldn't be null.");

            if (seRequest->getSeSelector()
                    ->getAidSelector()
                    ->getFileOccurrence() ==
                SeSelector::AidSelector::FileOccurrence::NEXT) {
                mLogger->trace("[%] processSeRequest => The current selection"
                              " is a next selection, close the logical "
                              "channel\n", getName());

                /*
                 * Close the channel (will reset the current selection status)
                 */
                closeLogicalChannel();

            } else if (!startsWith(mAidCurrentlySelected,
                                   seRequest->getSeSelector()
                                       ->getAidSelector()
                                           ->getAidToSelect())) {
                /*
                 * The AID changed (longer or different), close the logical
                 * channel.
                 */
                mLogger->debug(
                    "[%] processSeRequest => The AID changed, "
                    "close the logical channel. AID = %, "
                    "EXPECTEDAID = %", getName(), mAidCurrentlySelected,
					seRequest->getSeSelector());

                /*
                 * Close the channel (will reset the current selection status)
                 */
                closeLogicalChannel();
            }

            /*
             * Keep the current selection status (may be null if the current PO
             * didn't match)
             */
            selectionStatus = mCurrentSelectionStatus;
        }

        /* open the channel and do the selection if needed */
        if (!isLogicalChannelOpen()) {
            previouslyOpen = false;

            try {
                selectionStatus =
                    openLogicalChannelAndSelect(seRequest->getSeSelector());
                mLogger->trace("[%] processSeRequest => Logical channel "
                              "opening success\n", getName());
            } catch (const IllegalArgumentException& e) {
                (void)e;
                mLogger->debug("[%] processSeRequest => Logical channel "
                              "opening failure\n", getName());
                closeLogicalChannel();

                /*
                 * Return a null SeResponse when the opening of the logical
                 * channel failed
                 */
                return nullptr;
            }

            if (selectionStatus->hasMatched()) {
                /*
                 * The selection process succeeded, the logical channel is open
                 */
                mLogicalChannelIsOpen = true;

                if (selectionStatus->getFci()->isSuccessful()) {
                    /* The selection AID based was successful, keep the aid */
                    mAidCurrentlySelected = seRequest->getSeSelector()
                                               ->getAidSelector()
                                               ->getAidToSelect();
                }

                mCurrentSelectionStatus = selectionStatus;
            } else {
                /* The selection process failed, close the logical channel */
                closeLogicalChannel();
            }
        }
    } else {
        /*
         * Selector is null, we expect that the logical channel was previously
         * opened
         */
        if (!isLogicalChannelOpen())
            throw IllegalStateException(
                "[" + getName() + "]" +
                "processSeRequest => No logical channel opened!");
    }

    /* process request if not empty */
    if (seRequest->getApduRequests().size() > 0) {
        for (auto apduRequest : seRequest->getApduRequests()) {
            try {
                apduResponses.push_back(processApduRequest(apduRequest));
            } catch (KeypleReaderIOException& ex) {
                /*
                 * The process has been interrupted. We close the logical
                 * channel and launch a KeypleReaderException with the Apdu
                 * responses collected so far.
                 */
                mLogger->debug("The process has been interrupted, collect Apdu"
                              "responses collected so far\n");
                closeLogicalAndPhysicalChannels();
                ex.setSeResponse(std::make_shared<SeResponse>(
                    false, previouslyOpen, selectionStatus, apduResponses));
                throw ex;
            }
        }
    }

    return std::make_shared<SeResponse>(isLogicalChannelOpen(), previouslyOpen,
                                        selectionStatus, apduResponses);
}

bool AbstractLocalReader::startsWith(const std::vector<uint8_t>& source,
                                     const std::vector<uint8_t>& match)
{
    if (match.size() > source.size())
        return false;

    for (int i = 0; i < static_cast<int>(match.size()); i++) {
        if (source[i] != match[i])
            return false;
    }

    return true;
}

std::shared_ptr<ApduResponse> AbstractLocalReader::processApduRequest(
    std::shared_ptr<ApduRequest> apduRequest)
{
    std::shared_ptr<ApduResponse> apduResponse;
    long long timeStamp = System::nanoTime();
    long long elapsed10ms = (timeStamp - mBefore) / 100000;
    mBefore     = timeStamp;

    mLogger->debug("[%] processApduRequest => %, elapsed % ms\n", getName(),
		          apduRequest, elapsed10ms / 10);

    const std::vector<uint8_t>& buffer = apduRequest->getBytes();
    std::vector<uint8_t> rapdu;
    try {
        rapdu  = transmitApdu(buffer);
    } catch (const KeypleReaderIOException& e) {
        throw e;
    }

    apduResponse = std::make_shared<ApduResponse>(
                       rapdu, apduRequest->getSuccessfulStatusCodes());

    if (apduRequest->isCase4() && apduResponse->getDataOut().empty() &&
        apduResponse->isSuccessful()) {
        /* Do the get response command but keep the original status code */
        apduResponse = case4HackGetResponse(apduResponse->getStatusCode());
    }

    timeStamp    = System::nanoTime();
    elapsed10ms = (timeStamp - mBefore) / 100000;
    mBefore = timeStamp;

    mLogger->debug("[%] processApduRequest => %, elapsed % ms\n",
                  getName(), apduResponse, elapsed10ms / 10);

    return apduResponse;
}

std::shared_ptr<ApduResponse>
AbstractLocalReader::case4HackGetResponse(int originalStatusCode)
{
    mLogger->debug("case4HackGetResponse\n");

    /*
     * Build a get response command the actual length expected by the SE in the
     * get response command is handled in transmitApdu
     */
    long long timeStamp = System::nanoTime();
    long long elapsed10ms = (timeStamp - mBefore) / 100000;
    mBefore = timeStamp;

    mLogger->debug("[%] case4HackGetResponse => ApduRequest: NAME = "
                  "\"Internal Get Response\", RAWDATA = %, elapsed = %\n",
                  getName(), mGetResponseHackRequestBytes, elapsed10ms / 10.0);

    std::vector<uint8_t> getResponseHackResponseBytes =
        transmitApdu(mGetResponseHackRequestBytes);

    /* We expect here a 0x9000 status code */
    std::shared_ptr<ApduResponse> getResponseHackResponse =
        std::make_shared<ApduResponse>(getResponseHackResponseBytes, nullptr);

    timeStamp    = System::nanoTime();
    elapsed10ms = (timeStamp - mBefore) / 100000;
    mBefore = timeStamp;
    mLogger->debug("[%] case4HackGetResponse => Internal %, elapsed % ms\n",
                  getName(), mGetResponseHackRequestBytes, elapsed10ms / 10.0);

    if (getResponseHackResponse->isSuccessful()) {
        /* Replace the two last status word bytes by the original status word */
        getResponseHackResponseBytes[getResponseHackResponseBytes.size() - 2] =
            static_cast<uint8_t>((originalStatusCode & 0xff00) >> 8);
        getResponseHackResponseBytes[getResponseHackResponseBytes.size() - 1] =
            static_cast<uint8_t>(originalStatusCode & 0x00ff);
    }

    return getResponseHackResponse;
}

void AbstractLocalReader::closeLogicalAndPhysicalChannels()
{
    closeLogicalChannel();

    try {
        closePhysicalChannel();
    } catch (const KeypleReaderIOException& e) {
        mLogger->debug("[%] Exception occurred in "
                      "closeLogicalAndPhysicalChannels. Message: %\n",
                      getName(), e.getMessage());
    }
}

std::shared_ptr<ApduResponse> AbstractLocalReader::processExplicitAidSelection(
    SeSelector::AidSelector& aidSelector)
{
    std::shared_ptr<ApduResponse> fciResponse;
    const std::vector<uint8_t> aid = aidSelector.getAidToSelect();

    if (aid.empty()) {
        throw IllegalArgumentException(
            "AID must not be null for an AidSelector");
    }

    mLogger->debug("[%] openLogicalChannel => Select Application with AID = %\n",
		          getName(), aid);

    /*
     * build a get response command the actual length expected by the SE in the
     * get response command is handled in transmitApdu
     */
    std::vector<uint8_t> selectApplicationCommand;
    selectApplicationCommand.push_back(0x00); // CLA
    selectApplicationCommand.push_back(0xA4); // INS
    selectApplicationCommand.push_back(0x04); // P1: select by name
    /*
     * P2: b0,b1 define the File occurrence, b2,b3 define the File control
     * information. We use the bitmask defined in the respective enums.
     */
    selectApplicationCommand.push_back(
        aidSelector.getFileOccurrence().getIsoBitMask() |
        aidSelector.getFileControlInformation().getIsoBitMask());
    selectApplicationCommand.push_back((uint8_t)(aid.size()));     // Lc
    selectApplicationCommand.insert(selectApplicationCommand.end(), aid.begin(),
                                    aid.end()); // data
    selectApplicationCommand.push_back(0x00);                    // Le

    /*
     * we use here processApduRequest to manage case 4 hack. The successful
     * status codes list for this command is provided.
     */
    fciResponse = processApduRequest(std::make_shared<ApduRequest>(
        "Internal Select Application", selectApplicationCommand, true,
        aidSelector.getSuccessfulSelectionStatusCodes()));

    if (!fciResponse->isSuccessful()) {
        mLogger->debug("[%] openLogicalChannel => Application Selection "
                      "failed. SELECTOR = %\n", getName(), aidSelector);
    }

    return fciResponse;
}

std::shared_ptr<ApduResponse> AbstractLocalReader::recoverSelectionFciData(
    SeSelector::AidSelector& aidSelector)
{
    std::shared_ptr<ApduResponse> fciResponse;

    /*
     * Get Data APDU: CLA, INS, P1: always 0, P2: 0x6F FCI for the current DF,
     * LC: 0
     */
    std::vector<uint8_t> getDataCommand = {0x00, 0xCA, 0x00, 0x6F, 0x00};

    /* The successful status codes list for this command is provided */
    fciResponse = processApduRequest(std::make_shared<ApduRequest>(
        "Internal Get Data", getDataCommand, false,
        aidSelector.getSuccessfulSelectionStatusCodes()));

    if (!fciResponse->isSuccessful()) {
        mLogger->debug("[%] selectionGetData => Get data failed. SELECTOR = "
                      "%\n", getName(), aidSelector);
    }

    return fciResponse;
}

}
}
}
}
}
