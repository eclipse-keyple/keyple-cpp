#include "AbstractLocalReader.h"
#include "../message/SeRequest.h"
#include "../protocol/SeProtocol.h"
#include "../../transaction/SelectionRequest.h"
#include "../event/ReaderEvent.h"
#include "../../transaction/SelectionResponse.h"
#include "../exception/KeypleReaderException.h"
#include "../exception/KeypleIOReaderException.h"
#include "../../util/ByteArrayUtils.h"
#include "../exception/KeypleApplicationSelectionException.h"
#include "../protocol/SeProtocolSetting.h"
#include "Logger.h"
#include "LoggerFactory.h"
#include <typeinfo>
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
            namespace seproxy {
                namespace plugin {

                    using ObservableReader = org::eclipse::keyple::seproxy::event::ObservableReader;
                    using ReaderEvent = org::eclipse::keyple::seproxy::event::ReaderEvent;
                    using KeypleApplicationSelectionException = org::eclipse::keyple::seproxy::exception::KeypleApplicationSelectionException;
                    using KeypleChannelStateException = org::eclipse::keyple::seproxy::exception::KeypleChannelStateException;
                    using KeypleIOReaderException = org::eclipse::keyple::seproxy::exception::KeypleIOReaderException;
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using namespace org::eclipse::keyple::seproxy::message;
                    using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                    using SeProtocolSetting = org::eclipse::keyple::seproxy::protocol::SeProtocolSetting;
                    using SelectionRequest = org::eclipse::keyple::transaction::SelectionRequest;
                    using SelectionResponse = org::eclipse::keyple::transaction::SelectionResponse;
                    using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;

                    std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(AbstractLocalReader));
                    std::vector<char> AbstractLocalReader::getResponseHackRequestBytes = org::eclipse::keyple::util::ByteArrayUtils::fromHex("00C0000000");

                    AbstractLocalReader::AbstractLocalReader(const std::string &pluginName, const std::string &readerName) : AbstractObservableReader(pluginName, readerName) {
                        this->before = System::nanoTime();
                    }

                    void AbstractLocalReader::setDefaultSelectionRequest(std::shared_ptr<SelectionRequest> defaultSelectionRequest, ObservableReader::NotificationMode notificationMode)     {
                        this->defaultSelectionRequest = defaultSelectionRequest;
                        this->notificationMode = notificationMode;
                    };

                    void AbstractLocalReader::cardRemoved() {
                        if (presenceNotified) {
//                            notifyObservers(std::make_shared<ReaderEvent>(this->pluginName, this->name, ReaderEvent::EventType::SE_REMOVAL, nullptr));
                            presenceNotified = false;
                        }
                    }

                    void AbstractLocalReader::cardInserted() {
                        if (defaultSelectionRequest == nullptr) {
                            /* no default request is defined, just notify the SE insertion */
//                            notifyObservers(std::make_shared<ReaderEvent>(this->pluginName, this->name, ReaderEvent::EventType::SE_INSERTED, nullptr));
                            presenceNotified = true;
                        }
                        else {
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
//                                        notifyObservers(std::make_shared<ReaderEvent>(this->pluginName, this->name, ReaderEvent::EventType::SE_MATCHED, std::make_shared<SelectionResponse>(seResponseSet)));
                                        presenceNotified = true;
                                    }
                                    else {
                                        /* the SE did not match, close the logical channel */
                                        closeLogicalChannel();
                                    }
                                }
                                else {
                                    /* notify an SE_INSERTED event with the received response */
//                                    notifyObservers(std::make_shared<ReaderEvent>(this->pluginName, this->name, ReaderEvent::EventType::SE_INSERTED, std::make_shared<SelectionResponse>(seResponseSet)));
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

                    std::shared_ptr<ApduResponse> AbstractLocalReader::processApduRequest(std::shared_ptr<ApduRequest> apduRequest) throw(KeypleIOReaderException) {
                        std::shared_ptr<ApduResponse> apduResponse;
//                        if (logger->isTraceEnabled()) {
//                            long long timeStamp = System::nanoTime();
//                            double elapsedMs = static_cast<double>((timeStamp - before) / 100000) / 10;
//                            this->before = timeStamp;
//                            logger->trace("[{}] processApduRequest => {}, elapsed {} ms.", this->getName(), apduRequest, elapsedMs);
//                        }

                        std::vector<char> buffer = apduRequest->getBytes();
                        std::vector<char> resp = transmitApdu(buffer);
                        ApduResponse _apdu(resp, apduRequest->getSuccessfulStatusCodes());
                        apduResponse = std::make_shared<ApduResponse>(_apdu);

                        if (apduRequest->isCase4() && apduResponse->getDataOut().empty() && apduResponse->isSuccessful()) {
                            // do the get response command but keep the original status code
                            apduResponse = case4HackGetResponse(apduResponse->getStatusCode());
                        }

                        if (logger->isTraceEnabled()) {
                            long long timeStamp = System::nanoTime();
                            double elapsedMs = static_cast<double>((timeStamp - before) / 100000) / 10;
                            this->before = timeStamp;
//                            logger->trace("[{}] processApduRequest => {}, elapsed {} ms.", this->getName(), apduResponse, elapsedMs);
                        }
                        return apduResponse;
                    }

                    std::shared_ptr<ApduResponse> AbstractLocalReader::case4HackGetResponse(int originalStatusCode) throw(KeypleIOReaderException) {
                        /*
                         * build a get response command the actual length expected by the SE in the get response
                         * command is handled in transmitApdu
                         */
                        if (logger->isTraceEnabled()) {
                            long long timeStamp = System::nanoTime();
                            double elapsedMs = static_cast<double>((timeStamp - this->before) / 100000) / 10;
                            this->before = timeStamp;
//                            logger->trace("[{}] case4HackGetResponse => ApduRequest: NAME = \"Internal Get Response\", RAWDATA = {}, elapsed = {}", this->getName(), ByteArrayUtils::toHex(getResponseHackRequestBytes), elapsedMs);
                        }

                        std::vector<char> getResponseHackResponseBytes = transmitApdu(getResponseHackRequestBytes);

                        /* we expect here a 0x9000 status code */
                        std::shared_ptr<ApduResponse> getResponseHackResponse = std::make_shared<ApduResponse>(getResponseHackResponseBytes, nullptr);

                        if (logger->isTraceEnabled()) {
                            long long timeStamp = System::nanoTime();
                            double elapsedMs = static_cast<double>((timeStamp - this->before) / 100000) / 10;
                            this->before = timeStamp;
//                            logger->trace("[{}] case4HackGetResponse => Internal {}, elapsed {} ms.", this->getName(), getResponseHackResponseBytes, elapsedMs);
                        }

                        if (getResponseHackResponse->isSuccessful()) {
                            // replace the two last status word bytes by the original status word
                            getResponseHackResponseBytes[getResponseHackResponseBytes.size() - 2] = static_cast<char>(originalStatusCode >> 8);
                            getResponseHackResponseBytes[getResponseHackResponseBytes.size() - 1] = static_cast<char>(originalStatusCode & 0xFF);
                        }
                        return getResponseHackResponse;
                    }

                    std::shared_ptr<SeResponseSet> AbstractLocalReader::processSeRequestSet(std::shared_ptr<SeRequestSet> requestSet) throw(KeypleReaderException) {

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
//                                    logger->debug("[{}] processSeRequestSet => transmit {}", this->getName(), request);
                                    std::shared_ptr<SeResponse> response = nullptr;
                                    try {
                                        response = processSeRequest(request);
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
                                        throw ex;
                                    }
                                    responses.push_back(response);
//                                    logger->debug("[{}] processSeRequestSet => receive {}", this->getName(), response);
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
                                    /*
                                     * always explicitly close the logical channel to possibly process a multiple
                                     * selection with the same AID
                                     */
                                    closeLogicalChannel();
                                    if (lastRequestIndex == requestIndex) {
                                        /*
                                         * For the processing of the last SeRequest with a protocolFlag matching the
                                         * SE reader status, if the logical channel doesn't require to be kept open,
                                         * then the physical channel is closed.
                                         */
                                        closePhysicalChannel();

//                                        logger->debug("[{}] processSeRequestSet => Closing of the physical channel.", this->getName());
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

                    bool AbstractLocalReader::isLogicalChannelOpen() {
                        return logicalChannelIsOpen;
                    }

                    void AbstractLocalReader::closeLogicalChannel() {
//                        logger->trace("[{}] closeLogicalChannel => Closing of the logical channel.", this->getName());
                        logicalChannelIsOpen = false;
                        aidCurrentlySelected.clear();
                        currentSelectionStatus.reset();
                    }

                    void AbstractLocalReader::setLogicalChannelOpen() {
                        logicalChannelIsOpen = true;
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings({"PMD.ModifiedCyclomaticComplexity", "PMD.CyclomaticComplexity", "PMD.StdCyclomaticComplexity", "PMD.NPathComplexity", "PMD.ExcessiveMethodLength"}) protected final SeResponse processSeRequest(SeRequest seRequest) throws IllegalStateException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    std::shared_ptr<SeResponse> AbstractLocalReader::processSeRequest(std::shared_ptr<SeRequest> seRequest) throw(IllegalStateException, KeypleReaderException) {
                        bool previouslyOpen = true;
                        std::shared_ptr<SelectionStatus> selectionStatus = nullptr;

                        std::vector<std::shared_ptr<ApduResponse>> apduResponseList;

                        logger->trace("[{}] processSeRequest => Logical channel open = {}", isLogicalChannelOpen());
                        /*
                         * unless the selector is null, we try to open a logical channel; if the channel was open
                         * and the PO is still matching we won't redo the selection and just use the current
                         * selection status
                         */
                        if (seRequest->getSelector() != nullptr) {
                            /* check if AID changed if the channel is already open */
                            if (isLogicalChannelOpen() && std::static_pointer_cast<SeRequest::AidSelector>(seRequest->getSelector()) != nullptr) {
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
                                if ((std::static_pointer_cast<SeRequest::AidSelector>(seRequest->getSelector()))->getAidToSelect().size() >= aidCurrentlySelected.size() &&
                                    aidCurrentlySelected == Arrays::copyOfRange((std::static_pointer_cast<SeRequest::AidSelector>(seRequest->getSelector()))->getAidToSelect(), 0, aidCurrentlySelected.size())) {
                                    // the AID changed, close the logical channel
                                    if (logger->isTraceEnabled()) {
//                                        logger->trace("[{}] processSeRequest => The AID changed, close the logical channel. AID = {}, EXPECTEDAID = {}", this->getName(), ByteArrayUtils::toHex(aidCurrentlySelected), seRequest->getSelector());
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
                                    selectionStatus = openLogicalChannelAndSelect(seRequest->getSelector(), seRequest->getSuccessfulSelectionStatusCodes());
//                                    logger->trace("[{}] processSeRequest => Logical channel opening success.", this->getName());
                                }
                                catch (const KeypleApplicationSelectionException &e) {
//                                    logger->trace("[{}] processSeRequest => Logical channel opening failure", this->getName());
                                    closeLogicalChannel();
                                    /* return a null SeResponse when the opening of the logical channel failed */
                                    return nullptr;
                                }

                                if (selectionStatus->hasMatched()) {
                                    /* The selection process succeeded, the logical channel is open */
                                    setLogicalChannelOpen();
                                    if (selectionStatus->getFci()->isSuccessful()) {
                                        /* the selection AID based was successful, keep the aid */
                                        aidCurrentlySelected = (std::static_pointer_cast<SeRequest::AidSelector>(seRequest->getSelector()))->getAidToSelect();
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
//                                throw std::make_shared<IllegalStateException>("[" + this->getName() + "] processSeRequest => No logical channel opened!");
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
                                    closeLogicalChannel();
                                    ex.setSeResponse(std::shared_ptr<SeResponse>(new SeResponse(previouslyOpen, selectionStatus, apduResponseList)));
                                    throw ex;
                                }
                            }
                        }

                        /* close the channel if requested */
                        if (!seRequest->isKeepChannelOpen()) {
                            closeLogicalChannel();
                        }

                        return std::shared_ptr<SeResponse>(new SeResponse(previouslyOpen, selectionStatus, apduResponseList));
                    }

                    void AbstractLocalReader::addSeProtocolSetting(std::shared_ptr<SeProtocolSetting> seProtocolSetting) {
                        this->protocolsMap = seProtocolSetting->getProtocolsMap();
                    }
                }
            }
        }
    }
}
