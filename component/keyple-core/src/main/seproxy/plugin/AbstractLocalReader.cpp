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
#include "AbstractLocalReader.h"
#include "ApduRequest.h"
#include "ApduResponse.h"
#include "Arrays.h"
#include "ByteArrayUtil.h"
#include "DefaultSelectionsRequest.h"
#include "DefaultSelectionsResponse.h"
#include "KeypleReaderException.h"
#include "KeypleIOReaderException.h"
#include "KeypleApplicationSelectionException.h"
#include "ReaderEvent_Import.h"
#include "SeRequest.h"
#include "SeProtocol_Import.h"
#include "SeResponse.h"
#include "SeResponseSet.h"
#include "SeRequestSet.h"
#include "SeRequest.h"
#include "SeSelector_Import.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::util;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::seproxy::message;

using NotificationMode = ObservableReader::NotificationMode;

AbstractLocalReader::AbstractLocalReader(
  const std::string &pluginName, const std::string &readerName)
: AbstractObservableReader(pluginName, readerName)
{
    /*
     * Provides an initial value for measuring the inter-exchange time. The
     * first measurement gives the time elapsed since the plugin was loaded.
     */
    this->before = System::nanoTime(); 
}

bool AbstractLocalReader::isSePresent() 
{
    logger->debug("isSePresent\n");

    if (checkSePresence()) {
        logger->debug("isSePresent - SE present\n");
        return true;
    }
    else {
        if (isLogicalChannelOpen() || isPhysicalChannelOpen()) {
            cardRemoved();
        }
        logger->debug("isSePresent - SE not present\n");
        return false;
    }
}


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
                          "(message: %s, cause: %s)\n", e.getMessage(),
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
                      "%s", AbstractLoggedObservable<ReaderEvent>::getName(),
                      e.what());
        /* No stack trace throwable? */
    }
}

std::shared_ptr<ApduResponse> AbstractLocalReader::openChannelForAidHackGetData(
    std::shared_ptr<SeSelector::AidSelector> aidSelector)
{
    std::shared_ptr<SeSelector::AidSelector> noResponseAidSelector =
        std::make_shared<SeSelector::AidSelector>(
            aidSelector->getAidToSelect(),
            aidSelector->getSuccessfulSelectionStatusCodes(),
            aidSelector->getFileOccurrence(),
            SeSelector::AidSelector::FileControlInformation::NO_RESPONSE);

    std::shared_ptr<ApduResponse> fciResponse =
        openChannelForAid(noResponseAidSelector);

    if (fciResponse->isSuccessful()) {
        std::vector<char> getDataCommand(4);
        getDataCommand[0] = static_cast<char>(0x00); // CLA
        getDataCommand[1] = static_cast<char>(0xCA); // INS
        getDataCommand[2] = static_cast<char>(0x00); // P1: always 0
        getDataCommand[3] = static_cast<char>(0x6F); // P2: 0x6F FCI for the
                                                     // current DF

        /* The successful status codes list for this command is provided */
        fciResponse = processApduRequest(std::make_shared<ApduRequest>(
            "Internal Get Data", getDataCommand, false,
            aidSelector->getSuccessfulSelectionStatusCodes()));
        if (!fciResponse->isSuccessful()) {
            logger->trace("[%s] openChannelForAidHackGetData => Get data " \
                          "failed. SELECTOR = %s\n", this->getName(),
                          aidSelector->toString());
        }
    }

    return fciResponse;
}

void AbstractLocalReader::setForceGetDataFlag(bool forceGetDataFlag)
{
    this->forceGetDataFlag = forceGetDataFlag;
}

std::shared_ptr<SelectionStatus> AbstractLocalReader::openLogicalChannel(
    std::shared_ptr<SeSelector> seSelector)
{
    std::vector<char> atr = getATR();
    bool selectionHasMatched = true;
    std::shared_ptr<SelectionStatus> selectionStatus;

    /** Perform ATR filtering if requested */
    if (seSelector->getAtrFilter() != nullptr) {
        if (atr.empty()) {
            throw std::make_shared<KeypleIOReaderException>("Didn't get an " \
                                                            "ATR from the SE.");
        }

        if (logger->isTraceEnabled()) {
            logger->trace("[%s] openLogicalChannel => ATR = %s\n",
                          this->getName(), ByteArrayUtil::toHex(atr));
        }
        if (!seSelector->getAtrFilter()->atrMatches(atr)) {
            logger->info("[%s] openLogicalChannel => ATR didn't match. " \
                         "SELECTOR = %s, ATR = %s\n", this->getName(),
                         seSelector->toString(), ByteArrayUtil::toHex(atr));
            selectionHasMatched = false;
        }
    }

    /*
     * Perform application selection if requested and if ATR filtering matched
     * or was not requested
     */
    if (selectionHasMatched && seSelector->getAidSelector() != nullptr) {
        std::shared_ptr<ApduResponse> fciResponse;
        if (!forceGetDataFlag) {
            fciResponse = openChannelForAid(seSelector->getAidSelector());
        } else {
            fciResponse =
                openChannelForAidHackGetData(seSelector->getAidSelector());
        }

        /*
         * The ATR filtering matched or was not requested. The selection status
         * is determined by the answer to the select application command.
         */
        selectionStatus = std::make_shared<SelectionStatus>(
                              std::make_shared<AnswerToReset>(atr),
                              fciResponse, fciResponse->isSuccessful());
    } else {
        /*
         * The ATR filtering didn't match or no AidSelector was provided. The selection status
         * is determined by the ATR filtering.
         */
        std::vector<char> empty;
        selectionStatus = std::make_shared<SelectionStatus>(
                              std::make_shared<AnswerToReset>(atr),
                              std::make_shared<ApduResponse>(empty, nullptr),
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
        throw std::make_shared<KeypleChannelStateException>(
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
            throw std::make_shared<KeypleChannelStateException>(
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
                  "channel\n",
                  AbstractLoggedObservable<ReaderEvent>::getName());

    logicalChannelIsOpen = false;
    aidCurrentlySelected.reset();
    currentSelectionStatus.reset();
}

void AbstractLocalReader::addSeProtocolSetting(SeProtocol& seProtocol,
                                               const std::string &protocolRule)
{
    logger->debug("setSeProcotolSetting - adding 1 protocol to map: %s-%s\n",
                  seProtocol.getName(), protocolRule);

    this->protocolsMap.emplace(seProtocol, protocolRule);
}

void AbstractLocalReader::setSeProtocolSetting(std::unordered_map<SeProtocol,
                                               std::string> &protocolSetting)
{
    logger->debug("setSeProcotolSetting - adding %d protocols to map\n",
                  protocolSetting.size());

    this->protocolsMap.insert(protocolSetting.begin(), protocolSetting.end());
}

std::shared_ptr<SeResponseSet> AbstractLocalReader::processSeRequestSet(
    std::shared_ptr<SeRequestSet> requestSet)
{
    std::vector<bool> requestMatchesProtocol(requestSet->getRequests()->size());
    int requestIndex = 0, lastRequestIndex;

    /* Determine which requests are matching the current ATR */
    logger->debug("processSeRequestSet - determining which requests are " \
                  "matching the ATR\n");

    for (auto request : *(requestSet->getRequests())) {
        logger->debug("processSeRequestSet - retrieving request's se" \
                      "selector\n");

        std::shared_ptr<SeSelector> seSelector = request->getSeSelector();
        if (seSelector != nullptr) {
            logger->debug("processSeRequestSet - checking protocol flag " \
                          "match\n");
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

    std::vector<std::shared_ptr<SeResponse>> responses;
    bool stopProcess = false;

    for (auto request : *(requestSet->getRequests())) {
        logger->debug("processSeRequestSet - stopProcess ? %d\n", stopProcess);
        if (!stopProcess) {
            if (requestMatchesProtocol[requestIndex]) {
                logger->debug("[%s] processSeRequestSet => transmit %s\n",
                              AbstractLoggedObservable<ReaderEvent>::getName(),
                              request->toString());

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
                    ex.setSeResponseSet(
                        std::make_shared<SeResponseSet>(responses));
                    logger->debug(
                        "[%s] processSeRequestSet => transmit : process " \
                        "interrupted, collect previous responses %s\n",
                        AbstractLoggedObservable<ReaderEvent>::getName(),
                        "<to print>responses");
                    throw ex;
                }

                responses.push_back(response);
                logger->debug("[%s] processSeRequestSet => receive %s\n",
                              AbstractLoggedObservable<ReaderEvent>::getName(),
                              response->toString());
            } else {
                /*
                 * in case the protocolFlag of a SeRequest doesn't match the
                 * reader status, a null SeResponse is added to the
                 * SeResponseSet.
                 */
                responses.push_back(nullptr);
            }

            requestIndex++;
            if (!request->isKeepChannelOpen()) {
                if (lastRequestIndex == requestIndex) {
                    /*
                     * For the processing of the last SeRequest with a
                     * protocolFlag matching the SE reader status, if the
                     * logical channel doesn't require to be kept open, then the
                     * physical channel is closed.
                     */
                    closePhysicalChannel();

                    logger->debug(
                        "[%s] processSeRequestSet => Closing of the physical " \
                        "channel\n",
                        AbstractLoggedObservable<ReaderEvent>::getName());
                }
            } else {
                if (isLogicalChannelOpen()) {
                    stopProcess = true;
                }

                /*
                 * Exit the for loop here For the processing of a SeRequest with
                 * a protocolFlag When keepChannelOpen is true, we stop after
                 * the first matching request we which matches the current SE
                 * reader status, in case it's requested to keep the logical
                 * channel open, then the other remaining SeRequest are skipped,
                 * and null SeRequest are returned for them.
                 */
            }
        }
    }
    return std::make_shared<SeResponseSet>(responses);
}

std::shared_ptr<SeResponse>
AbstractLocalReader::processSeRequest(std::shared_ptr<SeRequest> seRequest)
{

    std::shared_ptr<SeResponse> seResponse = processSeRequestLogical(seRequest);

    /* close the physical channel if CLOSE_AFTER is requested */
    if (!seRequest->isKeepChannelOpen()) {
        closePhysicalChannel();
    }

    return seResponse;
}

std::shared_ptr<SeResponse> AbstractLocalReader::processSeRequestLogical(
    std::shared_ptr<SeRequest> seRequest)
{
    bool previouslyOpen = true;
    std::shared_ptr<SelectionStatus> selectionStatus = nullptr;

    std::vector<std::shared_ptr<ApduResponse>> apduResponseList;

    logger->trace("[%s] processSeRequest => Logical channel open = %s\n",
                  AbstractLoggedObservable<ReaderEvent>::getName(),
                  isLogicalChannelOpen());

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
            if (aidCurrentlySelected == nullptr) {
                throw std::make_shared<IllegalStateException>(
                          "AID currently selected shouldn't be null.");
            }

            if (seRequest->getSeSelector()->getAidSelector()
                    ->getFileOccurrence() ==
                        SeSelector::AidSelector::FileOccurrence::NEXT) {
                if (logger->isTraceEnabled()) {
                    logger->trace(
                        "[%s] processSeRequest => The current selection is a " \
                        " next selection, close the logical channel\n",
                        AbstractLoggedObservable<ReaderEvent>::getName());
                }

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
                if (logger->isTraceEnabled()) {
                    logger->trace(
                        "[%s] processSeRequest => The AID changed, close the " \
                        "logical channel. AID = %s, EXPECTEDAID = %s",
                        AbstractLoggedObservable<ReaderEvent>::getName(),
                        ByteArrayUtil::toHex(aidCurrentlySelected->getValue()),
                        seRequest->getSeSelector()->toString());
                }
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
                              "opening success\n",
                              AbstractLoggedObservable<ReaderEvent>::getName());
            }
            catch (const KeypleApplicationSelectionException &e) {
                (void)e;
                logger->trace("[%s] processSeRequest => Logical channel " \
                              "opening failure\n",
                              AbstractLoggedObservable<ReaderEvent>::getName());
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
            throw std::make_shared<IllegalStateException>(
                      "[" + AbstractLoggedObservable<ReaderEvent>::getName() +
                      "] processSeRequest => No logical channel opened!");
        } else {
            selectionStatus.reset();
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
                closeLogicalChannel();
                ex.setSeResponse(
                    std::make_shared<SeResponse>(false, previouslyOpen,
                                                 selectionStatus,
                                                 apduResponseList));
                throw ex;
            }
        }
    }

    /* Close the logical channel if requested */
    if (!seRequest->isKeepChannelOpen()) {
        closeLogicalChannel();
    }

    return std::make_shared<SeResponse>(isLogicalChannelOpen(), previouslyOpen,
                                        selectionStatus, apduResponseList);
}

std::shared_ptr<ApduResponse> AbstractLocalReader::processApduRequest(
    std::shared_ptr<ApduRequest> apduRequest)
{
    std::shared_ptr<ApduResponse> apduResponse;
    if (logger->isTraceEnabled()) {
        long long timeStamp = System::nanoTime();
        double elapsedMs =
            static_cast<double>((timeStamp - before) / 100000) / 10;
        this->before = timeStamp;
        logger->trace("[%s] processApduRequest => %s, elapsed %s ms\n",
                      AbstractLoggedObservable<ReaderEvent>::getName(),
                      apduRequest->toString(), elapsedMs);
    }


    std::vector<char> buffer = apduRequest->getBytes();
    std::vector<char> rapdu = transmitApdu(buffer);
    apduResponse = std::make_shared<ApduResponse>(
                       rapdu, apduRequest->getSuccessfulStatusCodes());

    if (apduRequest->isCase4()             &&
        apduResponse->getDataOut().empty() &&
        apduResponse->isSuccessful()) {
        /* Do the get response command but keep the original status code */
        apduResponse = case4HackGetResponse(apduResponse->getStatusCode());
    }

    if (logger->isTraceEnabled()) {
        long long timeStamp = System::nanoTime();
        double elapsedMs =
            static_cast<double>((timeStamp - before) / 100000) / 10;
        this->before = timeStamp;
        logger->trace("[%s] processApduRequest => %s, elapsed %s ms\n",
                      AbstractLoggedObservable<ReaderEvent>::getName(),
                      apduResponse->toString(), elapsedMs);
    }

    return apduResponse;
}

std::shared_ptr<ApduResponse>
AbstractLocalReader::case4HackGetResponse(int originalStatusCode)
{
    logger->debug("case4HackGetResponse\n");

    std::vector<char> getResponseHackRequestBytes =
        ByteArrayUtil::fromHex("00C0000000");

    /*
     * Build a get response command the actual length expected by the SE in the
     * get response command is handled in transmitApdu
     */
    if (logger->isTraceEnabled()) {
        long long timeStamp = System::nanoTime();
        double elapsedMs =
            static_cast<double>((timeStamp - this->before) / 100000) / 10;
        this->before = timeStamp;

        logger->debug("getResponseHackRequestBytes size: %d\n",
                      getResponseHackRequestBytes.size());

        logger->trace("[%s] case4HackGetResponse => ApduRequest: NAME = " \
                      "\"Internal Get Response\", RAWDATA = %s, elapsed = %s\n",
                      AbstractLoggedObservable<ReaderEvent>::getName(),
                      ByteArrayUtil::toHex(getResponseHackRequestBytes),
                      elapsedMs);
    }

    std::vector<char> getResponseHackResponseBytes =
        transmitApdu(getResponseHackRequestBytes);

    /* We expect here a 0x9000 status code */
    std::shared_ptr<ApduResponse> getResponseHackResponse =
        std::make_shared<ApduResponse>(getResponseHackResponseBytes, nullptr);

    if (logger->isTraceEnabled()) {
        long long timeStamp = System::nanoTime();
        double elapsedMs =
            static_cast<double>((timeStamp - this->before) / 100000) / 10;
        this->before = timeStamp;
        logger->trace("[%s] case4HackGetResponse => Internal %s, elapsed %s" \
                      "ms\n", AbstractLoggedObservable<ReaderEvent>::getName(),
                      "<to print>getResponseHackResponseBytes", elapsedMs);
    }

    if (getResponseHackResponse->isSuccessful()) {
        /* Replace the two last status word bytes by the original status word */
        getResponseHackResponseBytes[getResponseHackResponseBytes.size() - 2] =
            static_cast<char>(originalStatusCode >> 8);
        getResponseHackResponseBytes[getResponseHackResponseBytes.size() - 1] =
            static_cast<char>(originalStatusCode & 0xFF);
    }

    return getResponseHackResponse;
}

void AbstractLocalReader::setDefaultSelectionRequest(
    std::shared_ptr<AbstractDefaultSelectionsRequest> defaultSelectionsRequest,
    ObservableReader::NotificationMode notificationMode)
{
    this->defaultSelectionsRequest =
        std::static_pointer_cast<DefaultSelectionsRequest>(
            defaultSelectionsRequest);

    this->notificationMode = notificationMode;
}

}
}
}
}
