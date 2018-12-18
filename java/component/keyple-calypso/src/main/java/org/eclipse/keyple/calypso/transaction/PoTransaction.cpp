#include "PoTransaction.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ProxyReader.h"
#include "CalypsoPo.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/command/AbstractApduResponseParser.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeReader.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../command/sam/builder/session/SelectDiversifierCmdBuild.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/command/AbstractApduCommandBuilder.h"
#include "../command/sam/builder/session/SamGetChallengeCmdBuild.h"
#include "../command/sam/SamSendableInSession.h"
#include "exception/KeypleCalypsoSecureSessionException.h"
#include "../command/sam/parser/session/SamGetChallengeRespPars.h"
#include "../command/po/builder/session/AbstractOpenSessionCmdBuild.h"
#include "../command/SendableInSession.h"
#include "../command/po/parser/session/AbstractOpenSessionRespPars.h"
#include "../command/sam/parser/session/DigestCloseRespPars.h"
#include "../command/po/builder/session/CloseSessionCmdBuild.h"
#include "../command/po/parser/session/CloseSessionRespPars.h"
#include "../command/sam/builder/session/DigestAuthenticateCmdBuild.h"
#include "../command/sam/parser/session/DigestAuthenticateRespPars.h"
#include "../command/sam/builder/session/DigestInitCmdBuild.h"
#include "../command/sam/builder/session/DigestUpdateCmdBuild.h"
#include "../command/sam/builder/session/DigestCloseCmdBuild.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace transaction {
                    using SendableInSession = org::eclipse::keyple::calypso::command::SendableInSession;
                    using namespace org::eclipse::keyple::calypso::command::po;
                    using namespace org::eclipse::keyple::calypso::command::po::builder;
                    using AbstractOpenSessionCmdBuild = org::eclipse::keyple::calypso::command::po::builder::session::AbstractOpenSessionCmdBuild;
                    using CloseSessionCmdBuild = org::eclipse::keyple::calypso::command::po::builder::session::CloseSessionCmdBuild;
                    using namespace org::eclipse::keyple::calypso::command::po::parser;
                    using AbstractOpenSessionRespPars = org::eclipse::keyple::calypso::command::po::parser::session::AbstractOpenSessionRespPars;
                    using CloseSessionRespPars = org::eclipse::keyple::calypso::command::po::parser::session::CloseSessionRespPars;
                    using SamRevision = org::eclipse::keyple::calypso::command::sam::SamRevision;
                    using SamSendableInSession = org::eclipse::keyple::calypso::command::sam::SamSendableInSession;
                    using DigestAuthenticateCmdBuild = org::eclipse::keyple::calypso::command::sam::builder::session::DigestAuthenticateCmdBuild;
                    using SelectDiversifierCmdBuild = org::eclipse::keyple::calypso::command::sam::builder::session::SelectDiversifierCmdBuild;
                    using DigestAuthenticateRespPars = org::eclipse::keyple::calypso::command::sam::parser::session::DigestAuthenticateRespPars;
                    using DigestCloseRespPars = org::eclipse::keyple::calypso::command::sam::parser::session::DigestCloseRespPars;
                    using SamGetChallengeRespPars = org::eclipse::keyple::calypso::command::sam::parser::session::SamGetChallengeRespPars;
                    using namespace org::eclipse::keyple::calypso::transaction::exception;
                    using AbstractApduCommandBuilder = org::eclipse::keyple::command::AbstractApduCommandBuilder;
                    using AbstractApduResponseParser = org::eclipse::keyple::command::AbstractApduResponseParser;
                    using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                    using SeReader = org::eclipse::keyple::seproxy::SeReader;
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using namespace org::eclipse::keyple::seproxy::message;
                    using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
                    using TransmissionMode = org::eclipse::keyple::seproxy::protocol::TransmissionMode;
                    using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                    using org::slf4j::Logger;
                    using org::slf4j::LoggerFactory;
std::vector<char> const PoTransaction::ratificationCmdApduLegacy = org::eclipse::keyple::util::ByteArrayUtils::fromHex("94B2000000");
std::vector<char> const PoTransaction::ratificationCmdApdu = org::eclipse::keyple::util::ByteArrayUtils::fromHex("00B2000000");
const std::shared_ptr<org::slf4j::Logger> PoTransaction::logger = org::slf4j::LoggerFactory::getLogger(PoTransaction::typeid);

                    PoTransaction::PoTransaction(std::shared_ptr<SeReader> poReader, std::shared_ptr<CalypsoPo> calypsoPO, std::shared_ptr<SeReader> samReader, std::shared_ptr<EnumMap<SamSettings, Byte>> samSetting) : PoTransaction(poReader, calypsoPO) {


                        setSamSettings(samReader, samSetting);
                    }

                    PoTransaction::PoTransaction(std::shared_ptr<SeReader> poReader, std::shared_ptr<CalypsoPo> calypsoPO) : poReader(std::static_pointer_cast<ProxyReader>(poReader)), poCalypsoInstanceSerial(calypsoPO->getApplicationSerialNumber()), calypsoPo(calypsoPO) {


                        poRevision = calypsoPO->getRevision();

                        poCalypsoInstanceAid = calypsoPO->getDfName();

                        modificationsCounterIsInBytes = calypsoPO->isModificationsCounterInBytes();

                        modificationsCounterMax = modificationsCounter = calypsoPO->getModificationsCounter();

                        /* Serial Number of the selected Calypso instance. */

                        currentState = SessionState::SESSION_CLOSED;
                    }

                    void PoTransaction::setSamSettings(std::shared_ptr<SeReader> samReader, std::shared_ptr<EnumMap<SamSettings, Byte>> samSetting) {
                        this->samReader = std::static_pointer_cast<ProxyReader>(samReader);

                        /* Initialize samSetting with provided settings */
                        if (samSetting != nullptr) {
                            this->samSetting->putAll(samSetting);
                        }

                        /* Just work mode: we make sure that all the necessary parameters exist at least. */
                        if (!this->samSetting->containsKey(SamSettings::SAM_DEFAULT_KIF_PERSO)) {
                            this->samSetting->put(SamSettings::SAM_DEFAULT_KIF_PERSO, DEFAULT_KIF_PERSO);
                        }
                        if (!this->samSetting->containsKey(SamSettings::SAM_DEFAULT_KIF_LOAD)) {
                            this->samSetting->put(SamSettings::SAM_DEFAULT_KIF_LOAD, DEFAULT_KIF_LOAD);
                        }
                        if (!this->samSetting->containsKey(SamSettings::SAM_DEFAULT_KIF_DEBIT)) {
                            this->samSetting->put(SamSettings::SAM_DEFAULT_KIF_DEBIT, DEFAULT_KIF_DEBIT);
                        }
                        if (!this->samSetting->containsKey(SamSettings::SAM_DEFAULT_KEY_RECORD_NUMBER)) {
                            this->samSetting->put(SamSettings::SAM_DEFAULT_KEY_RECORD_NUMBER, DEFAULT_KEY_RECORD_NUMER);
                        }

                        logger->debug("Contructor => SAMSETTING = {}", this->samSetting);

                        samSettingsDefined = true;
                    }

                    void PoTransaction::setAuthorizedKvcList(std::vector<Byte> &authorizedKvcList) {
                        this->authorizedKvcList = authorizedKvcList;
                    }

                    bool PoTransaction::isSamSettingsDefined() {
                        return samSettingsDefined;
                    }

                    std::shared_ptr<SeResponse> PoTransaction::processAtomicOpening(SessionAccessLevel accessLevel, char openingSfiToSelect, char openingRecordNumberToRead, std::vector<std::shared_ptr<PoSendableInSession>> &poCommandsInsideSession) throw(KeypleReaderException) {

                        /*
                         * counts 'select diversifier' and 'get challenge' commands. At least get challenge is
                         * present
                         */
                        int numberOfSamCmd = 1;

                        /* SAM ApduRequest List to hold Select Diversifier and Get Challenge commands */
                        std::vector<std::shared_ptr<ApduRequest>> samApduRequestList;

                        if (logger->isDebugEnabled()) {
                            logger->debug("processAtomicOpening => Identification: DFNAME = {}, SERIALNUMBER = {}", ByteArrayUtils::toHex(poCalypsoInstanceAid), ByteArrayUtils::toHex(poCalypsoInstanceSerial));
                        }
                        /* diversify only if this has not already been done. */
                        if (!isDiversificationDone) {
                            /* Build the SAM Select Diversifier command to provide the SAM with the PO S/N */
                            std::shared_ptr<AbstractApduCommandBuilder> selectDiversifier = std::make_shared<SelectDiversifierCmdBuild>(this->samRevision, poCalypsoInstanceSerial);

                            samApduRequestList.push_back(selectDiversifier->getApduRequest());

                            /* increment command number */
                            numberOfSamCmd++;

                            /* change the diversification status */
                            isDiversificationDone = true;
                        }
                        /* Build the SAM Get Challenge command */
                        char challengeLength = poRevision.equals(PoRevision::REV3_2) ? CHALLENGE_LENGTH_REV32 : CHALLENGE_LENGTH_REV_INF_32;

                        std::shared_ptr<AbstractApduCommandBuilder> samGetChallenge = std::make_shared<org::eclipse::keyple::calypso::command::sam::builder::session::SamGetChallengeCmdBuild>(this->samRevision, challengeLength);

                        samApduRequestList.push_back(samGetChallenge->getApduRequest());

                        /* Build a SAM SeRequest */
                        std::shared_ptr<SeRequest> samSeRequest = std::make_shared<SeRequest>(samApduRequestList, ChannelState::KEEP_OPEN);

                        logger->debug("processAtomicOpening => identification: SAMSEREQUEST = {}", samSeRequest);

                        /*
                         * Transmit the SeRequest to the SAM and get back the SeResponse (list of ApduResponse)
                         */
                        std::shared_ptr<SeResponse> samSeResponse = samReader->transmit(samSeRequest);

                        if (samSeResponse == nullptr) {
                            throw std::make_shared<KeypleCalypsoSecureSessionException>("Null response received", KeypleCalypsoSecureSessionException::Type::SAM, samSeRequest->getApduRequests(), nullptr);
                        }

                        logger->debug("processAtomicOpening => identification: SAMSERESPONSE = {}", samSeResponse);

                        std::vector<std::shared_ptr<ApduResponse>> samApduResponseList = samSeResponse->getApduResponses();
                        std::vector<char> sessionTerminalChallenge;

                        if (samApduResponseList.size() == numberOfSamCmd && samApduResponseList[numberOfSamCmd - 1]->isSuccessful() && samApduResponseList[numberOfSamCmd - 1]->getDataOut()->size() == challengeLength) {
                            std::shared_ptr<SamGetChallengeRespPars> samChallengePars = std::make_shared<SamGetChallengeRespPars>(samApduResponseList[numberOfSamCmd - 1]);
                            sessionTerminalChallenge = samChallengePars->getChallenge();
                            if (logger->isDebugEnabled()) {
                                logger->debug("processAtomicOpening => identification: TERMINALCHALLENGE = {}", ByteArrayUtils::toHex(sessionTerminalChallenge));
                            }
                        }
                        else {
                            throw std::make_shared<KeypleCalypsoSecureSessionException>("Invalid message received", KeypleCalypsoSecureSessionException::Type::SAM, samApduRequestList, samApduResponseList);
                        }

                        /* PO ApduRequest List to hold Open Secure Session and other optional commands */
                        std::vector<std::shared_ptr<ApduRequest>> poApduRequestList;

                        /* Build the PO Open Secure Session command */
                        // TODO decide how to define the extraInfo field. Empty for the moment.
                        std::shared_ptr<AbstractOpenSessionCmdBuild> poOpenSession = AbstractOpenSessionCmdBuild::create(getRevision(), static_cast<char>(static_cast<int>(accessLevel) + 1), sessionTerminalChallenge, openingSfiToSelect, openingRecordNumberToRead, "");

                        /* Add the resulting ApduRequest to the PO ApduRequest list */
                        poApduRequestList.push_back(poOpenSession->getApduRequest());

                        /* Add all optional PoSendableInSession commands to the PO ApduRequest list */
                        if (poCommandsInsideSession.size() > 0) {
//JAVA TO C++ CONVERTER TODO TASK: Java wildcard generics are not converted to C++:
//ORIGINAL LINE: poApduRequestList.addAll(this.getApduRequestsToSendInSession((List<org.eclipse.keyple.calypso.command.SendableInSession>)(List<?>) poCommandsInsideSession));
                            poApduRequestList.addAll(this->getApduRequestsToSendInSession(static_cast<std::vector<std::shared_ptr<SendableInSession>>>(static_cast<std::vector<?>>(poCommandsInsideSession))));
                        }

                        /* Create a SeRequest from the ApduRequest list, PO AID as Selector, keep channel open */
                        std::shared_ptr<SeRequest> poSeRequest = std::make_shared<SeRequest>(poApduRequestList, ChannelState::KEEP_OPEN);

                        logger->debug("processAtomicOpening => opening:  POSEREQUEST = {}", poSeRequest);

                        /* Transmit the commands to the PO */
                        std::shared_ptr<SeResponse> poSeResponse = poReader->transmit(poSeRequest);

                        logger->debug("processAtomicOpening => opening:  POSERESPONSE = {}", poSeResponse);

                        if (poSeResponse == nullptr) {
                            throw std::make_shared<KeypleCalypsoSecureSessionException>("Null response received", KeypleCalypsoSecureSessionException::Type::PO, poSeRequest->getApduRequests(), nullptr);
                        }

                        if (poSeResponse->wasChannelPreviouslyOpen() == false) {
                            throw std::make_shared<KeypleCalypsoSecureSessionException>("The logical channel was not open", KeypleCalypsoSecureSessionException::Type::PO, poSeRequest->getApduRequests(), nullptr);
                        }

                        /* Retrieve and check the ApduResponses */
                        std::vector<std::shared_ptr<ApduResponse>> poApduResponseList = poSeResponse->getApduResponses();

                        /* Do some basic checks */
                        if (poApduRequestList.size() != poApduResponseList.size()) {
                            throw std::make_shared<KeypleCalypsoSecureSessionException>("Inconsistent requests and responses", KeypleCalypsoSecureSessionException::Type::PO, poApduRequestList, poApduResponseList);
                        }

                        for (auto apduR : poApduResponseList) {
                            if (!apduR->isSuccessful()) {
                                throw std::make_shared<KeypleCalypsoSecureSessionException>("Invalid response", KeypleCalypsoSecureSessionException::Type::PO, poApduRequestList, poApduResponseList);
                            }
                        }

                        /* Track Read Records for later use to build anticipated responses. */
                        AnticipatedResponseBuilder::storeCommandResponse(poCommandsInsideSession, poApduRequestList, poApduResponseList, true);

                        /* Parse the response to Open Secure Session (the first item of poApduResponseList) */
                        std::shared_ptr<AbstractOpenSessionRespPars> poOpenSessionPars = AbstractOpenSessionRespPars::create(poApduResponseList[0], poRevision);
                        std::vector<char> sessionCardChallenge = poOpenSessionPars->getPoChallenge();

                        /* Build the Digest Init command from PO Open Session */
                        poKif = poOpenSessionPars->getSelectedKif();
                        /** The PO KVC */
                        // TODO handle rev 1 KVC (provided in the response to select DF. CalypsoPo?)
                        char poKvc = poOpenSessionPars->getSelectedKvc();

                        if (logger->isDebugEnabled()) {
                            logger->debug("processAtomicOpening => opening: CARDCHALLENGE = {}, POKIF = {}, POKVC = {}", ByteArrayUtils::toHex(sessionCardChallenge), std::string::format("%02X", poKif), std::string::format("%02X", poKvc));
                        }

                        if (authorizedKvcList.size() > 0 && !std::find(authorizedKvcList.begin(), authorizedKvcList.end(), poKvc) != authorizedKvcList.end()) {
                            throw std::make_shared<KeypleCalypsoSecureSessionUnauthorizedKvcException>(std::string::format("PO KVC = %02X", poKvc));
                        }

                        char kif;
                        if (poKif == KIF_UNDEFINED) {
                            switch (accessLevel) {
                                case org::eclipse::keyple::calypso::transaction::PoTransaction::SessionAccessLevel::SESSION_LVL_PERSO:
                                    kif = samSetting->get(SamSettings::SAM_DEFAULT_KIF_PERSO);
                                    break;
                                case org::eclipse::keyple::calypso::transaction::PoTransaction::SessionAccessLevel::SESSION_LVL_LOAD:
                                    kif = samSetting->get(SamSettings::SAM_DEFAULT_KIF_LOAD);
                                    break;
                                case org::eclipse::keyple::calypso::transaction::PoTransaction::SessionAccessLevel::SESSION_LVL_DEBIT:
                                default:
                                    kif = samSetting->get(SamSettings::SAM_DEFAULT_KIF_DEBIT);
                                    break;
                            }
                        }
                        else {
                            kif = poKif;
                        }

                        /* Keep the ratification status and read data */
                        wasRatified_Renamed = poOpenSessionPars->wasRatified();
                        openRecordDataRead = poOpenSessionPars->getRecordDataRead();

                        /*
                         * Initialize the DigestProcessor. It will store all digest operations (Digest Init, Digest
                         * Update) until the session closing. AT this moment, all SAM Apdu will be processed at
                         * once.
                         */
                        DigestProcessor::initialize(poRevision, samRevision, false, false, poRevision.equals(PoRevision::REV3_2), samSetting->get(SamSettings::SAM_DEFAULT_KEY_RECORD_NUMBER), kif, poKvc, poApduResponseList[0]->getDataOut());

                        /*
                         * Add all commands data to the digest computation. The first command in the list is the
                         * open secure session command. This command is not included in the digest computation, so
                         * we skip it and start the loop at index 1.
                         */
                        if ((poCommandsInsideSession.size() > 0) && !poCommandsInsideSession.empty()) {

                            for (int i = 1; i < poApduRequestList.size(); i++) {
                                /*
                                 * Add requests and responses to the DigestProcessor
                                 */
                                DigestProcessor::pushPoExchangeData(poApduRequestList[i], poApduResponseList[i]);
                            }
                        }

                        currentState = SessionState::SESSION_OPEN;

                        /* Remove Open Secure Session response and create a new SeResponse */
                        poApduResponseList.erase(poApduResponseList.begin());

                        return std::make_shared<SeResponse>(true, poSeResponse->getSelectionStatus(), poApduResponseList);
                    }

                    std::vector<std::shared_ptr<ApduRequest>> PoTransaction::getApduRequestsToSendInSession(std::vector<std::shared_ptr<SendableInSession>> &poOrSamCommandsInsideSession) {
                        std::vector<std::shared_ptr<ApduRequest>> apduRequestList;
                        if (poOrSamCommandsInsideSession.size() > 0) {
                            for (auto cmd : poOrSamCommandsInsideSession) {
                                apduRequestList.push_back((std::static_pointer_cast<AbstractApduCommandBuilder>(cmd))->getApduRequest());
                            }
                        }
                        return apduRequestList;
                    }

                    std::shared_ptr<SeResponse> PoTransaction::processAtomicPoCommands(std::vector<std::shared_ptr<PoSendableInSession>> &poCommands, ChannelState channelState) throw(KeypleReaderException) {

                        // Get PO ApduRequest List from PoSendableInSession List
//JAVA TO C++ CONVERTER TODO TASK: Java wildcard generics are not converted to C++:
//ORIGINAL LINE: List<ApduRequest> poApduRequestList = this.getApduRequestsToSendInSession((List<org.eclipse.keyple.calypso.command.SendableInSession>)(List<?>) poCommands);
                        std::vector<std::shared_ptr<ApduRequest>> poApduRequestList = this->getApduRequestsToSendInSession(static_cast<std::vector<std::shared_ptr<SendableInSession>>>(static_cast<std::vector<?>>(poCommands)));

                        /*
                         * Create a SeRequest from the ApduRequest list, PO AID as Selector, manage the logical
                         * channel according to the channelState enum
                         */
                        std::shared_ptr<SeRequest> poSeRequest = std::make_shared<SeRequest>(poApduRequestList, channelState);

                        logger->debug("processAtomicPoCommands => POREQUEST = {}", poSeRequest);

                        /* Transmit the commands to the PO */
                        std::shared_ptr<SeResponse> poSeResponse = poReader->transmit(poSeRequest);

                        logger->debug("processAtomicPoCommands => PORESPONSE = {}", poSeResponse);

                        if (poSeResponse == nullptr) {
                            throw std::make_shared<KeypleCalypsoSecureSessionException>("Null response received", KeypleCalypsoSecureSessionException::Type::PO, poSeRequest->getApduRequests(), nullptr);
                        }

                        if (poSeResponse->wasChannelPreviouslyOpen() == false) {
                            throw std::make_shared<KeypleCalypsoSecureSessionException>("The logical channel was not open", KeypleCalypsoSecureSessionException::Type::PO, poSeRequest->getApduRequests(), nullptr);
                        }

                        /* Retrieve and check the ApduResponses */
                        std::vector<std::shared_ptr<ApduResponse>> poApduResponseList = poSeResponse->getApduResponses();

                        /* Do some basic checks */
                        if (poApduRequestList.size() != poApduResponseList.size()) {
                            throw std::make_shared<KeypleCalypsoSecureSessionException>("Inconsistent requests and responses", KeypleCalypsoSecureSessionException::Type::PO, poApduRequestList, poApduResponseList);
                        }

                        for (auto apduR : poApduResponseList) {
                            if (!apduR->isSuccessful()) {
                                throw std::make_shared<KeypleCalypsoSecureSessionException>("Invalid response", KeypleCalypsoSecureSessionException::Type::PO, poApduRequestList, poApduResponseList);
                            }
                        }

                        /* Track Read Records for later use to build anticipated responses. */
                        AnticipatedResponseBuilder::storeCommandResponse(poCommands, poApduRequestList, poApduResponseList, false);

                        /*
                         * Add all commands data to the digest computation if this method is called within a Secure
                         * Session.
                         */
                        if (currentState == SessionState::SESSION_OPEN) {
                            for (int i = 0; i < poApduRequestList.size(); i++) {
                                /*
                                 * Add requests and responses to the DigestProcessor
                                 */
                                DigestProcessor::pushPoExchangeData(poApduRequestList[i], poApduResponseList[i]);
                            }
                        }
                        return poSeResponse;
                    }

                    std::shared_ptr<SeResponse> PoTransaction::processSamCommands(std::vector<std::shared_ptr<SamSendableInSession>> &samCommands) throw(KeypleReaderException) {

                        /* Init SAM ApduRequest List - for the first SAM exchange */
//JAVA TO C++ CONVERTER TODO TASK: Java wildcard generics are not converted to C++:
//ORIGINAL LINE: List<ApduRequest> samApduRequestList = this.getApduRequestsToSendInSession((List<org.eclipse.keyple.calypso.command.SendableInSession>)(List<?>) samCommands);
                        std::vector<std::shared_ptr<ApduRequest>> samApduRequestList = this->getApduRequestsToSendInSession(static_cast<std::vector<std::shared_ptr<SendableInSession>>>(static_cast<std::vector<?>>(samCommands)));

                        /* SeRequest from the command list */
                        std::shared_ptr<SeRequest> samSeRequest = std::make_shared<SeRequest>(samApduRequestList, ChannelState::KEEP_OPEN);

                        logger->debug("processSamCommands => SAMSEREQUEST = {}", samSeRequest);

                        /* Transmit SeRequest and get SeResponse */
                        std::shared_ptr<SeResponse> samSeResponse = samReader->transmit(samSeRequest);

                        if (samSeResponse == nullptr) {
                            throw std::make_shared<KeypleCalypsoSecureSessionException>("Null response received", KeypleCalypsoSecureSessionException::Type::SAM, samSeRequest->getApduRequests(), nullptr);
                        }

                        if (currentState == SessionState::SESSION_OPEN && samSeResponse->wasChannelPreviouslyOpen() == false) {
                            throw std::make_shared<KeypleCalypsoSecureSessionException>("The logical channel was not open", KeypleCalypsoSecureSessionException::Type::SAM, samSeRequest->getApduRequests(), nullptr);
                        }
                        // TODO check if the wasChannelPreviouslyOpen should be done in the case where the session
                        // is closed

                        return samSeResponse;
                    }

                    std::shared_ptr<SeResponse> PoTransaction::processAtomicClosing(std::vector<std::shared_ptr<PoModificationCommand>> &poModificationCommands, std::vector<std::shared_ptr<ApduResponse>> &poAnticipatedResponses, TransmissionMode transmissionMode, ChannelState channelState) throw(KeypleReaderException) {

                        if (currentState != SessionState::SESSION_OPEN) {
                            throw std::make_shared<IllegalStateException>("Bad session state. Current: " + currentState.toString() + ", expected: " + SessionState::SESSION_OPEN.toString());
                        }

                        /* Get PO ApduRequest List from PoSendableInSession List - for the first PO exchange */
//JAVA TO C++ CONVERTER TODO TASK: Java wildcard generics are not converted to C++:
//ORIGINAL LINE: List<ApduRequest> poApduRequestList = this.getApduRequestsToSendInSession((List<org.eclipse.keyple.calypso.command.SendableInSession>)(List<?>) poModificationCommands);
                        std::vector<std::shared_ptr<ApduRequest>> poApduRequestList = this->getApduRequestsToSendInSession(static_cast<std::vector<std::shared_ptr<SendableInSession>>>(static_cast<std::vector<?>>(poModificationCommands)));

                        /* Compute "anticipated" Digest Update (for optional poModificationCommands) */
                        if ((poModificationCommands.size() > 0) && !poApduRequestList.empty()) {
                            if (poApduRequestList.size() == poAnticipatedResponses.size()) {
                                /*
                                 * Add all commands data to the digest computation: commands and anticipated
                                 * responses.
                                 */
                                for (int i = 0; i < poApduRequestList.size(); i++) {
                                    /*
                                     * Add requests and responses to the DigestProcessor
                                     */
                                    DigestProcessor::pushPoExchangeData(poApduRequestList[i], poAnticipatedResponses[i]);
                                }
                            }
                            else {
                                throw std::make_shared<KeypleCalypsoSecureSessionException>("Inconsistent requests and anticipated responses", KeypleCalypsoSecureSessionException::Type::PO, poApduRequestList, poAnticipatedResponses);
                            }
                        }

                        /* All SAM digest operations will now run at once. */
                        /* Get the SAM Digest request from the cache manager */
                        std::shared_ptr<SeRequest> samSeRequest = DigestProcessor::getSamDigestRequest();

                        logger->debug("processAtomicClosing => SAMREQUEST = {}", samSeRequest);

                        /* Transmit SeRequest and get SeResponse */
                        std::shared_ptr<SeResponse> samSeResponse = samReader->transmit(samSeRequest);

                        logger->debug("processAtomicClosing => SAMRESPONSE = {}", samSeResponse);

                        if (samSeResponse == nullptr) {
                            throw std::make_shared<KeypleCalypsoSecureSessionException>("Null response received", KeypleCalypsoSecureSessionException::Type::SAM, samSeRequest->getApduRequests(), nullptr);
                        }

                        if (samSeResponse->wasChannelPreviouslyOpen() == false) {
                            throw std::make_shared<KeypleCalypsoSecureSessionException>("The logical channel was not open", KeypleCalypsoSecureSessionException::Type::PO, samSeRequest->getApduRequests(), nullptr);
                        }

                        std::vector<std::shared_ptr<ApduResponse>> samApduResponseList = samSeResponse->getApduResponses();

                        for (int i = 0; i < samApduResponseList.size(); i++) {
                            if (!samApduResponseList[i]->isSuccessful()) {

                                logger->debug("processAtomicClosing => command failure REQUEST = {}, RESPONSE = {}", samSeRequest->getApduRequests()[i], samApduResponseList[i]);
                                throw std::make_shared<IllegalStateException>("ProcessClosing command failure during digest computation process.");
                            }
                        }

                        /* Get Terminal Signature from the latest response */
                        std::vector<char> sessionTerminalSignature;
                        // TODO Add length check according to Calypso REV (4 / 8)
                        if (!samApduResponseList.empty()) {
                            std::shared_ptr<DigestCloseRespPars> respPars = std::make_shared<DigestCloseRespPars>(samApduResponseList[samApduResponseList.size() - 1]);

                            sessionTerminalSignature = respPars->getSignature();
                        }

                        if (logger->isDebugEnabled()) {
                            logger->debug("processAtomicClosing => SIGNATURE = {}", ByteArrayUtils::toHex(sessionTerminalSignature));
                        }

                        std::shared_ptr<PoCustomReadCommandBuilder> ratificationCommand;
                        bool ratificationAsked;

                        if (transmissionMode == TransmissionMode::CONTACTLESS) {
                            if (poRevision == PoRevision::REV2_4) {
                                ratificationCommand = std::make_shared<PoCustomReadCommandBuilder>("Ratification command", std::make_shared<ApduRequest>(ratificationCmdApduLegacy, false));
                            }
                            else {
                                ratificationCommand = std::make_shared<PoCustomReadCommandBuilder>("Ratification command", std::make_shared<ApduRequest>(ratificationCmdApdu, false));
                            }
                            /*
                             * Ratification is done by the ratification command above so is not requested in the
                             * Close Session command
                             */
                            ratificationAsked = false;
                        }
                        else {
                            /* Ratification is requested in the Close Session command in contacts mode */
                            ratificationAsked = true;
                            ratificationCommand.reset();
                        }

                        /* Build the PO Close Session command. The last one for this session */
                        std::shared_ptr<CloseSessionCmdBuild> closeCommand = std::make_shared<CloseSessionCmdBuild>(calypsoPo->getPoClass(), ratificationAsked, sessionTerminalSignature);

                        poApduRequestList.push_back(closeCommand->getApduRequest());

                        /* Keep the position of the Close Session command in request list */
                        int closeCommandIndex = poApduRequestList.size() - 1;

                        /*
                         * Add the PO Ratification command if any
                         */
                        if (ratificationCommand != nullptr) {
                            poApduRequestList.push_back(ratificationCommand->getApduRequest());
                        }

                        /*
                         * Transfer PO commands
                         */
                        std::shared_ptr<SeRequest> poSeRequest = std::make_shared<SeRequest>(poApduRequestList, channelState);

                        logger->debug("processAtomicClosing => POSEREQUEST = {}", poSeRequest);

                        std::shared_ptr<SeResponse> poSeResponse;
                        try {
                            poSeResponse = poReader->transmit(poSeRequest);
                        }
                        catch (const KeypleReaderException &ex) {
                            poSeResponse = ex->getSeResponse();
                            /*
                             * The current exception may have been caused by a communication issue with the PO
                             * during the ratification command.
                             *
                             * In this case, we do not stop the process and consider the Secure Session close. We'll
                             * check the signature.
                             *
                             * We should have one response less than requests.
                             */
                            if (ratificationAsked || poSeResponse == nullptr || poSeResponse->getApduResponses().size() != poApduRequestList.size() - 1) {
                                /* Add current PO SeResponse to exception */
                                ex->setSeResponse(poSeResponse);
                                throw std::make_shared<KeypleReaderException>("PO Reader Exception while closing Secure Session", ex);
                            }
                        }

                        if (poSeResponse == nullptr) {
                            throw std::make_shared<KeypleCalypsoSecureSessionException>("Null response received", KeypleCalypsoSecureSessionException::Type::PO, poSeRequest->getApduRequests(), nullptr);
                        }

                        if (poSeResponse->wasChannelPreviouslyOpen() == false) {
                            throw std::make_shared<KeypleCalypsoSecureSessionException>("The logical channel was not open", KeypleCalypsoSecureSessionException::Type::PO, poSeRequest->getApduRequests(), nullptr);
                        }

                        logger->debug("processAtomicClosing => POSERESPONSE = {}", poSeResponse);

                        std::vector<std::shared_ptr<ApduResponse>> poApduResponseList = poSeResponse->getApduResponses();

                        // TODO add support of poRevision parameter to CloseSessionRespPars for REV2.4 PO CLAss byte
                        // before last if ratification, otherwise last one
                        std::shared_ptr<CloseSessionRespPars> poCloseSessionPars = std::make_shared<CloseSessionRespPars>(poApduResponseList[closeCommandIndex]);
                        if (!poCloseSessionPars->isSuccessful()) {
                            throw std::make_shared<KeypleCalypsoSecureSessionException>("Didn't get a signature", KeypleCalypsoSecureSessionException::Type::PO, poApduRequestList, poApduResponseList);
                        }

                        /* Check the PO signature part with the SAM */
                        /* Build and send SAM Digest Authenticate command */
                        std::shared_ptr<AbstractApduCommandBuilder> digestAuth = std::make_shared<DigestAuthenticateCmdBuild>(samRevision, poCloseSessionPars->getSignatureLo());

                        std::vector<std::shared_ptr<ApduRequest>> samApduRequestList;
                        samApduRequestList.push_back(digestAuth->getApduRequest());

                        samSeRequest = std::make_shared<SeRequest>(samApduRequestList, ChannelState::KEEP_OPEN);

                        logger->debug("PoTransaction.DigestProcessor => checkPoSignature: SAMREQUEST = {}", samSeRequest);

                        samSeResponse = samReader->transmit(samSeRequest);

                        logger->debug("PoTransaction.DigestProcessor => checkPoSignature: SAMRESPONSE = {}", samSeResponse);

                        if (samSeResponse == nullptr) {
                            throw std::make_shared<KeypleCalypsoSecureSessionException>("Null response received", KeypleCalypsoSecureSessionException::Type::SAM, samSeRequest->getApduRequests(), nullptr);
                        }

                        if (samSeResponse->wasChannelPreviouslyOpen() == false) {
                            throw std::make_shared<KeypleCalypsoSecureSessionException>("The logical channel was not open", KeypleCalypsoSecureSessionException::Type::SAM, samSeRequest->getApduRequests(), nullptr);
                        }

                        /* Get transaction result parsing the response */
                        samApduResponseList = samSeResponse->getApduResponses();

                        transactionResult = false;
                        if ((samApduResponseList.size() > 0) && !samApduResponseList.empty()) {
                            std::shared_ptr<DigestAuthenticateRespPars> respPars = std::make_shared<DigestAuthenticateRespPars>(samApduResponseList[0]);
                            transactionResult = respPars->isSuccessful();
                            if (transactionResult) {
                                logger->debug("PoTransaction.DigestProcessor => checkPoSignature: mutual authentication successful.");
                            }
                            else {
                                logger->debug("PoTransaction.DigestProcessor => checkPoSignature: mutual authentication failure.");
                            }
                        }
                        else {
                            logger->debug("DigestProcessor => checkPoSignature: no response to Digest Authenticate.");
                            throw std::make_shared<IllegalStateException>("No response to Digest Authenticate.");
                        }

                        currentState = SessionState::SESSION_CLOSED;

                        /* Remove ratification response if any */
                        if (!ratificationAsked) {
                            poApduResponseList.pop_back();
                        }
                        /* Remove Close Secure Session response and create a new SeResponse */
                        poApduResponseList.pop_back();

                        return std::make_shared<SeResponse>(true, poSeResponse->getSelectionStatus(), poApduResponseList);
                    }

                    std::shared_ptr<SeResponse> PoTransaction::processAtomicClosing(std::vector<std::shared_ptr<PoModificationCommand>> &poModificationCommands, TransmissionMode transmissionMode, ChannelState channelState) throw(KeypleReaderException) {
                        std::vector<std::shared_ptr<ApduResponse>> poAnticipatedResponses = AnticipatedResponseBuilder::getResponses(poModificationCommands);
                        return processAtomicClosing(poModificationCommands, poAnticipatedResponses, transmissionMode, channelState);
                    }

                    PoRevision PoTransaction::getRevision() {
                        // TODO checks if poRevision initialized
                        return poRevision;
                    }

                    bool PoTransaction::isSuccessful() {

                        if (currentState != SessionState::SESSION_CLOSED) {
                            throw std::make_shared<IllegalStateException>("Session is not closed, state:" + currentState.toString() + ", expected: " + SessionState::SESSION_OPEN.toString());
                        }

                        return transactionResult;
                    }

                    char PoTransaction::getPoKif() {
                        return poKif;
                    }

                    bool PoTransaction::wasRatified() {
                        return wasRatified_Renamed;
                    }

                    std::vector<char> PoTransaction::getOpenRecordDataRead() {
                        return openRecordDataRead;
                    }

const std::vector<std::vector<char>> PoTransaction::DigestProcessor::poDigestDataCache = std::vector<std::vector<char>>();
org::eclipse::keyple::calypso::command::sam::SamRevision PoTransaction::DigestProcessor::samRevision = (org.eclipse.keyple.calypso.command.sam.SamRevision)0;
PoRevision PoTransaction::DigestProcessor::poRevision = (PoRevision)0;
bool PoTransaction::DigestProcessor::encryption = false;
bool PoTransaction::DigestProcessor::verification = false;
bool PoTransaction::DigestProcessor::revMode = false;
char PoTransaction::DigestProcessor::keyRecordNumber = 0;
char PoTransaction::DigestProcessor::keyKIF = 0;
char PoTransaction::DigestProcessor::keyKVC = 0;

                    void PoTransaction::DigestProcessor::initialize(PoRevision poRev, SamRevision samRev, bool sessionEncryption, bool verificationMode, bool rev3_2Mode, char workKeyRecordNumber, char workKeyKif, char workKeyKVC, std::vector<char> &digestData) {
                        /* Store work context */
                        poRevision = poRev;
                        samRevision = samRev;
                        encryption = sessionEncryption;
                        verification = verificationMode;
                        revMode = rev3_2Mode;
                        keyRecordNumber = workKeyRecordNumber;
                        keyKIF = workKeyKif;
                        keyKVC = workKeyKVC;
                        if (logger->isDebugEnabled()) {
                            logger->debug("PoTransaction.DigestProcessor => initialize: POREVISION = {}, SAMREVISION = {}, SESSIONENCRYPTION = {}", poRev, samRev, sessionEncryption, verificationMode);
                            logger->debug("PoTransaction.DigestProcessor => initialize: VERIFICATIONMODE = {}, REV32MODE = {} KEYRECNUMBER = {}", verificationMode, rev3_2Mode, workKeyRecordNumber);
                            logger->debug("PoTransaction.DigestProcessor => initialize: KIF = {}, KVC {}, DIGESTDATA = {}", std::string::format("%02X", workKeyKif), std::string::format("%02X", workKeyKVC), ByteArrayUtils::toHex(digestData));
                        }

                        /* Clear data cache */
                        poDigestDataCache.clear();

                        /*
                         * Build Digest Init command as first ApduRequest of the digest computation process
                         */
                        poDigestDataCache.push_back(digestData);
                    }

                    void PoTransaction::DigestProcessor::pushPoExchangeData(std::shared_ptr<ApduRequest> request, std::shared_ptr<ApduResponse> response) {

                        logger->debug("PoTransaction.DigestProcessor => pushPoExchangeData: REQUEST = {}", request);

                        /*
                         * Add an ApduRequest to the digest computation: if the request is of case4 type, Le
                         * must be excluded from the digest computation. In this cas, we remove here the last
                         * byte of the command buffer.
                         */
                        if (request->isCase4()) {
                            poDigestDataCache.push_back(Arrays::copyOfRange(request->getBytes(), 0, request->getBytes().size() - 1));
                        }
                        else {
                            poDigestDataCache.push_back(request->getBytes());
                        }

                        logger->debug("PoTransaction.DigestProcessor => pushPoExchangeData: RESPONSE = {}", response);

                        /* Add an ApduResponse to the digest computation */
                        poDigestDataCache.push_back(response->getBytes());
                    }

                    std::shared_ptr<SeRequest> PoTransaction::DigestProcessor::getSamDigestRequest() {
                        std::vector<std::shared_ptr<ApduRequest>> samApduRequestList;

                        if (poDigestDataCache.empty()) {
                            logger->debug("PoTransaction.DigestProcessor => getSamDigestRequest: no data in cache.");
                            throw std::make_shared<IllegalStateException>("Digest data cache is empty.");
                        }
                        if (poDigestDataCache.size() % 2 == 0) {
                            /* the number of buffers should be 2*n + 1 */
                            logger->debug("PoTransaction.DigestProcessor => getSamDigestRequest: wrong number of buffer in cache NBR = {}.", poDigestDataCache.size());
                            throw std::make_shared<IllegalStateException>("Digest data cache is inconsistent.");
                        }

                        /*
                         * Build and append Digest Init command as first ApduRequest of the digest computation
                         * process
                         */
                        samApduRequestList.push_back((std::make_shared<org::eclipse::keyple::calypso::command::sam::builder::session::DigestInitCmdBuild>(samRevision, verification, revMode, keyRecordNumber, keyKIF, keyKVC, poDigestDataCache[0]))->getApduRequest());

                        /*
                         * Build and append Digest Update commands
                         *
                         * The first command is at index 1.
                         */
                        for (int i = 1; i < poDigestDataCache.size(); i++) {
                            samApduRequestList.push_back(std::make_shared<org::eclipse::keyple::calypso::command::sam::builder::session::DigestUpdateCmdBuild>(samRevision, encryption, poDigestDataCache[i])
                                                    .getApduRequest());
                        }

                        /*
                         * Build and append Digest Close command
                         */
                        samApduRequestList.push_back(((std::make_shared<org::eclipse::keyple::calypso::command::sam::builder::session::DigestCloseCmdBuild>(samRevision, poRevision.equals(PoRevision::REV3_2) ? SIGNATURE_LENGTH_REV32 : SIGNATURE_LENGTH_REV_INF_32))->getApduRequest()));


                        return std::make_shared<SeRequest>(samApduRequestList, ChannelState::KEEP_OPEN);
                    }

                    PoTransaction::AnticipatedResponseBuilder::CommandResponse::CommandResponse(std::shared_ptr<ApduRequest> apduRequest, std::shared_ptr<ApduResponse> apduResponse) : apduRequest(apduRequest), apduResponse(apduResponse) {
                    }

                    std::shared_ptr<ApduRequest> PoTransaction::AnticipatedResponseBuilder::CommandResponse::getApduRequest() {
                        return apduRequest;
                    }

                    std::shared_ptr<ApduResponse> PoTransaction::AnticipatedResponseBuilder::CommandResponse::getApduResponse() {
                        return apduResponse;
                    }

std::unordered_map<Byte, std::shared_ptr<CommandResponse>> PoTransaction::AnticipatedResponseBuilder::sfiCommandResponseHashMap;

                    void PoTransaction::AnticipatedResponseBuilder::storeCommandResponse(std::vector<std::shared_ptr<PoSendableInSession>> &poSendableInSessions, std::vector<std::shared_ptr<ApduRequest>> &apduRequests, std::vector<std::shared_ptr<ApduResponse>> &apduResponses, Boolean skipFirstItem) {
                        if (poSendableInSessions.size() > 0) {
                            /*
                             * Store Read Records' requests and responses for later use to build anticipated
                             * responses.
                             */
                            std::vector<std::shared_ptr<ApduRequest>>::const_iterator apduRequestIterator = apduRequests.begin();
                            std::vector<std::shared_ptr<ApduResponse>>::const_iterator apduResponseIterator = apduResponses.begin();
                            if (skipFirstItem) {
                                /* case of processAtomicOpening */
//JAVA TO C++ CONVERTER TODO TASK: Java iterators are only converted within the context of 'while' and 'for' loops:
                                apduRequestIterator.next();
//JAVA TO C++ CONVERTER TODO TASK: Java iterators are only converted within the context of 'while' and 'for' loops:
                                apduResponseIterator.next();
                            }
                            /* Iterate over the poCommandsInsideSession list */
                            for (auto poSendableInSession : poSendableInSessions) {
                                if (std::dynamic_pointer_cast<ReadRecordsCmdBuild>(poSendableInSession) != nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: Java iterators are only converted within the context of 'while' and 'for' loops:
                                    std::shared_ptr<ApduRequest> apduRequest = apduRequestIterator.next();
                                    char sfi = static_cast<char>((apduRequest->getBytes()[OFFSET_P2] >> 3) & 0x1F);
//JAVA TO C++ CONVERTER TODO TASK: Java iterators are only converted within the context of 'while' and 'for' loops:
                                    sfiCommandResponseHashMap.emplace(sfi, std::make_shared<CommandResponse>(apduRequest, apduResponseIterator.next()));
                                }
                                else {
//JAVA TO C++ CONVERTER TODO TASK: Java iterators are only converted within the context of 'while' and 'for' loops:
                                    apduRequestIterator.next();
//JAVA TO C++ CONVERTER TODO TASK: Java iterators are only converted within the context of 'while' and 'for' loops:
                                    apduResponseIterator.next();
                                }
                            }
                        }
                    }

                    std::vector<std::shared_ptr<ApduResponse>> PoTransaction::AnticipatedResponseBuilder::getResponses(std::vector<std::shared_ptr<PoModificationCommand>> &poModificationCommands) throw(KeypleCalypsoSecureSessionException) {
                        std::vector<std::shared_ptr<ApduResponse>> apduResponses;
                        if (poModificationCommands.size() > 0) {
                            for (auto poModificationCommand : poModificationCommands) {
                                if (std::dynamic_pointer_cast<DecreaseCmdBuild>(poModificationCommand) != nullptr || std::dynamic_pointer_cast<IncreaseCmdBuild>(poModificationCommand) != nullptr) {
                                    /* response = NNNNNN9000 */
                                    std::vector<char> modCounterApduRequest = (std::static_pointer_cast<PoCommandBuilder>(poModificationCommand))->getApduRequest()->getBytes();
                                    /* Retrieve SFI from the current Decrease command */
                                    char sfi = static_cast<char>((modCounterApduRequest[OFFSET_P2] >> 3) & 0x1F);
                                    /*
                                     * Look for the counter value in the stored records. Only the first
                                     * occurrence of the SFI is taken into account. We assume here that the
                                     * record number is always 1.
                                     */
                                    std::shared_ptr<CommandResponse> commandResponse = sfiCommandResponseHashMap[sfi];
                                    if (commandResponse != nullptr) {
                                        char counterNumber = modCounterApduRequest[OFFSET_P1];
                                        /*
                                         * The record containing the counters is structured as follow:
                                         * AAAAAAABBBBBBCCCCCC...XXXXXX each counter being a 3-byte unsigned
                                         * number. Convert the 3-byte block indexed by the counter number to an
                                         * int.
                                         */
                                        int currentCounterValue = ByteBuffer::wrap(commandResponse->getApduResponse()->getBytes()).order(ByteOrder::BIG_ENDIAN).getInt((counterNumber - 1) * 3) >> 8;
                                        /* Extract the add or subtract value from the modification request */
                                        int addSubtractValue = ByteBuffer::wrap(modCounterApduRequest).order(ByteOrder::BIG_ENDIAN).getInt(OFFSET_DATA) >> 8;
                                        /* Build the response */
                                        std::vector<char> response(5);
                                        int newCounterValue;
                                        if (std::dynamic_pointer_cast<DecreaseCmdBuild>(poModificationCommand) != nullptr) {
                                            newCounterValue = currentCounterValue - addSubtractValue;
                                        }
                                        else {
                                            newCounterValue = currentCounterValue + addSubtractValue;
                                        }
                                        response[0] = static_cast<char>((newCounterValue & 0x00FF0000) >> 16);
                                        response[1] = static_cast<char>((newCounterValue & 0x0000FF00) >> 8);
                                        response[2] = static_cast<char>((newCounterValue & 0x000000FF) >> 0);
                                        response[3] = static_cast<char>(0x90);
                                        response[4] = static_cast<char>(0x00);
                                        apduResponses.push_back(std::make_shared<ApduResponse>(response, nullptr));
                                        if (logger->isDebugEnabled()) {
                                            logger->debug("Anticipated response. COMMAND = {}, SFI = {}, COUNTERVALUE = {}, DECREMENT = {}, NEWVALUE = {} ", (std::dynamic_pointer_cast<DecreaseCmdBuild>(poModificationCommand) != nullptr) ? "Decrease" : "Increase", sfi, currentCounterValue, addSubtractValue, newCounterValue);
                                        }
                                    }
                                    else {
                                        throw std::make_shared<KeypleCalypsoSecureSessionException>("Anticipated response. COMMAND = " + ((std::dynamic_pointer_cast<DecreaseCmdBuild>(poModificationCommand) != nullptr) ? "Decrease" : "Increase") + ". Unable to determine anticipated counter value. SFI = " + std::to_string(sfi), (std::static_pointer_cast<PoCommandBuilder>(poModificationCommand))->getApduRequest(), nullptr);
                                    }
                                }
                                else {
                                    /* Append/Update/Write Record: response = 9000 */
                                    apduResponses.push_back(std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("9000"), nullptr));
                                }
                            }
                        }
                        return apduResponses;
                    }

                    bool PoTransaction::processOpening(ModificationMode modificationMode, SessionAccessLevel accessLevel, char openingSfiToSelect, char openingRecordNumberToRead) throw(KeypleReaderException) {
                        currentModificationMode = modificationMode;
                        currentAccessLevel = accessLevel;
                        char localOpeningRecordNumberToRead = openingRecordNumberToRead;
                        bool poProcessSuccess = true;
                        /*
                         * Iterator to keep the progress in updating the parsers from the list of prepared commands
                         */
                        std::vector<std::shared_ptr<AbstractApduResponseParser>>::const_iterator apduResponseParserIterator = poResponseParserList.begin();
                        std::vector<std::shared_ptr<PoSendableInSession>> poAtomicCommandBuilderList;
                        for (auto poCommandBuilderElement : poCommandBuilderList) {
                            if (!(std::dynamic_pointer_cast<PoModificationCommand>(poCommandBuilderElement) != nullptr)) {
                                /* This command does not affect the PO modifications buffer */
                                poAtomicCommandBuilderList.push_back(poCommandBuilderElement);
                            }
                            else {
                                /* This command affects the PO modifications buffer */
                                if (willOverflowBuffer(std::static_pointer_cast<PoModificationCommand>(poCommandBuilderElement))) {
                                    if (currentModificationMode == ModificationMode::ATOMIC) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                        throw std::make_shared<IllegalStateException>("ATOMIC mode error! This command would overflow the PO modifications buffer: " + poCommandBuilderElement->toString());
                                    }
                                    std::shared_ptr<SeResponse> seResponseOpening = processAtomicOpening(currentAccessLevel, openingSfiToSelect, localOpeningRecordNumberToRead, poAtomicCommandBuilderList);

                                    /*
                                     * inhibit record reading for next round, keep file selection (TODO check this)
                                     */
                                    localOpeningRecordNumberToRead = static_cast<char>(0x00);

                                    if (!updateParsersWithResponses(seResponseOpening, apduResponseParserIterator)) {
                                        poProcessSuccess = false;
                                    }
                                    /*
                                     * Closes the session, resets the modifications buffer counters for the next
                                     * round (set the contact mode to avoid the transmission of the ratification)
                                     */
                                    processAtomicClosing(nullptr, TransmissionMode::CONTACTS, ChannelState::KEEP_OPEN);
                                    resetModificationsBufferCounter();
                                    /*
                                     * Clear the list and add the command that did not fit in the PO modifications
                                     * buffer. We also update the usage counter without checking the result.
                                     */
                                    poAtomicCommandBuilderList.clear();
                                    poAtomicCommandBuilderList.push_back(poCommandBuilderElement);
                                    /*
                                     * just update modifications buffer usage counter, ignore result (always false)
                                     */
                                    willOverflowBuffer(std::static_pointer_cast<PoModificationCommand>(poCommandBuilderElement));
                                }
                                else {
                                    /*
                                     * The command fits in the PO modifications buffer, just add it to the list
                                     */
                                    poAtomicCommandBuilderList.push_back(poCommandBuilderElement);
                                }
                            }
                        }

                        std::shared_ptr<SeResponse> seResponseOpening = processAtomicOpening(currentAccessLevel, openingSfiToSelect, localOpeningRecordNumberToRead, poAtomicCommandBuilderList);
                        if (!updateParsersWithResponses(seResponseOpening, apduResponseParserIterator)) {
                            poProcessSuccess = false;
                        }

                        poCommandBuilderList.clear();
                        poResponseParserList.clear();
                        return poProcessSuccess;
                    }

                    bool PoTransaction::processPoCommands(ChannelState channelState) throw(KeypleReaderException) {
                        bool poProcessSuccess = true;
                        /*
                         * Iterator to keep the progress in updating the parsers from the list of prepared commands
                         */
                        std::vector<std::shared_ptr<AbstractApduResponseParser>>::const_iterator abstractApduResponseParserIterator = poResponseParserList.begin();
                        if (currentState == SessionState::SESSION_CLOSED) {
                            /* PO commands sent outside a Secure Session. No modifications buffer limitation. */
                            std::shared_ptr<SeResponse> seResponsePoCommands = processAtomicPoCommands(poCommandBuilderList, channelState);

                            if (!updateParsersWithResponses(seResponsePoCommands, abstractApduResponseParserIterator)) {
                                poProcessSuccess = false;
                            }
                        }
                        else {
                            /* A session is open, we have to care about the PO modifications buffer */
                            std::vector<std::shared_ptr<PoSendableInSession>> poAtomicCommandBuilderList;

                            for (auto poCommandBuilderElement : poCommandBuilderList) {
                                if (!(std::dynamic_pointer_cast<PoModificationCommand>(poCommandBuilderElement) != nullptr)) {
                                    /* This command does not affect the PO modifications buffer */
                                    poAtomicCommandBuilderList.push_back(poCommandBuilderElement);
                                }
                                else {
                                    /* This command affects the PO modifications buffer */
                                    if (willOverflowBuffer((std::static_pointer_cast<PoModificationCommand>(poCommandBuilderElement)))) {
                                        if (currentModificationMode == ModificationMode::ATOMIC) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                            throw std::make_shared<IllegalStateException>("ATOMIC mode error! This command would overflow the PO modifications buffer: " + poCommandBuilderElement->toString());
                                        }
                                        /*
                                         * The current command would overflow the modifications buffer in the PO. We
                                         * send the current commands and update the parsers. The parsers Iterator is
                                         * kept all along the process.
                                         */
                                        std::shared_ptr<SeResponse> seResponsePoCommands = processAtomicPoCommands(poAtomicCommandBuilderList, ChannelState::KEEP_OPEN);
                                        if (!updateParsersWithResponses(seResponsePoCommands, abstractApduResponseParserIterator)) {
                                            poProcessSuccess = false;
                                        }
                                        /*
                                         * Close the session and reset the modifications buffer counters for the
                                         * next round (set the contact mode to avoid the transmission of the
                                         * ratification)
                                         */
                                        processAtomicClosing(nullptr, TransmissionMode::CONTACTS, ChannelState::KEEP_OPEN);
                                        resetModificationsBufferCounter();
                                        /* We reopen a new session for the remaining commands to be sent */
                                        std::shared_ptr<SeResponse> seResponseOpening = processAtomicOpening(currentAccessLevel, static_cast<char>(0x00), static_cast<char>(0x00), nullptr);
                                        /*
                                         * Clear the list and add the command that did not fit in the PO
                                         * modifications buffer. We also update the usage counter without checking
                                         * the result.
                                         */
                                        poAtomicCommandBuilderList.clear();
                                        poAtomicCommandBuilderList.push_back(poCommandBuilderElement);
                                        /*
                                         * just update modifications buffer usage counter, ignore result (always
                                         * false)
                                         */
                                        willOverflowBuffer(std::static_pointer_cast<PoModificationCommand>(poCommandBuilderElement));
                                    }
                                    else {
                                        /*
                                         * The command fits in the PO modifications buffer, just add it to the list
                                         */
                                        poAtomicCommandBuilderList.push_back(poCommandBuilderElement);
                                    }
                                }
                            }
                            if (!poAtomicCommandBuilderList.empty()) {
                                std::shared_ptr<SeResponse> seResponsePoCommands = processAtomicPoCommands(poAtomicCommandBuilderList, ChannelState::KEEP_OPEN);
                                if (!updateParsersWithResponses(seResponsePoCommands, abstractApduResponseParserIterator)) {
                                    poProcessSuccess = false;
                                }
                            }
                            // TODO add session abort command if channel closing is requested
                            // if(channelState) {
                            // /* abort the PO session session */
                            // }
                        }
                        /* clean up global lists */
                        poCommandBuilderList.clear();
                        poResponseParserList.clear();
                        return poProcessSuccess;
                    }

                    bool PoTransaction::processClosing(TransmissionMode transmissionMode, ChannelState channelState) throw(KeypleReaderException) {
                        bool poProcessSuccess = true;
                        bool atLeastOneReadCommand = false;
                        bool sessionPreviouslyClosed = false;
                        std::vector<std::shared_ptr<PoModificationCommand>> poModificationCommandList;
                        std::vector<std::shared_ptr<AbstractApduResponseParser>>::const_iterator abstractApduResponseParserIterator = poResponseParserList.begin();
                        std::vector<std::shared_ptr<PoModificationCommand>> poAtomicCommandBuilderList;
                        std::shared_ptr<SeResponse> seResponseClosing;
                        for (auto poCommandBuilderElement : poCommandBuilderList) {
                            if (!(std::dynamic_pointer_cast<PoModificationCommand>(poCommandBuilderElement) != nullptr)) {
                                /*
                                 * This command does not affect the PO modifications buffer. We will call
                                 * processPoCommands first
                                 */
                                poAtomicCommandBuilderList.push_back(std::static_pointer_cast<PoModificationCommand>(poCommandBuilderElement));
                                atLeastOneReadCommand = true;
                            }
                            else {
                                /* This command affects the PO modifications buffer */
                                if (willOverflowBuffer(std::static_pointer_cast<PoModificationCommand>(poCommandBuilderElement))) {
                                    if (currentModificationMode == ModificationMode::ATOMIC) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                        throw std::make_shared<IllegalStateException>("ATOMIC mode error! This command would overflow the PO modifications buffer: " + poCommandBuilderElement->toString());
                                    }
                                    /*
                                     * Reopen a session with the same access level if it was previously closed in
                                     * this current processClosing
                                     */
                                    if (sessionPreviouslyClosed) {
                                        processAtomicOpening(currentAccessLevel, static_cast<char>(0x00), static_cast<char>(0x00), nullptr);
                                    }

                                    /*
                                     * If at least one non-modifying was prepared, we use processAtomicPoCommands
                                     * instead of processAtomicClosing to send the list
                                     */
                                    if (atLeastOneReadCommand) {
                                        std::vector<std::shared_ptr<PoSendableInSession>> poSendableInSessionList;
                                        for (auto command : poAtomicCommandBuilderList) {
                                            poSendableInSessionList.push_back(std::static_pointer_cast<PoSendableInSession>(command));
                                        }
                                        seResponseClosing = processAtomicPoCommands(poSendableInSessionList, ChannelState::KEEP_OPEN);
                                        atLeastOneReadCommand = false;
                                    }
                                    else {
                                        /* All commands in the list are 'modifying' */
                                        seResponseClosing = processAtomicClosing(poAtomicCommandBuilderList, TransmissionMode::CONTACTS, ChannelState::KEEP_OPEN);
                                        resetModificationsBufferCounter();
                                        sessionPreviouslyClosed = true;
                                    }

                                    std::vector<std::shared_ptr<AbstractApduResponseParser>>::const_iterator apduResponseParserIterator = poResponseParserList.begin();
                                    if (!updateParsersWithResponses(seResponseClosing, apduResponseParserIterator)) {
                                        poProcessSuccess = false;
                                    }
                                    /*
                                     * Clear the list and add the command that did not fit in the PO modifications
                                     * buffer. We also update the usage counter without checking the result.
                                     */
                                    poAtomicCommandBuilderList.clear();
                                    poAtomicCommandBuilderList.push_back(std::static_pointer_cast<PoModificationCommand>(poCommandBuilderElement));
                                    /*
                                     * just update modifications buffer usage counter, ignore result (always false)
                                     */
                                    willOverflowBuffer(std::static_pointer_cast<PoModificationCommand>(poCommandBuilderElement));
                                }
                                else {
                                    /*
                                     * The command fits in the PO modifications buffer, just add it to the list
                                     */
                                    poAtomicCommandBuilderList.push_back(std::static_pointer_cast<PoModificationCommand>(poCommandBuilderElement));
                                }
                            }
                        }
                        if (sessionPreviouslyClosed) {
                            /*
                             * Reopen if needed, to close the session with the requested conditions
                             * (CommunicationMode and channelState)
                             */
                            processAtomicOpening(currentAccessLevel, static_cast<char>(0x00), static_cast<char>(0x00), nullptr);
                        }

                        /* Finally, close the session as requested */
                        seResponseClosing = processAtomicClosing(poAtomicCommandBuilderList, transmissionMode, channelState);

                        /* Update parsers */
                        if (!updateParsersWithResponses(seResponseClosing, abstractApduResponseParserIterator)) {
                            poProcessSuccess = false;
                        }

                        /* clean up global lists */
                        poCommandBuilderList.clear();
                        poResponseParserList.clear();
                        return poProcessSuccess;
                    }

                    bool PoTransaction::updateParsersWithResponses(std::shared_ptr<SeResponse> seResponse, std::shared_ptr<Iterator<std::shared_ptr<AbstractApduResponseParser>>> parserIterator) {
                        bool allSuccessfulCommands = true;
                        /* double loop to set apdu responses to corresponding parsers */
                        for (auto apduResponse : seResponse->getApduResponses()) {
                            if (!parserIterator->hasNext()) {
                                throw std::make_shared<IllegalStateException>("Parsers list and responses list mismatch! ");
                            }
                            parserIterator->next().setApduResponse(apduResponse);
                            if (!apduResponse->isSuccessful()) {
                                allSuccessfulCommands = false;
                            }
                        }
                        return allSuccessfulCommands;
                    }

                    bool PoTransaction::willOverflowBuffer(std::shared_ptr<PoModificationCommand> modificationCommand) {
                        bool willOverflow = false;
                        if (modificationsCounterIsInBytes) {
                            int bufferRequirement = (std::static_pointer_cast<AbstractApduCommandBuilder>(modificationCommand))->getApduRequest()->getBytes()[OFFSET_Lc] + 6;

                            if (modificationsCounter - bufferRequirement > 0) {
                                modificationsCounter = modificationsCounter - bufferRequirement;
                            }
                            else {
                                if (logger->isTraceEnabled()) {
                                    logger->trace("Modifications buffer overflow! BYTESMODE, CURRENTCOUNTER = {}, REQUIREMENT = {}", modificationsCounter, bufferRequirement);
                                }
                                willOverflow = true;
                            }
                        }
                        else {
                            if (modificationsCounter > 0) {
                                modificationsCounter--;
                            }
                            else {
                                if (logger->isTraceEnabled()) {
                                    logger->trace("Modifications buffer overflow! COMMANDSMODE, CURRENTCOUNTER = {}, REQUIREMENT = {}", modificationsCounter, 1);
                                }
                                willOverflow = true;
                            }
                        }
                        return willOverflow;
                    }

                    void PoTransaction::resetModificationsBufferCounter() {
                        if (logger->isTraceEnabled()) {
                            logger->trace("Modifications buffer counter reset: PREVIOUSVALUE = {}, NEWVALUE = {}", modificationsCounter, modificationsCounterMax);
                        }
                        modificationsCounter = modificationsCounterMax;
                    }

                    std::shared_ptr<ReadRecordsRespPars> PoTransaction::prepareReadRecordsCmdInternal(char sfi, ReadDataStructure readDataStructureEnum, char firstRecordNumber, int expectedLength, const std::string &extraInfo) {

                        /*
                         * the readJustOneRecord flag is set to false only in case of multiple read records, in all
                         * other cases it is set to true
                         */
                        bool readJustOneRecord = !(readDataStructureEnum == readDataStructureEnum.MULTIPLE_RECORD_DATA);

                        poCommandBuilderList.push_back(std::make_shared<ReadRecordsCmdBuild>(calypsoPo->getPoClass(), sfi, firstRecordNumber, readJustOneRecord, static_cast<char>(expectedLength), extraInfo));

                        std::shared_ptr<ReadRecordsRespPars> poResponseParser = std::make_shared<ReadRecordsRespPars>(firstRecordNumber, readDataStructureEnum);

                        poResponseParserList.push_back(poResponseParser);

                        return poResponseParser;
                    }

                    std::shared_ptr<ReadRecordsRespPars> PoTransaction::prepareReadRecordsCmd(char sfi, ReadDataStructure readDataStructureEnum, char firstRecordNumber, int expectedLength, const std::string &extraInfo) {
                        if (expectedLength < 1 || expectedLength > 250) {
                            throw std::invalid_argument("Bad length.");
                        }
                        return prepareReadRecordsCmdInternal(sfi, readDataStructureEnum, firstRecordNumber, expectedLength, extraInfo);
                    }

                    std::shared_ptr<ReadRecordsRespPars> PoTransaction::prepareReadRecordsCmd(char sfi, ReadDataStructure readDataStructureEnum, char firstRecordNumber, const std::string &extraInfo) {
                        if (poReader->getTransmissionMode() == TransmissionMode::CONTACTS) {
                            throw std::invalid_argument("In contacts mode, the expected length must be specified.");
                        }
                        return prepareReadRecordsCmdInternal(sfi, readDataStructureEnum, firstRecordNumber, 0, extraInfo);
                    }

                    std::shared_ptr<AppendRecordRespPars> PoTransaction::prepareAppendRecordCmd(char sfi, std::vector<char> &newRecordData, const std::string &extraInfo) {
                        poCommandBuilderList.push_back(std::make_shared<AppendRecordCmdBuild>(calypsoPo->getPoClass(), sfi, newRecordData, extraInfo));

                        std::shared_ptr<AppendRecordRespPars> poResponseParser = std::make_shared<AppendRecordRespPars>();

                        poResponseParserList.push_back(poResponseParser);

                        return poResponseParser;
                    }

                    std::shared_ptr<UpdateRecordRespPars> PoTransaction::prepareUpdateRecordCmd(char sfi, char recordNumber, std::vector<char> &newRecordData, const std::string &extraInfo) {
                        poCommandBuilderList.push_back(std::make_shared<UpdateRecordCmdBuild>(calypsoPo->getPoClass(), sfi, recordNumber, newRecordData, extraInfo));

                        std::shared_ptr<UpdateRecordRespPars> poResponseParser = std::make_shared<UpdateRecordRespPars>();

                        poResponseParserList.push_back(poResponseParser);

                        return poResponseParser;
                    }

                    std::shared_ptr<IncreaseRespPars> PoTransaction::prepareIncreaseCmd(char sfi, char counterNumber, int incValue, const std::string &extraInfo) {
                        poCommandBuilderList.push_back(std::make_shared<IncreaseCmdBuild>(calypsoPo->getPoClass(), sfi, counterNumber, incValue, extraInfo));

                        std::shared_ptr<IncreaseRespPars> poResponseParser = std::make_shared<IncreaseRespPars>();

                        poResponseParserList.push_back(poResponseParser);

                        return poResponseParser;
                    }

                    std::shared_ptr<DecreaseRespPars> PoTransaction::prepareDecreaseCmd(char sfi, char counterNumber, int decValue, const std::string &extraInfo) {
                        poCommandBuilderList.push_back(std::make_shared<DecreaseCmdBuild>(calypsoPo->getPoClass(), sfi, counterNumber, decValue, extraInfo));
                        std::shared_ptr<DecreaseRespPars> poResponseParser = std::make_shared<DecreaseRespPars>();

                        poResponseParserList.push_back(poResponseParser);

                        return poResponseParser;
                    }
                }
            }
        }
    }
}
