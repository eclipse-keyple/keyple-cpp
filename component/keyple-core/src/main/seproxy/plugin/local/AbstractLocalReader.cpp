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

#include <typeinfo>

#include "AbstractDefaultSelectionsRequest.h"
#include "AbstractObservableLocalReader.h"
#include "AbstractLocalReader.h"
#include "ApduRequest.h"
#include "ApduResponse.h"
#include "Arrays.h"
#include "ByteArrayUtil.h"
#include "DefaultSelectionsRequest.h"
#include "DefaultSelectionsResponse.h"
#include "KeypleChannelControlException.h"
#include "KeypleReaderException.h"
#include "KeypleIOReaderException.h"
#include "KeypleApplicationSelectionException.h"
#include "ObservableReader_Import.h"
#include "ReaderEvent_Import.h"
#include "SeRequest.h"
#include "SeProtocol_Import.h"
#include "SeResponse.h"
#include "SeRequest.h"
#include "SeSelector_Import.h"
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

AbstractLocalReader::AbstractLocalReader(
  const std::string& pluginName, const std::string& readerName)
: AbstractReader(pluginName, readerName)
{
    /*
     * Provides an initial value for measuring the inter-exchange time. The
     * first measurement gives the time elapsed since the plugin was loaded.
     */
    this->before = System::nanoTime();
}

bool AbstractLocalReader::isSePresent()
{
    return checkSePresence();
}

#if 0
void AbstractLocalReader::cardInserted()
{
    logger->debug("card inserted notification\n");

    if (defaultSelectionsRequest == nullptr) {
        logger->debug("defaultSelectionRequest is null\n");

        /* no default request is defined, just notify the SE insertion */
        AbstractLoggedObservable::notifyObservers(
            std::make_shared<ReaderEvent>(this->pluginName, this->name,
                                          ReaderEvent::EventType::SE_INSERTED,
                                          nullptr));
        presenceNotified = true;
    } else {
        logger->debug("defaultSelectionRequest is not null\n");

        /*
         * A default request is defined, send it a notify according to the
         * notification mode and the selection status
         */
        bool aSeMatched = false;
        try {
            std::shared_ptr<SeResponseSet> seResponseSet =
                processSeRequestSet(
                    defaultSelectionsRequest->getSelectionSeRequestSet());

            for (auto seResponse : seResponseSet->getResponses()) {
                if (seResponse != nullptr &&
                    seResponse->getSelectionStatus()->hasMatched()) {
                    aSeMatched = true;
                    break;
                }
            }

            if (notificationMode == NotificationMode::MATCHED_ONLY) {
                /*
                 * Notify only if a SE matched the selection, just ignore if
                 * not
                 */
                if (aSeMatched) {
                    AbstractLoggedObservable::notifyObservers(
                        std::make_shared<ReaderEvent>(
                            this->pluginName,
                            this->name,ReaderEvent::EventType::SE_MATCHED,
                            std::make_shared<DefaultSelectionsResponse>(
                                seResponseSet)));
                    presenceNotified = true;
                }
                else {
                    /* the SE did not match, close the logical channel */
                    closeLogicalChannel();
                }
            } else {
                if (aSeMatched) {
                    /*
                     * The SE matched, notify an SE_MATCHED event with the
                     * received response
                     */
                    AbstractLoggedObservable::notifyObservers(
                        std::make_shared<ReaderEvent>(
                            this->pluginName,
                            this->name, ReaderEvent::EventType::SE_MATCHED,
                            std::make_shared<DefaultSelectionsResponse>(
                                seResponseSet)));
                } else {
                    /*
                     * The SE didn't match, notify an SE_INSERTED event with the
                     * received response
                     */
                    AbstractLoggedObservable::notifyObservers(
                        std::make_shared<ReaderEvent>(
                            this->pluginName,
                            this->name, ReaderEvent::EventType::SE_INSERTED,
                            std::make_shared<DefaultSelectionsResponse>(
                                seResponseSet)));
                }
                presenceNotified = true;
            }
        }
        catch (const KeypleReaderException &e) {
            /* The last transmission failed, close the logical channel */
            closeLogicalChannel();
            logger->error("cardInserted - caught KeypleReaderException " \
                          "(message: %s, cause: %s)\n", e.getMessage().c_str(),
                          e.getCause().what());


            /*
             * In this case the card has been removed or not read correctly, do
             * not throw event.
             */
        }
    }
}

void AbstractLocalReader::cardRemoved()
{
    logger->debug("cardRemoved - \n");

    if (presenceNotified) {
        AbstractLoggedObservable::notifyObservers(std::make_shared<ReaderEvent>(
            this->pluginName, this->name, ReaderEvent::EventType::SE_REMOVAL,
            nullptr));
        presenceNotified = false;
    }

    closeLogicalChannel();

    try {
        closePhysicalChannel();
    }
    catch (const KeypleChannelStateException &e) {
        logger->trace("[%s] Exception occured in waitForCardAbsent. Message:" \
                      "%s",
                      AbstractLoggedObservable<ReaderEvent>::getName().c_str(),
                      e.what());
        /* No stack trace throwable? */
    }
}
#endif

void AbstractLocalReader::setForceGetDataFlag(bool forceGetDataFlag)
{
    this->forceGetDataFlag = forceGetDataFlag;
}

std::shared_ptr<SelectionStatus> AbstractLocalReader::openLogicalChannel(
    std::shared_ptr<SeSelector> seSelector)
{
    const std::vector<uint8_t>& atr = getATR();
    bool selectionHasMatched = true;
    std::shared_ptr<SelectionStatus> selectionStatus;

    /** Perform ATR filtering if requested */
    if (seSelector->getAtrFilter() != nullptr) {
        if (atr.empty()) {
            throw KeypleIOReaderException("Didn't get an ATR from the SE");
        }

        logger->debug("[%s] openLogicalChannel => ATR = %s\n", this->getName(),
                      ByteArrayUtil::toHex(atr));

        if (!seSelector->getAtrFilter()->atrMatches(atr)) {
            logger->info("[%s] openLogicalChannel => ATR didn't match. " \
                         "SELECTOR = %s, ATR = %s", this->getName(),
                         seSelector->toString(),
                         ByteArrayUtil::toHex(atr));

            selectionHasMatched = false;
        }
    }

    /**
     * Perform application selection if requested and if ATR filtering matched
     * or was not requested
     */
    if (selectionHasMatched && seSelector->getAidSelector() != nullptr) {
        std::shared_ptr<ApduResponse> fciResponse;

        std::shared_ptr<SmartSelectionReader> smartSelectionReader =
            std::dynamic_pointer_cast<SmartSelectionReader>(shared_from_this());

        if (smartSelectionReader) {
            fciResponse =
                smartSelectionReader->openChannelForAid(
                    *(seSelector->getAidSelector().get()));
        } else {
            fciResponse = processExplicitAidSelection(
                              *(seSelector->getAidSelector().get()));
        }

        if (fciResponse->isSuccessful() &&
            fciResponse->getDataOut().size() == 0) {
            /**
             * The selection didn't provide data (e.g. OMAPI), we get the FCI
             * using a Get Data command.
             * <p>
             * The AID selector is provided to handle successful status word in
             * the Get Data command.
             */
            fciResponse = recoverSelectionFciData(
                              *(seSelector->getAidSelector().get()));
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
        throw KeypleChannelControlException(
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
            throw KeypleChannelControlException(
                      "Fail to open physical channel.");
        }
    }

    selectionStatus = openLogicalChannel(seSelector);

    return selectionStatus;
}

bool AbstractLocalReader::isLogicalChannelOpen()
{
    return logicalChannelIsOpen;
}

void AbstractLocalReader::closeLogicalChannel()
{
    logger->trace("[%s] closeLogicalChannel => Closing of the logical " \
                  "channel\n", this->getName());

    logicalChannelIsOpen = false;
    aidCurrentlySelected.reset();
    currentSelectionStatus.reset();
}

void AbstractLocalReader::addSeProtocolSetting(SeProtocol& seProtocol,
                                               const std::string &protocolRule)
{
    logger->debug("setSeProcotolSetting - adding 1 protocol to map: %s-%s\n",
                  seProtocol.getName().c_str(), protocolRule.c_str());

    this->protocolsMap.emplace(seProtocol, protocolRule);
}

void AbstractLocalReader::setSeProtocolSetting(
    const std::map<SeProtocol, std::string> &protocolSetting)
{
    logger->debug("setSeProcotolSetting - adding %d protocols to map\n",
                  protocolSetting.size());

    this->protocolsMap.insert(protocolSetting.begin(), protocolSetting.end());
}

std::list<std::shared_ptr<SeResponse>> AbstractLocalReader::processSeRequestSet(
    std::set<std::shared_ptr<SeRequest>>& requestSet,
    MultiSeRequestProcessing multiSeRequestProcessing,
    ChannelControl channelControl)
{
    std::vector<bool> requestMatchesProtocol(requestSet.size());
    int requestIndex = 0, lastRequestIndex;

    /* Determine which requests are matching the current ATR */
    logger->debug("processSeRequestSet - determining which requests are " \
                  "matching the ATR\n");

    for (auto request : requestSet) {
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
    requestIndex = 0;

    /*
     * The current requestSet is possibly made of several APDU command lists.
     *
     * If the requestMatchesProtocol is true we process the requestSet.
     *
     * If the requestMatchesProtocol is false we skip to the next requestSet.
     *
     * If keepChannelOpen is false, we close the physical channel for the last
     * request.
     */
    logger->debug("processSeRequestSet - processing requests set\n");

    std::list<std::shared_ptr<SeResponse>> responses;
    bool stopProcess = false;

    for (auto request : requestSet) {
        logger->debug("processSeRequestSet - stopProcess ? %d\n", stopProcess);
        if (!stopProcess) {
            if (requestMatchesProtocol[requestIndex]) {
                logger->debug(
                    "[%s] processSeRequestSet => transmit %s\n",
                    this->getName(), request->toString());

                std::shared_ptr<SeResponse> response = nullptr;

                try {
                    response = processSeRequestLogical(request);
                } catch (KeypleReaderException &ex) {
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
                    ex.setSeResponseSet(responses);
                    logger->debug(
                       "[%s] processSeRequestSet => transmit : process " \
                       "interrupted, collect previous responses %s\n",
                       this->getName(), "<to print>responses");
                    throw ex;
                }

                responses.push_back(response);
                logger->debug("[%s] processSeRequestSet => receive %s\n",
                              this->getName(), response->toString());
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
                if (logicalChannelIsOpen) {
                    /*
                     * The current PO matches the selection case, we stop here
                     */
                    stopProcess = true;
                }
            }

            requestIndex++;
            if (lastRequestIndex == requestIndex &&
                channelControl !=  ChannelControl::KEEP_OPEN) {
                /* Close logical channel unconditionally */
                closeLogicalChannel();

                if (this->countObservers() == 0) {
                    /*
                        * Not observable/observed: close immediately the
                        * physical channel if requested.
                        */
                    closePhysicalChannel();
                }

                std::shared_ptr<AbstractObservableLocalReader> abstractR =
                    std::dynamic_pointer_cast<AbstractObservableLocalReader>
                        (shared_from_this());

                if (abstractR) {
                    /*
                        * Request the removal sequence when the reader is
                        * monitored by a thread.
                        */
                    abstractR->startRemovalSequence();
                }
            }
        }
    }
    return responses;
}

std::shared_ptr<SeResponse>
AbstractLocalReader::processSeRequest(std::shared_ptr<SeRequest> seRequest,
    ChannelControl channelControl)
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

        if (this->countObservers() == 0) {
            /*
             * Not observable/observed: close immediately the physical channel
             * if requested.
             */
            closePhysicalChannel();
        }

        std::shared_ptr<AbstractObservableLocalReader> abstractR =
            std::dynamic_pointer_cast<AbstractObservableLocalReader>
                (shared_from_this());

        if (abstractR) {
            /*
             * Request the removal sequence when the reader is
             * monitored by a thread.
             */
            abstractR->startRemovalSequence();
        }
    }

    return seResponse;
}

std::shared_ptr<SeResponse> AbstractLocalReader::processSeRequestLogical(
    std::shared_ptr<SeRequest> seRequest)
{
    bool previouslyOpen = true;
    std::shared_ptr<SelectionStatus> selectionStatus = nullptr;

    std::vector<std::shared_ptr<ApduResponse>> apduResponseList;

    logger->debug("[%s] processSeRequest => Logical channel open = %d\n",
                  this->getName(), isLogicalChannelOpen());

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
            if (aidCurrentlySelected == nullptr)
                throw IllegalStateException(
                          "AID currently selected shouldn't be null.");

            if (seRequest->getSeSelector()->getAidSelector()
                    ->getFileOccurrence() ==
                        SeSelector::AidSelector::FileOccurrence::NEXT) {
                logger->trace("[%s] processSeRequest => The current selection" \
                              " is a next selection, close the logical " \
                              "channel\n", this->getName());

                /*
                 * Close the channel (will reset the current selection status)
                 */
                closeLogicalChannel();

            } else if (!aidCurrentlySelected->startsWith(seRequest
                           ->getSeSelector()->getAidSelector()
                           ->getAidToSelect())) {
                /*
                 * The AID changed (longer or different), close the logical
                 * channel.
                 */
                logger->debug("[%s] processSeRequest => The AID changed, " \
                              "close the logical channel. AID = %s, " \
                              "EXPECTEDAID = %s", this->getName(),
                               ByteArrayUtil::toHex(
                                   aidCurrentlySelected->getValue()).c_str(),
                                   seRequest->getSeSelector()->toString());

                /*
                 * Close the channel (will reset the current selection status)
                 */
                closeLogicalChannel();
            }

            /*
             * Keep the current selection status (may be null if the current PO
             * didn't match)
             */
            selectionStatus = currentSelectionStatus;
        }

        /* open the channel and do the selection if needed */
        if (!isLogicalChannelOpen()) {
            previouslyOpen = false;

            try {
                selectionStatus =
                    openLogicalChannelAndSelect(seRequest->getSeSelector());
                logger->trace("[%s] processSeRequest => Logical channel " \
                              "opening success\n", this->getName());
            } catch (const KeypleApplicationSelectionException &e) {
                (void)e;
                logger->debug("[%s] processSeRequest => Logical channel " \
                              "opening failure\n", this->getName());
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
                logicalChannelIsOpen = true;

                if (selectionStatus->getFci()->isSuccessful()) {
                    /* The selection AID based was successful, keep the aid */
                    aidCurrentlySelected =
                        seRequest->getSeSelector()->getAidSelector()
                            ->getAidToSelect();
                }

                currentSelectionStatus = selectionStatus;
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
        if (!isLogicalChannelOpen()) {
            throw IllegalStateException(
                      StringHelper::formatSimple(
                          "[%s] processSeRequest => No logical channel opened!",
                          this->getName()));
        }
    }

    /* process request if not empty */
    if (seRequest->getApduRequests().size() > 0) {
        for (auto apduRequest : seRequest->getApduRequests()) {
            try {
                apduResponseList.push_back(processApduRequest(apduRequest));
            }
            catch (KeypleIOReaderException &ex) {
                /*
                 * The process has been interrupted. We close the logical
                 * channel and launch a KeypleReaderException with the Apdu
                 * responses collected so far.
                 */
                logger->debug("The process has been interrupted, collect Apdu" \
                              "responses collected so far\n");
                closeLogicalAndPhysicalChannels();
                ex.setSeResponse(
                    std::make_shared<SeResponse>(false, previouslyOpen,
                                                 selectionStatus,
                                                 apduResponseList));
                throw ex;
            }
        }
    }

    return std::make_shared<SeResponse>(isLogicalChannelOpen(), previouslyOpen,
                                        selectionStatus, apduResponseList);
}

std::shared_ptr<ApduResponse> AbstractLocalReader::processApduRequest(
    std::shared_ptr<ApduRequest> apduRequest)
{
    std::shared_ptr<ApduResponse> apduResponse;
    long long timeStamp = System::nanoTime();
    double elapsedMs = static_cast<double>((timeStamp - before) / 100000) / 10;
    this->before = timeStamp;
    logger->debug("[%s] processApduRequest => %s, elapsed %d ms\n",
                  this->getName(), apduRequest->toString(), elapsedMs);


    std::vector<uint8_t> buffer = apduRequest->getBytes();
    std::vector<uint8_t> rapdu = transmitApdu(buffer);
    apduResponse = std::make_shared<ApduResponse>(
                       rapdu, apduRequest->getSuccessfulStatusCodes());

    if (apduRequest->isCase4()             &&
        apduResponse->getDataOut().empty() &&
        apduResponse->isSuccessful()) {
        /* Do the get response command but keep the original status code */
        apduResponse = case4HackGetResponse(apduResponse->getStatusCode());
    }

    timeStamp = System::nanoTime();
    elapsedMs = static_cast<double>((timeStamp - before) / 100000) / 10;
    this->before = timeStamp;
    logger->debug("[%s] processApduRequest => %s, elapsed %d ms\n",
                    this->getName(), apduResponse->toString(), elapsedMs);

    return apduResponse;
}

std::shared_ptr<ApduResponse>
AbstractLocalReader::case4HackGetResponse(int originalStatusCode)
{
    logger->debug("case4HackGetResponse\n");

    std::vector<uint8_t> getResponseHackRequestBytes =
        ByteArrayUtil::fromHex("00C0000000");

    /*
     * Build a get response command the actual length expected by the SE in the
     * get response command is handled in transmitApdu
     */
    long long timeStamp = System::nanoTime();
    double elapsedMs = static_cast<double>((timeStamp - before) / 100000) / 10;
    this->before = timeStamp;

   logger->debug("[%s] case4HackGetResponse => ApduRequest: NAME = " \
                 "\"Internal Get Response\", RAWDATA = %s, elapsed = %s\n",
                 this->getName(),
                 ByteArrayUtil::toHex(getResponseHackRequestBytes).c_str(),
                 elapsedMs);

    std::vector<uint8_t> getResponseHackResponseBytes =
        transmitApdu(getResponseHackRequestBytes);

    /* We expect here a 0x9000 status code */
    std::shared_ptr<ApduResponse> getResponseHackResponse =
        std::make_shared<ApduResponse>(getResponseHackResponseBytes, nullptr);

    timeStamp = System::nanoTime();
    elapsedMs = static_cast<double>((timeStamp - before) / 100000) / 10;
    this->before = timeStamp;
    logger->debug("[%s] case4HackGetResponse => Internal %s, elapsed %s ms\n",
                  this->getName(), "<to print>getResponseHackResponseBytes",
                  elapsedMs);

    if (getResponseHackResponse->isSuccessful()) {
        /* Replace the two last status word bytes by the original status word */
        getResponseHackResponseBytes[getResponseHackResponseBytes.size() - 2] =
            originalStatusCode >> 8;
        getResponseHackResponseBytes[getResponseHackResponseBytes.size() - 1] =
            originalStatusCode & 0xFF;
    }

    return getResponseHackResponse;
}

void AbstractLocalReader::closeLogicalAndPhysicalChannels()
{
    closeLogicalChannel();

    try {
        closePhysicalChannel();
    } catch (KeypleChannelControlException &e) {
        logger->debug("[%s] Exception occurred in " \
                      "closeLogicalAndPhysicalChannels. Message: %s\n",
                      this->getName(), e.getMessage());
    }
}

std::shared_ptr<ApduResponse> AbstractLocalReader::processExplicitAidSelection(
    SeSelector::AidSelector& aidSelector)
{
    std::shared_ptr<ApduResponse> fciResponse;
    const std::vector<uint8_t> aid = aidSelector.getAidToSelect()->getValue();

    if (aid.empty()) {
        throw IllegalArgumentException(
                  "AID must not be null for an AidSelector");
    }

    logger->debug("[%s] openLogicalChannel => Select Application with AID = " \
                  "%s\n", this->getName(), ByteArrayUtil::toHex(aid));

    /*
     * build a get response command the actual length expected by the SE in the
     * get response command is handled in transmitApdu
     */
    std::vector<uint8_t> selectApplicationCommand;
    selectApplicationCommand.reserve(6 + aid.size());
    selectApplicationCommand[0] = 0x00; // CLA
    selectApplicationCommand[1] = 0xA4; // INS
    selectApplicationCommand[2] = 0x04; // P1: select by name
    /*
     * P2: b0,b1 define the File occurrence, b2,b3 define the File control
     * information. We use the bitmask defined in the respective enums.
     */
    selectApplicationCommand[3] =
        aidSelector.getFileOccurrence().getIsoBitMask() |
        aidSelector.getFileControlInformation().getIsoBitMask();
    selectApplicationCommand[4] = aid.size(); // Lc
    System::arraycopy(aid, 0, selectApplicationCommand, 5, aid.size()); // data
    selectApplicationCommand[5 + aid.size()] = 0x00; // Le

    /*
     * we use here processApduRequest to manage case 4 hack. The successful
     * status codes list for this command is provided.
     */
    fciResponse = processApduRequest(
                      std::make_shared<ApduRequest>(
                          "Internal Select Application",
                          selectApplicationCommand, true,
                          aidSelector.getSuccessfulSelectionStatusCodes()));

    if (!fciResponse->isSuccessful()) {
        logger->debug("[%s] openLogicalChannel => Application Selection " \
                      "failed. SELECTOR = %s\n", this->getName(),
                      aidSelector.toString());
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
    fciResponse = processApduRequest(
                      std::make_shared<ApduRequest>(
                          "Internal Get Data", getDataCommand, false,
                          aidSelector.getSuccessfulSelectionStatusCodes()));

    if (!fciResponse->isSuccessful()) {
        logger->debug("[%s] selectionGetData => Get data failed. SELECTOR = " \
                      "%s", this->getName(), aidSelector.toString());
    }

    return fciResponse;
}

}
}
}
}
}
