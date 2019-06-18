#include <typeinfo>

#include "AbstractLocalReader.h"
#include "SeRequest.h"
#include "SeProtocol.h"
#include "ReaderEvent_Import.h"
#include "SelectionResponse.h"
#include "KeypleReaderException.h"
#include "KeypleIOReaderException.h"
#include "ByteArrayUtils.h"
#include "KeypleApplicationSelectionException.h"
#include "SeProtocolSetting.h"
#include "SeResponse.h"
#include "SeResponseSet.h"
#include "ApduRequest.h"
#include "ApduResponse.h"
#include "SeRequestSet.h"
#include "SeRequest.h"
#include "Arrays.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

using ApduResponse                        = org::eclipse::keyple::core::seproxy::message::ApduResponse;
using ObservableReader                    = org::eclipse::keyple::core::seproxy::event::ObservableReader;
using ReaderEvent                         = org::eclipse::keyple::core::seproxy::event::ReaderEvent;
using KeypleApplicationSelectionException = org::eclipse::keyple::core::seproxy::exception::KeypleApplicationSelectionException;
using KeypleChannelStateException         = org::eclipse::keyple::core::seproxy::exception::KeypleChannelStateException;
using KeypleIOReaderException             = org::eclipse::keyple::core::seproxy::exception::KeypleIOReaderException;
using KeypleReaderException               = org::eclipse::keyple::core::seproxy::exception::KeypleReaderException;
using SeProtocol                          = org::eclipse::keyple::core::seproxy::protocol::SeProtocol;
using SeProtocolSetting                   = org::eclipse::keyple::core::seproxy::protocol::SeProtocolSetting;
using SelectionResponse                   = org::eclipse::keyple::core::seproxy::event::SelectionResponse;
using ByteArrayUtils                      = org::eclipse::keyple::core::util::ByteArrayUtils;
using SeRequestSet                        = org::eclipse::keyple::core::seproxy::message::SeRequestSet;

AbstractLocalReader::AbstractLocalReader(const std::string &pluginName, const std::string &readerName)
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

    if (defaultSelectionRequest == nullptr) {
        logger->debug("defaultSelectionRequest is null\n");

        /* no default request is defined, just notify the SE insertion */
        AbstractLoggedObservable::notifyObservers(std::make_shared<ReaderEvent>(this->pluginName, this->name, ReaderEvent::EventType::SE_INSERTED, nullptr));
        presenceNotified = true;
    } else {
        logger->debug("defaultSelectionRequest is not null\n");

        /*
            * a default request is defined, send it a notify according to the notification mode and
            * the selection status
            */
        bool aSeMatched = false;
        try {
            std::shared_ptr<SeResponseSet> seResponseSet = processSeRequestSet(defaultSelectionRequest->getSelectionSeRequestSet());

            for (auto seResponse : seResponseSet->getResponses()) {
                if (seResponse != nullptr && seResponse->getSelectionStatus()->hasMatched()) {
                    aSeMatched = true;
                    break;
                }
            }
            if (notificationMode == ObservableReader::NotificationMode::MATCHED_ONLY) {
                /* notify only if a SE matched the selection, just ignore if not */
                if (aSeMatched) {
                    AbstractLoggedObservable::notifyObservers(std::make_shared<ReaderEvent>(this->pluginName, this->name, ReaderEvent::EventType::SE_MATCHED, std::make_shared<SelectionResponse>(seResponseSet)));
                    presenceNotified = true;
                }
                else {
                    /* the SE did not match, close the logical channel */
                    closeLogicalChannel();
                }
            }
            else {
                if (aSeMatched) {
                    /* The SE matched, notify an SE_MATCHED event with the received response */
                    AbstractLoggedObservable::notifyObservers(std::make_shared<ReaderEvent>(this->pluginName, this->name, ReaderEvent::EventType::SE_MATCHED, std::make_shared<SelectionResponse>(seResponseSet)));
                }
                else {
                    /*
                        * The SE didn't match, notify an SE_INSERTED event with the received
                        * response
                        */
                    AbstractLoggedObservable::notifyObservers(std::make_shared<ReaderEvent>(this->pluginName, this->name, ReaderEvent::EventType::SE_INSERTED, std::make_shared<SelectionResponse>(seResponseSet)));
                }
                presenceNotified = true;
            }
        }
        catch (const KeypleReaderException &e) {
            /* the last transmission failed, close the logical channel */
            closeLogicalChannel();
            e.printStackTrace();
            // in this case the card has been removed or not read correctly, do not throw event
        }
    }
}

void AbstractLocalReader::cardRemoved()
{
    logger->debug("cardRemoved - \n");

    if (presenceNotified) {
        AbstractLoggedObservable::notifyObservers(std::make_shared<ReaderEvent>(this->pluginName, this->name, ReaderEvent::EventType::SE_REMOVAL, nullptr));
        presenceNotified = false;
    }

    closeLogicalChannel();

    try {
        closePhysicalChannel();
    }
    catch (const KeypleChannelStateException &e) {
        logger->trace("[%s] Exception occured in waitForCardAbsent. Message: %s", AbstractLoggedObservable<ReaderEvent>::getName(), e.what());
        throw std::make_shared<NoStackTraceThrowable>();
    }
}

std::shared_ptr<SelectionStatus> AbstractLocalReader::openLogicalChannelAndSelect(std::shared_ptr<SeSelector> seSelector)
{
    std::shared_ptr<SelectionStatus> selectionStatus;

    if (seSelector == nullptr) {
        throw std::make_shared<KeypleChannelStateException>("Try to open logical channel without selector.");
    }

    if (!isLogicalChannelOpen()) {
    /*
            * init of the physical SE channel: if not yet established, opening of a new physical
            * channel
        */
        if (!isPhysicalChannelOpen()) {
            openPhysicalChannel();
        }
        if (!isPhysicalChannelOpen()) {
            throw std::make_shared<KeypleChannelStateException>("Fail to open physical channel.");
        }
    }

    selectionStatus = openLogicalChannel(seSelector);

    return selectionStatus;
}

bool AbstractLocalReader::isLogicalChannelOpen() {
    return logicalChannelIsOpen;
}

void AbstractLocalReader::closeLogicalChannel() {
    logger->trace("[%s] closeLogicalChannel => Closing of the logical channel\n", AbstractLoggedObservable<ReaderEvent>::getName());
    logicalChannelIsOpen = false;
    aidCurrentlySelected.clear();
    currentSelectionStatus.reset();
}

void AbstractLocalReader::addSeProtocolSetting(std::shared_ptr<SeProtocolSetting> seProtocolSetting) {
    std::unordered_map<std::shared_ptr<SeProtocol>, std::string> map = seProtocolSetting->getProtocolsMap();
    this->protocolsMap.insert(map.begin(), map.end());
}

std::shared_ptr<SeResponseSet> AbstractLocalReader::processSeRequestSet(std::shared_ptr<SeRequestSet> requestSet)
{
    std::vector<bool> requestMatchesProtocol(requestSet->getRequests()->size());
    int requestIndex = 0, lastRequestIndex;

    // Determine which requests are matching the current ATR
    for (auto request : *(requestSet->getRequests())) {
        requestMatchesProtocol[requestIndex] = protocolFlagMatches(request->getProtocolFlag());
        requestIndex++;
    }

    /*
        * we have now an array of booleans saying whether the corresponding request and the current
        * SE match or not
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
        * If keepChannelOpen is false, we close the physical channel for the last request.
        */
    std::vector<std::shared_ptr<SeResponse>> responses;
    bool stopProcess = false;
    for (auto request : *(requestSet->getRequests())) {

        if (!stopProcess) {
            if (requestMatchesProtocol[requestIndex]) {
                logger->debug("[%s] processSeRequestSet => transmit %s\n",
                              AbstractLoggedObservable<ReaderEvent>::getName(), request);
                std::shared_ptr<SeResponse> response = nullptr;
                try {
                    response = processSeRequestLogical(request);
                }
                catch (KeypleReaderException &ex) {
                    /*
                        * The process has been interrupted. We launch a KeypleReaderException with
                        * the responses collected so far.
                        */
                    /* Add the latest (and partial) SeResponse to the current list. */
                    responses.push_back(ex.getSeResponse());
                    /* Build a SeResponseSet with the available data. */
                    ex.setSeResponseSet(std::make_shared<SeResponseSet>(responses));
                    logger->debug("[%s] processSeRequestSet => transmit : process interrupted, " \
                                  "collect previous responses %s\n",
                                  AbstractLoggedObservable<ReaderEvent>::getName(),
                                  "<to print>responses");
                    throw ex;
                }
                responses.push_back(response);
                logger->debug("[%s] processSeRequestSet => receive %s\n",
                              AbstractLoggedObservable<ReaderEvent>::getName(), response);
            }
            else {
                /*
                    * in case the protocolFlag of a SeRequest doesn't match the reader status, a
                    * null SeResponse is added to the SeResponseSet.
                    */
                responses.push_back(nullptr);
            }
            requestIndex++;
            if (!request->isKeepChannelOpen()) {
                if (lastRequestIndex == requestIndex) {
                    /*
                        * For the processing of the last SeRequest with a protocolFlag matching the
                        * SE reader status, if the logical channel doesn't require to be kept open,
                        * then the physical channel is closed.
                        */
                    closePhysicalChannel();

                    logger->debug("[%s] processSeRequestSet => Closing of the physical channel\n", AbstractLoggedObservable<ReaderEvent>::getName());
                }
            }
            else {
                if (isLogicalChannelOpen()) {
                    stopProcess = true;
                }
                /*
                    * When keepChannelOpen is true, we stop after the first matching request we
                    * exit the for loop here For the processing of a SeRequest with a protocolFlag
                    * which matches the current SE reader status, in case it's requested to keep
                    * the logical channel open, then the other remaining SeRequest are skipped, and
                    * null SeRequest are returned for them.
                    */
            }
        }
    }
    return std::make_shared<SeResponseSet>(responses);
}

std::shared_ptr<SeResponse> AbstractLocalReader::processSeRequest(std::shared_ptr<SeRequest> seRequest)
{

    std::shared_ptr<SeResponse> seResponse = processSeRequestLogical(seRequest);

    /* close the physical channel if CLOSE_AFTER is requested */
    if (!seRequest->isKeepChannelOpen()) {
        closePhysicalChannel();
    }

    return seResponse;
}

std::shared_ptr<SeResponse> AbstractLocalReader::processSeRequestLogical(std::shared_ptr<SeRequest> seRequest)
{
    bool previouslyOpen = true;
    std::shared_ptr<SelectionStatus> selectionStatus = nullptr;

    std::vector<std::shared_ptr<ApduResponse>> apduResponseList;

    logger->trace("[%s] processSeRequest => Logical channel open = %s\n",
                  AbstractLoggedObservable<ReaderEvent>::getName(), isLogicalChannelOpen());

    /*
        * unless the selector is null, we try to open a logical channel; if the channel was open
        * and the PO is still matching we won't redo the selection and just use the current
        * selection status
        */
    if (seRequest->getSeSelector() != nullptr) {
        /* check if AID changed if the channel is already open */
        if (isLogicalChannelOpen() && seRequest->getSeSelector()->getAidSelector() != nullptr) {
            /*
                * AID comparison hack: we check here if the initial selection AID matches the
                * beginning of the AID provided in the SeRequest (coming from FCI data and supposed
                * to be longer than the selection AID).
                *
                * The current AID (selector) length must be at least equal or greater than the
                * selection AID. All bytes of the selection AID must match the beginning of the
                * current AID.
                */
            if (aidCurrentlySelected.empty()) {
                throw std::make_shared<IllegalStateException>("AID currently selected shouldn't be null.");
            }
            if (seRequest->getSeSelector()->getAidSelector()->isSelectNext()) {
                if (logger->isTraceEnabled()) {
                    logger->trace("[%s] processSeRequest => The current selection is a next selection, close the logical channel\n",
                                    AbstractLoggedObservable<ReaderEvent>::getName());
                }
                /* close the channel (will reset the current selection status) */
                closeLogicalChannel();
            }
                else if (seRequest->getSeSelector()->getAidSelector()->getAidToSelect().size() >= aidCurrentlySelected.size() &&
                        aidCurrentlySelected == Arrays::copyOfRange(seRequest->getSeSelector()->getAidSelector()->getAidToSelect(), 0, aidCurrentlySelected.size())) {
                // the AID changed, close the logical channel
                if (logger->isTraceEnabled()) {
                    logger->trace("[%s] processSeRequest => The AID changed, " \
                                  "close the logical channel. AID = %s, " \
                                  "EXPECTEDAID = %s",
                                  AbstractLoggedObservable<ReaderEvent>::getName(),
                                  ByteArrayUtils::toHex(aidCurrentlySelected),
                                  seRequest->getSeSelector());
                }
                /* close the channel (will reset the current selection status) */
                closeLogicalChannel();
            }
            /* keep the current selection status (may be null if the current PO didn't match) */
            selectionStatus = currentSelectionStatus;
        }

        /* open the channel and do the selection if needed */
        if (!isLogicalChannelOpen()) {
            previouslyOpen = false;

            try {
                selectionStatus = openLogicalChannelAndSelect(seRequest->getSeSelector());
                logger->trace("[%s] processSeRequest => Logical channel opening success\n", AbstractLoggedObservable<ReaderEvent>::getName());
            }
            catch (const KeypleApplicationSelectionException &e) {
                (void)e;
                logger->trace("[%s] processSeRequest => Logical channel opening failure\n", AbstractLoggedObservable<ReaderEvent>::getName());
                closeLogicalChannel();
                /* return a null SeResponse when the opening of the logical channel failed */
                return nullptr;
            }

            if (selectionStatus->hasMatched()) {
                /* The selection process succeeded, the logical channel is open */
                logicalChannelIsOpen = true;

                if (selectionStatus->getFci()->isSuccessful()) {
                    /* the selection AID based was successful, keep the aid */
                    aidCurrentlySelected = seRequest->getSeSelector()->getAidSelector()->getAidToSelect();
                }
                currentSelectionStatus = selectionStatus;
            }
            else {
                /* The selection process failed, close the logical channel */
                closeLogicalChannel();
            }
        }
    }
    else {
        /* selector is null, we expect that the logical channel was previously opened */
        if (!isLogicalChannelOpen()) {
            throw std::make_shared<IllegalStateException>("[" + AbstractLoggedObservable<ReaderEvent>::getName() + "] processSeRequest => No logical channel opened!");
        }
        else {
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
                    * The process has been interrupted. We close the logical channel and launch a
                    * KeypleReaderException with the Apdu responses collected so far.
                    */
                logger->debug("The process has been interrupted, collect Apdu responses collected so far\n");
                closeLogicalChannel();
                ex.setSeResponse(std::make_shared<SeResponse>(previouslyOpen, selectionStatus, apduResponseList));
                throw ex;
            }
        }
    }

    /* close the logical channel if requested */
    if (!seRequest->isKeepChannelOpen()) {
        closeLogicalChannel();
    }

    return std::make_shared<SeResponse>(previouslyOpen, selectionStatus, apduResponseList);
}

std::shared_ptr<ApduResponse> AbstractLocalReader::processApduRequest(std::shared_ptr<ApduRequest> apduRequest)
{
    std::shared_ptr<ApduResponse> apduResponse;
    if (logger->isTraceEnabled()) {
        long long timeStamp = System::nanoTime();
        double elapsedMs = static_cast<double>((timeStamp - before) / 100000) / 10;
        this->before = timeStamp;
        logger->trace("[%s] processApduRequest => %s, elapsed %s ms\n",
                      AbstractLoggedObservable<ReaderEvent>::getName(), apduRequest, elapsedMs);
    }

    logger->debug("processApduRequest - retrieving request bytes\n");
    std::vector<char> buffer = apduRequest->getBytes();
    logger->debug("processApduRequest - request: %s\n", ByteArrayUtils::toHex(buffer));
    std::vector<char> rapdu = transmitApdu(buffer);
    logger->debug("processApduRequest - response: %s\n", ByteArrayUtils::toHex(rapdu));
    apduResponse = std::make_shared<ApduResponse>(rapdu, apduRequest->getSuccessfulStatusCodes());

    if (apduRequest->isCase4() && apduResponse->getDataOut().empty() && apduResponse->isSuccessful()) {
        // do the get response command but keep the original status code
        apduResponse = case4HackGetResponse(apduResponse->getStatusCode());
    }

    if (logger->isTraceEnabled()) {
        long long timeStamp = System::nanoTime();
        double elapsedMs = static_cast<double>((timeStamp - before) / 100000) / 10;
        this->before = timeStamp;
        logger->trace("[%s] processApduRequest => %s, elapsed %s ms\n",
                      AbstractLoggedObservable<ReaderEvent>::getName(), apduResponse, elapsedMs);
    }

    return apduResponse;
}

std::shared_ptr<ApduResponse> AbstractLocalReader::case4HackGetResponse(int originalStatusCode)
{
    logger->debug("case4HackGetResponse\n");

    std::vector<char> getResponseHackRequestBytes = ByteArrayUtils::fromHex("00C0000000");

    /*
     * build a get response command the actual length expected by the SE in the get response
     * command is handled in transmitApdu
     */
    if (logger->isTraceEnabled()) {
        long long timeStamp = System::nanoTime();
        double elapsedMs = static_cast<double>((timeStamp - this->before) / 100000) / 10;
        this->before = timeStamp;
	logger->debug("getResponseHackRequestBytes size: %d\n", getResponseHackRequestBytes.size());
        logger->trace("[%s] case4HackGetResponse => ApduRequest: NAME = \"Internal Get Response\", RAWDATA = %s, elapsed = %s\n",
                        AbstractLoggedObservable<ReaderEvent>::getName(), ByteArrayUtils::toHex(getResponseHackRequestBytes), elapsedMs);
    }

    std::vector<char> getResponseHackResponseBytes = transmitApdu(getResponseHackRequestBytes);

    /* we expect here a 0x9000 status code */
    std::shared_ptr<ApduResponse> getResponseHackResponse = std::make_shared<ApduResponse>(getResponseHackResponseBytes, nullptr);

    if (logger->isTraceEnabled()) {
        long long timeStamp = System::nanoTime();
        double elapsedMs = static_cast<double>((timeStamp - this->before) / 100000) / 10;
        this->before = timeStamp;
        logger->trace("[%s] case4HackGetResponse => Internal %s, elapsed %s ms\n",
                        AbstractLoggedObservable<ReaderEvent>::getName(), "<to print>getResponseHackResponseBytes", elapsedMs);
    }

    if (getResponseHackResponse->isSuccessful()) {
        // replace the two last status word bytes by the original status word
        getResponseHackResponseBytes[getResponseHackResponseBytes.size() - 2] = static_cast<char>(originalStatusCode >> 8);
        getResponseHackResponseBytes[getResponseHackResponseBytes.size() - 1] = static_cast<char>(originalStatusCode & 0xFF);
    }

    return getResponseHackResponse;
}

void AbstractLocalReader::setDefaultSelectionRequest(std::shared_ptr<DefaultSelectionRequest> defaultSelectionRequest,
                                                        ObservableReader::NotificationMode notificationMode)
{
    this->defaultSelectionRequest = defaultSelectionRequest;
    this->notificationMode = notificationMode;
}

}
}
}
}
}
}
