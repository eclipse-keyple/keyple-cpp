
#include "AbstractApduCommandBuilder.h"
#include "AbstractApduResponseParser.h"
#include "AbstractOpenSessionCmdBuild.h"
#include "AbstractOpenSessionRespPars.h"
#include "AppendRecordCmdBuild.h"
#include "AppendRecordRespPars.h"
#include "CalypsoPo.h"
#include "CloseSessionCmdBuild.h"
#include "CloseSessionRespPars.h"
#include "DecreaseCmdBuild.h"
#include "DigestAuthenticateCmdBuild.h"
#include "DigestAuthenticateRespPars.h"
#include "DigestCloseCmdBuild.h"
#include "DigestCloseRespPars.h"
#include "DigestInitCmdBuild.h"
#include "DigestUpdateCmdBuild.h"
#include "KeypleReaderException.h"
#include "KeypleCalypsoSecureSessionException.h"
#include "KeypleCalypsoSecureSessionUnauthorizedKvcException.h"
#include "IncreaseCmdBuild.h"
#include "PoCommandBuilder.h"
#include "PoCustomReadCommandBuilder.h"
#include "PoTransaction.h"
#include "ReadDataStructure.h"
#include "ReadRecordsCmdBuild.h"
#include "SamCommandBuilder.h"
#include "SamGetChallengeCmdBuild.h"
#include "SamSendableInSession.h"
#include "SamGetChallengeRespPars.h"
#include "SelectDiversifierCmdBuild.h"
#include "SendableInSession.h"
#include "UpdateRecordCmdBuild.h"
#include "UpdateRecordRespPars.h"

/* Core */
#include "ByteArrayUtils.h"
#include "ProxyReader.h"
#include "SeReader.h"

/* Common */
#include "Arrays.h"
#include "stringhelper.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace transaction {

                    using AbstractApduCommandBuilder                         = org::eclipse::keyple::command::AbstractApduCommandBuilder;
                    using AbstractApduResponseParser                         = org::eclipse::keyple::command::AbstractApduResponseParser;
                    using AbstractOpenSessionCmdBuild                        = org::eclipse::keyple::calypso::command::po::builder::session::AbstractOpenSessionCmdBuild;
                    using AbstractOpenSessionRespPars                        = org::eclipse::keyple::calypso::command::po::parser::session::AbstractOpenSessionRespPars;
                    using AppendRecordCmdBuild                               = org::eclipse::keyple::calypso::command::po::builder::AppendRecordCmdBuild;
                    using AppendRecordRespPars                               = org::eclipse::keyple::calypso::command::po::parser::AppendRecordRespPars;
                    using ByteArrayUtils                                     = org::eclipse::keyple::util::ByteArrayUtils;
                    using ChannelState                                       = org::eclipse::keyple::seproxy::ChannelState;
                    using CloseSessionCmdBuild                               = org::eclipse::keyple::calypso::command::po::builder::session::CloseSessionCmdBuild;
                    using CloseSessionRespPars                               = org::eclipse::keyple::calypso::command::po::parser::session::CloseSessionRespPars;
                    using DecreaseCmdBuild                                   = org::eclipse::keyple::calypso::command::po::builder::DecreaseCmdBuild;
                    using DigestAuthenticateCmdBuild                         = org::eclipse::keyple::calypso::command::sam::builder::session::DigestAuthenticateCmdBuild;
                    using DigestAuthenticateRespPars                         = org::eclipse::keyple::calypso::command::sam::parser::session::DigestAuthenticateRespPars;
                    using DigestCloseCmdBuild                                = org::eclipse::keyple::calypso::command::sam::builder::session::DigestCloseCmdBuild;
                    using DigestCloseRespPars                                = org::eclipse::keyple::calypso::command::sam::parser::session::DigestCloseRespPars;
                    using DigestInitCmdBuild                                 = org::eclipse::keyple::calypso::command::sam::builder::session::DigestInitCmdBuild;
                    using DigestUpdateCmdBuild                               = org::eclipse::keyple::calypso::command::sam::builder::session::DigestUpdateCmdBuild;
                    using IncreaseCmdBuild                                   = org::eclipse::keyple::calypso::command::po::builder::IncreaseCmdBuild;
                    using KeypleReaderException                              = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using KeypleCalypsoSecureSessionException                = org::eclipse::keyple::calypso::transaction::exception::KeypleCalypsoSecureSessionException;
                    using KeypleCalypsoSecureSessionUnauthorizedKvcException = org::eclipse::keyple::calypso::transaction::exception::KeypleCalypsoSecureSessionUnauthorizedKvcException;
                    using PoCommandBuilder                                   = org::eclipse::keyple::calypso::command::po::PoCommandBuilder;
                    using PoCustomReadCommandBuilder                         = org::eclipse::keyple::calypso::command::po::PoCustomReadCommandBuilder;
                    using ProxyReader                                        = org::eclipse::keyple::seproxy::message::ProxyReader;
                    using ReadDataStructure                                  = org::eclipse::keyple::calypso::command::po::parser::ReadDataStructure;
                    using ReadRecordsCmdBuild                                = org::eclipse::keyple::calypso::command::po::builder::ReadRecordsCmdBuild;
                    using SamCommandBuilder                                  = org::eclipse::keyple::calypso::command::sam::SamCommandBuilder;
                    using SamGetChallengeRespPars                            = org::eclipse::keyple::calypso::command::sam::parser::session::SamGetChallengeRespPars;
                    using SamRevision                                        = org::eclipse::keyple::calypso::command::sam::SamRevision;
                    using SamSendableInSession                               = org::eclipse::keyple::calypso::command::sam::SamSendableInSession;
                    using SelectDiversifierCmdBuild                          = org::eclipse::keyple::calypso::command::sam::builder::session::SelectDiversifierCmdBuild;
                    using SeReader                                           = org::eclipse::keyple::seproxy::SeReader;
                    using TransmissionMode                                   = org::eclipse::keyple::seproxy::protocol::TransmissionMode;
                    using UpdateRecordCmdBuild                               = org::eclipse::keyple::calypso::command::po::builder::UpdateRecordCmdBuild;
                    using UpdateRecordRespPars                               = org::eclipse::keyple::calypso::command::po::parser::UpdateRecordRespPars;

                    std::vector<char> PoTransaction::ratificationCmdApduLegacy = org::eclipse::keyple::util::ByteArrayUtils::fromHex("94B2000000");
                    std::vector<char> PoTransaction::ratificationCmdApdu = org::eclipse::keyple::util::ByteArrayUtils::fromHex("00B2000000");

                    PoTransaction::PoTransaction(std::shared_ptr<SeReader> poReader, std::shared_ptr<CalypsoPo> calypsoPO, std::shared_ptr<SeReader> samReader, std::shared_ptr<std::map<SamSettings, char>> samSetting)
                    : PoTransaction(poReader, calypsoPO) {


                        setSamSettings(samReader, samSetting);
                    }

                    PoTransaction::PoTransaction(std::shared_ptr<SeReader> poReader, std::shared_ptr<CalypsoPo> calypsoPO)
                    : poReader(std::dynamic_pointer_cast<ProxyReader>(poReader)), poCalypsoInstanceSerial(calypsoPO->getApplicationSerialNumber()), calypsoPo(calypsoPO)
                    {


                        poRevision = calypsoPO->getRevision();

                        poCalypsoInstanceAid = calypsoPO->getDfName();

                        modificationsCounterIsInBytes = calypsoPO->isModificationsCounterInBytes();

                        modificationsCounterMax = modificationsCounter = calypsoPO->getModificationsCounter();

                        /* Serial Number of the selected Calypso instance. */

                        currentState = SessionState::SESSION_CLOSED;
                    }

                    void PoTransaction::setSamSettings(std::shared_ptr<SeReader> samReader, std::shared_ptr<std::map<SamSettings, char>> samSetting)
                    {
                        this->samReader = std::dynamic_pointer_cast<ProxyReader>(samReader);

                        /* Initialize samSetting with provided settings */
                        if (samSetting != nullptr) {
                            this->samSetting->insert(samSetting->begin(), samSetting->end());
                        }

                        /* Just work mode: we make sure that all the necessary parameters exist at least. */
                        if (!this->samSetting->count(SamSettings::SAM_DEFAULT_KIF_PERSO)) {
                            this->samSetting->insert(std::pair<SamSettings, char>(SamSettings::SAM_DEFAULT_KIF_PERSO, DEFAULT_KIF_PERSO));
                        }
                        if (!this->samSetting->count(SamSettings::SAM_DEFAULT_KIF_LOAD)) {
                            this->samSetting->insert(std::pair<SamSettings, char>(SamSettings::SAM_DEFAULT_KIF_LOAD, DEFAULT_KIF_LOAD));
                        }
                        if (!this->samSetting->count(SamSettings::SAM_DEFAULT_KIF_DEBIT)) {
                            this->samSetting->insert(std::pair<SamSettings, char>(SamSettings::SAM_DEFAULT_KIF_DEBIT, DEFAULT_KIF_DEBIT));
                        }
                        if (!this->samSetting->count(SamSettings::SAM_DEFAULT_KEY_RECORD_NUMBER)) {
                            this->samSetting->insert(std::pair<SamSettings, char>(SamSettings::SAM_DEFAULT_KEY_RECORD_NUMBER, DEFAULT_KEY_RECORD_NUMER));
                        }

                        logger->debug("Contructor => SAMSETTING = {}", this->samSetting);

                        samSettingsDefined = true;
                    }

                    void PoTransaction::setAuthorizedKvcList(std::vector<char> &authorizedKvcList) {
                        this->authorizedKvcList = authorizedKvcList;
                    }

                    bool PoTransaction::isSamSettingsDefined() {
                        return samSettingsDefined;
                    }

                    std::shared_ptr<SeResponse> PoTransaction::processAtomicOpening(SessionAccessLevel accessLevel, char openingSfiToSelect, char openingRecordNumberToRead, std::vector<std::shared_ptr<PoSendableInSession>> &poCommandsInsideSession)
                    {

                        /*
                         * counts 'select diversifier' and 'get challenge' commands. At least get challenge is
                         * present
                         */
                        int numberOfSamCmd = 1;

                        /* SAM ApduRequest List to hold Select Diversifier and Get Challenge commands */
                        std::vector<std::shared_ptr<ApduRequest>> samApduRequestList;

                        if (logger->isDebugEnabled()) {
                            logger->debug("processAtomicOpening => Identification: DFNAME = %s, SERIALNUMBER = %s\n",
                                          org::eclipse::keyple::util::ByteArrayUtils::toHex(poCalypsoInstanceAid),
                                          org::eclipse::keyple::util::ByteArrayUtils::toHex(poCalypsoInstanceSerial));
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
                        char challengeLength = poRevision == PoRevision::REV3_2 ? CHALLENGE_LENGTH_REV32 : CHALLENGE_LENGTH_REV_INF_32;

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
                            std::vector<std::shared_ptr<ApduResponse>> emptyVector;
                            std::vector<std::shared_ptr<ApduRequest>> req = samSeRequest->getApduRequests();
                            throw KeypleCalypsoSecureSessionException("Null response received", KeypleCalypsoSecureSessionException::Type::SAM, req, emptyVector);
                        }

                        logger->debug("processAtomicOpening => identification: SAMSERESPONSE = {}", samSeResponse);

                        std::vector<std::shared_ptr<ApduResponse>> samApduResponseList = samSeResponse->getApduResponses();
                        std::vector<char> sessionTerminalChallenge;

                        if ((int)samApduResponseList.size() == numberOfSamCmd &&
                            samApduResponseList[numberOfSamCmd - 1]->isSuccessful() &&
                            (int)samApduResponseList[numberOfSamCmd - 1]->getDataOut().size() == challengeLength) {
                            std::shared_ptr<SamGetChallengeRespPars> samChallengePars = std::make_shared<SamGetChallengeRespPars>(samApduResponseList[numberOfSamCmd - 1]);
                            sessionTerminalChallenge = samChallengePars->getChallenge();
                            if (logger->isDebugEnabled()) {
                                logger->debug("processAtomicOpening => identification: TERMINALCHALLENGE = %s", org::eclipse::keyple::util::ByteArrayUtils::toHex(sessionTerminalChallenge));
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
                            /*
                             * poCommandsInsideSession is of type:
                             *   std::vector<std::shared_ptr<PoSendableInSession>>
                             * with:
                             *   PoSendableInSession : public org::eclipse::keyple::calypso::command::SendableInSession<std::shared_ptr<PoCommandBuilder>>
                             * which makes it somehow:
                             *   std::vector<std::shared_ptr<org::eclipse::keyple::calypso::command::SendableInSession<std::shared_ptr<PoCommandBuilder>>>>
                             * 
                             * getApduRequestsToSendInSession is expecting:
                             *   std::vector<std::shared_ptr<org::eclipse::keyple::calypso::command::SendableInSession<T>>> &poOrSamCommandsInsideSession
                             */
                            std::vector<std::shared_ptr<org::eclipse::keyple::calypso::command::SendableInSession<PoCommandBuilder>>> vec(poCommandsInsideSession.begin(), poCommandsInsideSession.end());
                            std::vector<std::shared_ptr<ApduRequest>> apduReq = this->getApduRequestsToSendInSession(vec);
                            poApduRequestList.insert(std::end(poApduRequestList), std::begin(apduReq), std::end(apduReq));
                        }

                        /* Create a SeRequest from the ApduRequest list, PO AID as Selector, keep channel open */
                        std::shared_ptr<SeRequest> poSeRequest = std::make_shared<SeRequest>(poApduRequestList, ChannelState::KEEP_OPEN);

                        logger->debug("processAtomicOpening => opening:  POSEREQUEST = {}", poSeRequest);

                        /* Transmit the commands to the PO */
                        std::shared_ptr<SeResponse> poSeResponse = poReader->transmit(poSeRequest);

                        logger->debug("processAtomicOpening => opening:  POSERESPONSE = {}", poSeResponse);

                        if (poSeResponse == nullptr) {
                            std::vector<std::shared_ptr<ApduResponse>> emptyVector;
                            std::vector<std::shared_ptr<ApduRequest>> req = poSeRequest->getApduRequests();
                            throw KeypleCalypsoSecureSessionException("Null response received", KeypleCalypsoSecureSessionException::Type::PO, req, emptyVector);
                        }

                        if (poSeResponse->wasChannelPreviouslyOpen() == false) {
                            std::vector<std::shared_ptr<ApduResponse>> emptyVector;
                            std::vector<std::shared_ptr<ApduRequest>> req = poSeRequest->getApduRequests();
                            throw KeypleCalypsoSecureSessionException("The logical channel was not open", KeypleCalypsoSecureSessionException::Type::PO, req, emptyVector);
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
                            logger->debug("processAtomicOpening => opening: CARDCHALLENGE = {}, POKIF = {}, POKVC = {}", ByteArrayUtils::toHex(sessionCardChallenge), StringHelper::formatSimple("%02X", poKif), StringHelper::formatSimple("%02X", poKvc));
                        }

                        if (authorizedKvcList.size() > 0 && std::find(authorizedKvcList.begin(), authorizedKvcList.end(), poKvc) != authorizedKvcList.end()) {
                            throw KeypleCalypsoSecureSessionUnauthorizedKvcException(StringHelper::formatSimple("PO KVC = %02X", poKvc));
                        }

                        char kif;
                        if (poKif == KIF_UNDEFINED) {
                            switch (accessLevel) {
                                case org::eclipse::keyple::calypso::transaction::PoTransaction::SessionAccessLevel::SESSION_LVL_PERSO:
                                    kif = samSetting->find(SamSettings::SAM_DEFAULT_KIF_PERSO)->second;
                                    break;
                                case org::eclipse::keyple::calypso::transaction::PoTransaction::SessionAccessLevel::SESSION_LVL_LOAD:
                                    kif = samSetting->find(SamSettings::SAM_DEFAULT_KIF_LOAD)->second;
                                    break;
                                case org::eclipse::keyple::calypso::transaction::PoTransaction::SessionAccessLevel::SESSION_LVL_DEBIT:
                                default:
                                    kif = samSetting->find(SamSettings::SAM_DEFAULT_KIF_DEBIT)->second;
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
                        std::vector<char> dataOut = poApduResponseList[0]->getDataOut();
                        DigestProcessor::initialize(poRevision, samRevision, false, false, poRevision == (PoRevision::REV3_2), samSetting->find(SamSettings::SAM_DEFAULT_KEY_RECORD_NUMBER)->second, kif, poKvc, dataOut);

                        /*
                         * Add all commands data to the digest computation. The first command in the list is the
                         * open secure session command. This command is not included in the digest computation, so
                         * we skip it and start the loop at index 1.
                         */
                        if ((poCommandsInsideSession.size() > 0) && !poCommandsInsideSession.empty()) {

                            for (int i = 1; i < (int)poApduRequestList.size(); i++) {
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

                    template<typename T>
                    std::vector<std::shared_ptr<ApduRequest>> PoTransaction::getApduRequestsToSendInSession(std::vector<std::shared_ptr<org::eclipse::keyple::calypso::command::SendableInSession<T>>> &poOrSamCommandsInsideSession) {
                        std::vector<std::shared_ptr<ApduRequest>> apduRequestList;
                        if ((int)poOrSamCommandsInsideSession.size() > 0) {
                            for (auto cmd : poOrSamCommandsInsideSession) {
                                apduRequestList.push_back(reinterpret_cast<PoCommandBuilder*>(cmd.get())->getApduRequest());
                            }
                        }
                        return apduRequestList;
                    }

                    std::shared_ptr<SeResponse> PoTransaction::processAtomicPoCommands(std::vector<std::shared_ptr<PoSendableInSession>> &poCommands, ChannelState channelState)
                    {

                        // Get PO ApduRequest List from PoSendableInSession List
                        /*
                         * poCommandsInsideSession is of type:
                         *   std::vector<std::shared_ptr<PoSendableInSession>>
                         * with:
                         *   PoSendableInSession : public org::eclipse::keyple::calypso::command::SendableInSession<std::shared_ptr<PoCommandBuilder>>
                         * which makes it somehow:
                         *   std::vector<std::shared_ptr<org::eclipse::keyple::calypso::command::SendableInSession<std::shared_ptr<PoCommandBuilder>>>>
                         * 
                         * getApduRequestsToSendInSession is expecting:
                         *   std::vector<std::shared_ptr<org::eclipse::keyple::calypso::command::SendableInSession<T>>> &poOrSamCommandsInsideSession
                         */
                        std::vector<std::shared_ptr<org::eclipse::keyple::calypso::command::SendableInSession<PoCommandBuilder>>> vec(poCommands.begin(), poCommands.end());     
                        std::vector<std::shared_ptr<ApduRequest>> poApduRequestList = this->getApduRequestsToSendInSession(vec);

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
                            std::vector<std::shared_ptr<ApduResponse>> emptyVector;
                            std::vector<std::shared_ptr<ApduRequest>> req = poSeRequest->getApduRequests();
                            throw KeypleCalypsoSecureSessionException("Null response received", KeypleCalypsoSecureSessionException::Type::PO, req, emptyVector);
                        }

                        if (poSeResponse->wasChannelPreviouslyOpen() == false) {
                            std::vector<std::shared_ptr<ApduResponse>> emptyVector;
                            std::vector<std::shared_ptr<ApduRequest>> req = poSeRequest->getApduRequests();
                            throw KeypleCalypsoSecureSessionException("The logical channel was not open", KeypleCalypsoSecureSessionException::Type::PO, req, emptyVector);
                        }

                        /* Retrieve and check the ApduResponses */
                        std::vector<std::shared_ptr<ApduResponse>> poApduResponseList = poSeResponse->getApduResponses();

                        /* Do some basic checks */
                        if (poApduRequestList.size() != poApduResponseList.size()) {
                            throw KeypleCalypsoSecureSessionException("Inconsistent requests and responses", KeypleCalypsoSecureSessionException::Type::PO, poApduRequestList, poApduResponseList);
                        }

                        for (auto apduR : poApduResponseList) {
                            if (!apduR->isSuccessful()) {
                                throw KeypleCalypsoSecureSessionException("Invalid response", KeypleCalypsoSecureSessionException::Type::PO, poApduRequestList, poApduResponseList);
                            }
                        }

                        /* Track Read Records for later use to build anticipated responses. */
                        AnticipatedResponseBuilder::storeCommandResponse(poCommands, poApduRequestList, poApduResponseList, false);

                        /*
                         * Add all commands data to the digest computation if this method is called within a Secure
                         * Session.
                         */
                        if (currentState == SessionState::SESSION_OPEN) {
                            for (int i = 0; i < (int)poApduRequestList.size(); i++) {
                                /*
                                 * Add requests and responses to the DigestProcessor
                                 */
                                DigestProcessor::pushPoExchangeData(poApduRequestList[i], poApduResponseList[i]);
                            }
                        }
                        return poSeResponse;
                    }

                    std::shared_ptr<SeResponse> PoTransaction::processSamCommands(std::vector<std::shared_ptr<SamSendableInSession>> &samCommands)
                    {

                        /*
                         * Init SAM ApduRequest List - for the first SAM exchange
                         *
                         * poCommandsInsideSession is of type:
                         *   std::vector<std::shared_ptr<PoSendableInSession>>
                         * with:
                         *   PoSendableInSession : public org::eclipse::keyple::calypso::command::SendableInSession<std::shared_ptr<PoCommandBuilder>>
                         * which makes it somehow:
                         *   std::vector<std::shared_ptr<org::eclipse::keyple::calypso::command::SendableInSession<std::shared_ptr<PoCommandBuilder>>>>
                         * 
                         * getApduRequestsToSendInSession is expecting:
                         *   std::vector<std::shared_ptr<org::eclipse::keyple::calypso::command::SendableInSession<T>>> &poOrSamCommandsInsideSession
                         */
                        std::vector<std::shared_ptr<org::eclipse::keyple::calypso::command::SendableInSession<SamCommandBuilder>>> vec(samCommands.begin(), samCommands.end());
                        std::vector<std::shared_ptr<ApduRequest>> samApduRequestList = this->getApduRequestsToSendInSession(vec);

                        /* SeRequest from the command list */
                        std::shared_ptr<SeRequest> samSeRequest = std::make_shared<SeRequest>(samApduRequestList, ChannelState::KEEP_OPEN);

                        logger->debug("processSamCommands => SAMSEREQUEST = {}", samSeRequest);

                        /* Transmit SeRequest and get SeResponse */
                        std::shared_ptr<SeResponse> samSeResponse = samReader->transmit(samSeRequest);

                        if (samSeResponse == nullptr) {
                            std::vector<std::shared_ptr<ApduResponse>> emptyVector;
                            std::vector<std::shared_ptr<ApduRequest>> req = samSeRequest->getApduRequests();
                            throw KeypleCalypsoSecureSessionException("Null response received", KeypleCalypsoSecureSessionException::Type::SAM, req, emptyVector);
                        }

                        if (currentState == SessionState::SESSION_OPEN && samSeResponse->wasChannelPreviouslyOpen() == false) {
                            std::vector<std::shared_ptr<ApduResponse>> emptyVector;
                            std::vector<std::shared_ptr<ApduRequest>> req = samSeRequest->getApduRequests();
                            throw KeypleCalypsoSecureSessionException("The logical channel was not open", KeypleCalypsoSecureSessionException::Type::SAM, req, emptyVector);
                        }
                        // TODO check if the wasChannelPreviouslyOpen should be done in the case where the session
                        // is closed

                        return samSeResponse;
                    }

                    std::shared_ptr<SeResponse> PoTransaction::processAtomicClosing(std::vector<std::shared_ptr<PoModificationCommand>> &poModificationCommands,
                                                                                    std::vector<std::shared_ptr<ApduResponse>> &poAnticipatedResponses,
                                                                                    TransmissionMode transmissionMode,
                                                                                    ChannelState channelState)
                    {
                        if (currentState != SessionState::SESSION_OPEN) {
                            throw IllegalStateException(StringHelper::formatSimple("Bad session state. Current: %d, expected: %d",
                                                                                   static_cast<int>(currentState),
                                                                                   static_cast<int>(SessionState::SESSION_OPEN)));
                        }

                        /*
                         * Get PO ApduRequest List from PoSendableInSession List - for the first PO exchange
                         * 
                         * poCommandsInsideSession is of type:
                         *   std::vector<std::shared_ptr<PoSendableInSession>>
                         * with:
                         *   PoSendableInSession : public org::eclipse::keyple::calypso::command::SendableInSession<std::shared_ptr<PoCommandBuilder>>
                         * which makes it somehow:
                         *   std::vector<std::shared_ptr<org::eclipse::keyple::calypso::command::SendableInSession<std::shared_ptr<PoCommandBuilder>>>>
                         * 
                         * getApduRequestsToSendInSession is expecting:
                         *   std::vector<std::shared_ptr<org::eclipse::keyple::calypso::command::SendableInSession<T>>> &poOrSamCommandsInsideSession
                         */
                        std::vector<std::shared_ptr<org::eclipse::keyple::calypso::command::SendableInSession<PoCommandBuilder>>> vec;
                        for (auto cmd : poModificationCommands)
                            vec.push_back(std::shared_ptr<org::eclipse::keyple::calypso::command::SendableInSession<PoCommandBuilder>>(reinterpret_cast<org::eclipse::keyple::calypso::command::SendableInSession<PoCommandBuilder>*>(cmd.get())));
                        std::vector<std::shared_ptr<ApduRequest>> poApduRequestList = this->getApduRequestsToSendInSession(vec);

                        /* Compute "anticipated" Digest Update (for optional poModificationCommands) */
                        if ((poModificationCommands.size() > 0) && !poApduRequestList.empty()) {
                            if (poApduRequestList.size() == poAnticipatedResponses.size()) {
                                /*
                                 * Add all commands data to the digest computation: commands and anticipated
                                 * responses.
                                 */
                                for (int i = 0; i < (int)poApduRequestList.size(); i++) {
                                    /*
                                     * Add requests and responses to the DigestProcessor
                                     */
                                    DigestProcessor::pushPoExchangeData(poApduRequestList[i], poAnticipatedResponses[i]);
                                }
                            }
                            else {
                                throw KeypleCalypsoSecureSessionException("Inconsistent requests and anticipated responses", KeypleCalypsoSecureSessionException::Type::PO, poApduRequestList, poAnticipatedResponses);
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
                            std::vector<std::shared_ptr<ApduResponse>> emptyVector;
                            std::vector<std::shared_ptr<ApduRequest>> req = samSeRequest->getApduRequests();
                            throw KeypleCalypsoSecureSessionException("Null response received", KeypleCalypsoSecureSessionException::Type::SAM, req, emptyVector);
                        }

                        if (samSeResponse->wasChannelPreviouslyOpen() == false) {
                            std::vector<std::shared_ptr<ApduResponse>> emptyVector;
                            std::vector<std::shared_ptr<ApduRequest>> req = samSeRequest->getApduRequests();
                            throw KeypleCalypsoSecureSessionException("The logical channel was not open", KeypleCalypsoSecureSessionException::Type::PO, req, emptyVector);
                        }

                        std::vector<std::shared_ptr<ApduResponse>> samApduResponseList = samSeResponse->getApduResponses();

                        for (int i = 0; i < (int)samApduResponseList.size(); i++) {
                            if (!samApduResponseList[i]->isSuccessful()) {

                                logger->debug("processAtomicClosing => command failure REQUEST = {}, RESPONSE = {}", samSeRequest->getApduRequests()[i], samApduResponseList[i]);
                                throw IllegalStateException("ProcessClosing command failure during digest computation process.");
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
                            logger->debug("processAtomicClosing => SIGNATURE = {}", org::eclipse::keyple::util::ByteArrayUtils::toHex(sessionTerminalSignature));
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
                        catch (KeypleReaderException &ex) {
                            poSeResponse = ex.getSeResponse();
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
                                ex.setSeResponse(poSeResponse);
                                throw KeypleReaderException("PO Reader Exception while closing Secure Session"); //, ex);
                            }
                        }

                        if (poSeResponse == nullptr) {
                            std::vector<std::shared_ptr<ApduResponse>> emptyVector;
                            std::vector<std::shared_ptr<ApduRequest>> req = poSeRequest->getApduRequests();
                            throw KeypleCalypsoSecureSessionException("Null response received", KeypleCalypsoSecureSessionException::Type::PO, req, emptyVector);
                        }

                        if (poSeResponse->wasChannelPreviouslyOpen() == false) {
                            std::vector<std::shared_ptr<ApduResponse>> emptyVector;
                            std::vector<std::shared_ptr<ApduRequest>> req = poSeRequest->getApduRequests();
                            throw KeypleCalypsoSecureSessionException("The logical channel was not open", KeypleCalypsoSecureSessionException::Type::PO, req, emptyVector);
                        }

                        logger->debug("processAtomicClosing => POSERESPONSE = %s", poSeResponse);

                        std::vector<std::shared_ptr<ApduResponse>> poApduResponseList = poSeResponse->getApduResponses();

                        // TODO add support of poRevision parameter to CloseSessionRespPars for REV2.4 PO CLAss byte
                        // before last if ratification, otherwise last one
                        std::shared_ptr<CloseSessionRespPars> poCloseSessionPars = std::make_shared<CloseSessionRespPars>(poApduResponseList[closeCommandIndex]);
                        if (!poCloseSessionPars->isSuccessful()) {
                            throw KeypleCalypsoSecureSessionException("Didn't get a signature", KeypleCalypsoSecureSessionException::Type::PO, poApduRequestList, poApduResponseList);
                        }

                        /* Check the PO signature part with the SAM */
                        /* Build and send SAM Digest Authenticate command */
                        std::vector<char> cmd = poCloseSessionPars->getSignatureLo();
                        std::shared_ptr<AbstractApduCommandBuilder> digestAuth = std::make_shared<DigestAuthenticateCmdBuild>(samRevision, cmd);

                        std::vector<std::shared_ptr<ApduRequest>> samApduRequestList;
                        samApduRequestList.push_back(digestAuth->getApduRequest());

                        samSeRequest = std::make_shared<SeRequest>(samApduRequestList, ChannelState::KEEP_OPEN);

                        logger->debug("PoTransaction.DigestProcessor => checkPoSignature: SAMREQUEST = {}", samSeRequest);

                        samSeResponse = samReader->transmit(samSeRequest);

                        logger->debug("PoTransaction.DigestProcessor => checkPoSignature: SAMRESPONSE = {}", samSeResponse);

                        if (samSeResponse == nullptr) {
                            std::vector<std::shared_ptr<ApduResponse>> emptyVector;
                            std::vector<std::shared_ptr<ApduRequest>> req = samSeRequest->getApduRequests();
                            throw KeypleCalypsoSecureSessionException("Null response received", KeypleCalypsoSecureSessionException::Type::SAM, req, emptyVector);
                        }

                        if (samSeResponse->wasChannelPreviouslyOpen() == false) {
                            std::vector<std::shared_ptr<ApduResponse>> emptyVector;
                            std::vector<std::shared_ptr<ApduRequest>> req = samSeRequest->getApduRequests();
                            throw KeypleCalypsoSecureSessionException("The logical channel was not open", KeypleCalypsoSecureSessionException::Type::SAM, req, emptyVector);
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
                            throw IllegalStateException("No response to Digest Authenticate.");
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

                    std::shared_ptr<SeResponse> PoTransaction::processAtomicClosing(std::vector<std::shared_ptr<PoModificationCommand>> &poModificationCommands, TransmissionMode transmissionMode, ChannelState channelState)
                    {
                        std::vector<std::shared_ptr<ApduResponse>> poAnticipatedResponses = AnticipatedResponseBuilder::getResponses(poModificationCommands);
                        return processAtomicClosing(poModificationCommands, poAnticipatedResponses, transmissionMode, channelState);
                    }

                    PoRevision PoTransaction::getRevision() {
                        // TODO checks if poRevision initialized
                        return poRevision;
                    }

                    bool PoTransaction::isSuccessful() {

                        if (currentState != SessionState::SESSION_CLOSED) {
                            throw IllegalStateException(StringHelper::formatSimple("Session is not closed, state: %d, expected: %d", static_cast<int>(currentState), static_cast<int>(SessionState::SESSION_OPEN)));
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

                    std::vector<std::vector<char>> PoTransaction::DigestProcessor::poDigestDataCache = std::vector<std::vector<char>>();
                    PoRevision PoTransaction::DigestProcessor::poRevision = (PoRevision)0;
                    bool PoTransaction::DigestProcessor::encryption = false;
                    bool PoTransaction::DigestProcessor::verification = false;
                    bool PoTransaction::DigestProcessor::revMode = false;
                    char PoTransaction::DigestProcessor::keyRecordNumber = 0;
                    char PoTransaction::DigestProcessor::keyKIF = 0;
                    char PoTransaction::DigestProcessor::keyKVC = 0;

                    void PoTransaction::DigestProcessor::initialize(PoRevision poRev, SamRevision samRev, bool sessionEncryption, bool verificationMode, bool rev3_2Mode, char workKeyRecordNumber, char workKeyKif, char workKeyKVC, std::vector<char> &digestData)
                    {
                        /* Store work context */
                        poRevision = poRev;
                        samRevision = SamRevision(samRev);
                        encryption = sessionEncryption;
                        verification = verificationMode;
                        revMode = rev3_2Mode;
                        keyRecordNumber = workKeyRecordNumber;
                        keyKIF = workKeyKif;
                        keyKVC = workKeyKVC;
                        if (logger->isDebugEnabled()) {
                            logger->debug("PoTransaction.DigestProcessor => initialize: POREVISION = %d, SAMREVISION = %d, SESSIONENCRYPTION = %d", static_cast<int>(poRev), static_cast<int>(samRev.ordinal()), sessionEncryption, verificationMode);
                            logger->debug("PoTransaction.DigestProcessor => initialize: VERIFICATIONMODE = %d, REV32MODE = %d KEYRECNUMBER = %d", verificationMode, rev3_2Mode, workKeyRecordNumber);
                            logger->debug("PoTransaction.DigestProcessor => initialize: KIF = %s, KVC %ds DIGESTDATA = %s", StringHelper::formatSimple("%02X", workKeyKif),
                                          StringHelper::formatSimple("%02X", workKeyKVC), org::eclipse::keyple::util::ByteArrayUtils::toHex(digestData));
                        }

                        /* Clear data cache */
                        poDigestDataCache.clear();

                        /*
                         * Build Digest Init command as first ApduRequest of the digest computation process
                         */
                        poDigestDataCache.push_back(digestData);
                    }

                    void PoTransaction::DigestProcessor::pushPoExchangeData(std::shared_ptr<ApduRequest> request, std::shared_ptr<ApduResponse> response)
                    {

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

                    std::shared_ptr<SeRequest> PoTransaction::DigestProcessor::getSamDigestRequest()
                    {
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
                        samApduRequestList.push_back((std::make_shared<DigestInitCmdBuild>(samRevision, verification, revMode, keyRecordNumber, keyKIF, keyKVC, poDigestDataCache[0]))->getApduRequest());

                        /*
                         * Build and append Digest Update commands
                         *
                         * The first command is at index 1.
                         */
                        for (int i = 1; i < (int)poDigestDataCache.size(); i++) {
                            samApduRequestList.push_back(std::make_shared<DigestUpdateCmdBuild>(samRevision, encryption, poDigestDataCache[i])->getApduRequest());
                        }

                        /*
                         * Build and append Digest Close command
                         */
                        samApduRequestList.push_back(std::make_shared<DigestCloseCmdBuild>(samRevision, poRevision == PoRevision::REV3_2 ? SIGNATURE_LENGTH_REV32 : SIGNATURE_LENGTH_REV_INF_32)->getApduRequest());

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

std::unordered_map<char, std::shared_ptr<PoTransaction::AnticipatedResponseBuilder::CommandResponse>> PoTransaction::AnticipatedResponseBuilder::sfiCommandResponseHashMap;

                    void PoTransaction::AnticipatedResponseBuilder::storeCommandResponse(std::vector<std::shared_ptr<PoSendableInSession>> &poSendableInSessions, std::vector<std::shared_ptr<ApduRequest>> &apduRequests, std::vector<std::shared_ptr<ApduResponse>> &apduResponses, bool /*Boolean*/ skipFirstItem) {
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
                                apduRequestIterator++;
//JAVA TO C++ CONVERTER TODO TASK: Java iterators are only converted within the context of 'while' and 'for' loops:
                                apduResponseIterator++;
                            }
                            /* Iterate over the poCommandsInsideSession list */
                            for (auto poSendableInSession : poSendableInSessions) {
                                if (reinterpret_cast<ReadRecordsCmdBuild*>(poSendableInSession.get()) != nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: Java iterators are only converted within the context of 'while' and 'for' loops:
                                    std::shared_ptr<ApduRequest> apduRequest = *(++apduRequestIterator);
                                    char sfi = static_cast<char>((apduRequest->getBytes()[OFFSET_P2] >> 3) & 0x1F);
//JAVA TO C++ CONVERTER TODO TASK: Java iterators are only converted within the context of 'while' and 'for' loops:
                                    sfiCommandResponseHashMap.emplace(std::pair<char, std::shared_ptr<PoTransaction::AnticipatedResponseBuilder::CommandResponse>>(sfi, std::make_shared<CommandResponse>(apduRequest, *(++apduResponseIterator))));
                                }
                                else {
//JAVA TO C++ CONVERTER TODO TASK: Java iterators are only converted within the context of 'while' and 'for' loops:
                                    apduRequestIterator++;
//JAVA TO C++ CONVERTER TODO TASK: Java iterators are only converted within the context of 'while' and 'for' loops:
                                    apduResponseIterator++;
                                }
                            }
                        }
                    }

                    std::vector<std::shared_ptr<ApduResponse>> PoTransaction::AnticipatedResponseBuilder::getResponses(std::vector<std::shared_ptr<PoModificationCommand>> &poModificationCommands)
                    {
                        std::vector<std::shared_ptr<ApduResponse>> apduResponses;
                        if (poModificationCommands.size() > 0) {
                            for (auto poModificationCommand : poModificationCommands) {
                                if (std::static_pointer_cast<DecreaseCmdBuild>(poModificationCommand) != nullptr || std::static_pointer_cast<IncreaseCmdBuild>(poModificationCommand) != nullptr) {
                                    /* response = NNNNNN9000 */
                                    std::vector<char> modCounterApduRequest = reinterpret_cast<DecreaseCmdBuild*>(poModificationCommand.get())->getApduRequest()->getBytes();
                                    /* Retrieve SFI from the current Decrease command */
                                    char sfi = static_cast<char>((modCounterApduRequest[OFFSET_P2] >> 3) & 0x1F);
                                    /*
                                     * Look for the counter value in the stored records. Only the first
                                     * occurrence of the SFI is taken into account. We assume here that the
                                     * record number is always 1.
                                     */
                                    std::shared_ptr<PoTransaction::AnticipatedResponseBuilder::CommandResponse> commandResponse = sfiCommandResponseHashMap[sfi];
                                    if (commandResponse != nullptr) {
                                        char counterNumber = modCounterApduRequest[OFFSET_P1];
                                        /*
                                         * The record containing the counters is structured as follow:
                                         * AAAAAAABBBBBBCCCCCC...XXXXXX each counter being a 3-byte unsigned
                                         * number. Convert the 3-byte block indexed by the counter number to an
                                         * int.
                                         */
                                        std::vector<char> resp = commandResponse->getApduResponse()->getBytes();
                                        int currentCounterValue = ByteArrayUtils::threeBytesToInt(resp, (counterNumber - 1) * 3);
                                        /* Extract the add or subtract value from the modification request */
                                        int addSubtractValue = ByteArrayUtils::threeBytesToInt(modCounterApduRequest, OFFSET_DATA);
                                        /* Build the response */
                                        std::vector<char> response(5);
                                        int newCounterValue;
                                        if (std::static_pointer_cast<DecreaseCmdBuild>(poModificationCommand) != nullptr) {
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
                                            /* FIXME: dynamic cast doesn't work here, it's just logs but... */
                                            logger->debug("Anticipated response. COMMAND = %s, SFI = %d, COUNTERVALUE = %d, DECREMENT = %d, NEWVALUE = %d ",
                                                          "(std::dynamic_pointer_cast<DecreaseCmdBuild>(poModificationCommand) != nullptr)" ? "Decrease" : "Increase",
                                                          sfi, currentCounterValue, addSubtractValue, newCounterValue);
                                        }
                                    }
                                    else {
                                        throw KeypleCalypsoSecureSessionException(StringHelper::formatSimple("Anticipated response. COMMAND = %s. Unable to determine anticipated counter value. SFI = %s\n",
                                                                                  std::static_pointer_cast<DecreaseCmdBuild>(poModificationCommand) != nullptr ? "Decrease" : "Increase", std::to_string(sfi)),
                                                                                  reinterpret_cast<DecreaseCmdBuild*>(poModificationCommand.get())->getApduRequest(), nullptr);
                                    }
                                }
                                else {
                                    /* Append/Update/Write Record: response = 9000 */
                                    std::vector<char> resp = org::eclipse::keyple::util::ByteArrayUtils::fromHex("9000");
                                    apduResponses.push_back(std::make_shared<ApduResponse>(resp, nullptr));
                                }
                            }
                        }
                        return apduResponses;
                    }

                    bool PoTransaction::processOpening(ModificationMode modificationMode, SessionAccessLevel accessLevel, char openingSfiToSelect, char openingRecordNumberToRead)
                    {
                        currentModificationMode = modificationMode;
                        currentAccessLevel = accessLevel;
                        char localOpeningRecordNumberToRead = openingRecordNumberToRead;
                        bool poProcessSuccess = true;
                        /*
                         * Iterator to keep the progress in updating the parsers from the list of prepared commands
                         */
                        std::shared_ptr<std::vector<std::shared_ptr<AbstractApduResponseParser>>::const_iterator> apduResponseParserIterator = std::make_shared<std::vector<std::shared_ptr<AbstractApduResponseParser>>::const_iterator>(poResponseParserList.begin());
                        std::vector<std::shared_ptr<PoSendableInSession>> poAtomicCommandBuilderList;
                        for (auto poCommandBuilderElement : poCommandBuilderList) {
                            if (!reinterpret_cast<PoModificationCommand*>(poCommandBuilderElement.get())) {
                                /* This command does not affect the PO modifications buffer */
                                poAtomicCommandBuilderList.push_back(poCommandBuilderElement);
                            }
                            else {
                                /* This command affects the PO modifications buffer */
                                if (willOverflowBuffer(std::shared_ptr<PoModificationCommand>(reinterpret_cast<PoModificationCommand*>(poCommandBuilderElement.get())))) {
                                    if (currentModificationMode == ModificationMode::ATOMIC) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                        /* FIXME */
                                        throw IllegalStateException("ATOMIC mode error! This command would overflow the PO modifications buffer: poCommandBuilderElement->toString()");
                                    }
                                    std::shared_ptr<SeResponse> seResponseOpening = processAtomicOpening(currentAccessLevel, openingSfiToSelect, localOpeningRecordNumberToRead, poAtomicCommandBuilderList);

                                    /*
                                     * inhibit record reading for next round, keep file selection (TODO check this)
                                     */
                                    localOpeningRecordNumberToRead = static_cast<char>(0x00);

                                    if (!updateParsersWithResponses(seResponseOpening, apduResponseParserIterator, poResponseParserList)) {
                                        poProcessSuccess = false;
                                    }
                                    /*
                                     * Closes the session, resets the modifications buffer counters for the next
                                     * round (set the contact mode to avoid the transmission of the ratification)
                                     */
                                    std::vector<std::shared_ptr<PoModificationCommand>> emptyVector;
                                    processAtomicClosing(emptyVector, TransmissionMode::CONTACTS, ChannelState::KEEP_OPEN);
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
                                    willOverflowBuffer(std::shared_ptr<PoModificationCommand>(reinterpret_cast<PoModificationCommand*>(poCommandBuilderElement.get())));
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
                        if (!updateParsersWithResponses(seResponseOpening, apduResponseParserIterator, poResponseParserList)) {
                            poProcessSuccess = false;
                        }

                        poCommandBuilderList.clear();
                        poResponseParserList.clear();
                        return poProcessSuccess;
                    }

                    bool PoTransaction::processPoCommands(ChannelState channelState)
                    {

                        /** This method should be called only if no session was previously open */
                        if (currentState == SessionState::SESSION_OPEN) {
                            throw std::make_shared<IllegalStateException>("A session is open");
                        }

                        bool poProcessSuccess = true;
                        /*
                         * Iterator to keep the progress in updating the parsers from the list of prepared commands
                         */
                        std::shared_ptr<std::vector<std::shared_ptr<AbstractApduResponseParser>>::const_iterator> abstractApduResponseParserIterator = std::make_shared<std::vector<std::shared_ptr<AbstractApduResponseParser>>::const_iterator>(poResponseParserList.begin());
                        /* PO commands sent outside a Secure Session. No modifications buffer limitation. */
                        std::shared_ptr<SeResponse> seResponsePoCommands = processAtomicPoCommands(poCommandBuilderList, channelState);

                        if (!updateParsersWithResponses(seResponsePoCommands, abstractApduResponseParserIterator, poResponseParserList)) {
                            poProcessSuccess = false;
                        }

                        /* clean up global lists */
                        poCommandBuilderList.clear();
                        poResponseParserList.clear();
                        return poProcessSuccess;
                    }

                    bool PoTransaction::processPoCommandsInSession()
                    {

                        /** This method should be called only if a session was previously open */
                        if (currentState == SessionState::SESSION_CLOSED) {
                            throw std::make_shared<IllegalStateException>("No open session");
                        }

                        bool poProcessSuccess = true;
                        /*
                         * Iterator to keep the progress in updating the parsers from the list of prepared commands
                         */
                        std::shared_ptr<std::vector<std::shared_ptr<AbstractApduResponseParser>>::const_iterator> abstractApduResponseParserIterator = std::make_shared<std::vector<std::shared_ptr<AbstractApduResponseParser>>::const_iterator>(poResponseParserList.begin());

                        /* A session is open, we have to care about the PO modifications buffer */
                        std::vector<std::shared_ptr<PoSendableInSession>> poAtomicCommandBuilderList;

                        for (auto poCommandBuilderElement : poCommandBuilderList) {
                            if (!reinterpret_cast<PoModificationCommand*>(poCommandBuilderElement.get())) {
                                /* This command does not affect the PO modifications buffer */
                                poAtomicCommandBuilderList.push_back(poCommandBuilderElement);
                            }
                            else {
                                /* This command affects the PO modifications buffer */
                                if (willOverflowBuffer(std::shared_ptr<PoModificationCommand>(reinterpret_cast<PoModificationCommand*>(poCommandBuilderElement.get())))) {
                                    if (currentModificationMode == ModificationMode::ATOMIC) {
                                        throw IllegalStateException("ATOMIC mode error! This command would overflow the PO modifications buffer: <fixme>poCommandBuilderElement->toString()");
                                    }
                                    /*
                                     * The current command would overflow the modifications buffer in the PO. We
                                     * send the current commands and update the parsers. The parsers Iterator is
                                     * kept all along the process.
                                     */
                                    std::shared_ptr<SeResponse> seResponsePoCommands = processAtomicPoCommands(poAtomicCommandBuilderList, ChannelState::KEEP_OPEN);
                                    if (!updateParsersWithResponses(seResponsePoCommands, abstractApduResponseParserIterator, poResponseParserList)) {
                                        poProcessSuccess = false;
                                    }
                                    /*
                                     * Close the session and reset the modifications buffer counters for the next
                                     * round (set the contact mode to avoid the transmission of the ratification)
                                     */
                                    std::vector<std::shared_ptr<PoModificationCommand>> empty1;
                                    processAtomicClosing(empty1, TransmissionMode::CONTACTS, ChannelState::KEEP_OPEN);
                                    resetModificationsBufferCounter();
                                    /* We reopen a new session for the remaining commands to be sent */
                                    std::vector<std::shared_ptr<PoSendableInSession>> empty2;
                                    std::shared_ptr<SeResponse> seResponseOpening = processAtomicOpening(currentAccessLevel, static_cast<char>(0x00), static_cast<char>(0x00), empty2);
                                    /*
                                     * Clear the list and add the command that did not fit in the PO modifications
                                     * buffer. We also update the usage counter without checking the result.
                                     */
                                    poAtomicCommandBuilderList.clear();
                                    poAtomicCommandBuilderList.push_back(std::shared_ptr<PoSendableInSession>(reinterpret_cast<PoSendableInSession*>(poCommandBuilderElement.get())));
                                    /*
                                     * just update modifications buffer usage counter, ignore result (always false)
                                     */
                                    willOverflowBuffer(std::shared_ptr<PoModificationCommand>(reinterpret_cast<PoModificationCommand*>(poCommandBuilderElement.get())));
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
                            if (!updateParsersWithResponses(seResponsePoCommands, abstractApduResponseParserIterator, poResponseParserList)) {
                                poProcessSuccess = false;
                            }
                        }

                        /* clean up global lists */
                        poCommandBuilderList.clear();
                        poResponseParserList.clear();
                        return poProcessSuccess;
                    }

                    bool PoTransaction::processClosing(TransmissionMode transmissionMode, ChannelState channelState)
                    {
                        bool poProcessSuccess = true;
                        bool atLeastOneReadCommand = false;
                        bool sessionPreviouslyClosed = false;
                        std::vector<std::shared_ptr<PoModificationCommand>> poModificationCommandList;
                        std::shared_ptr<std::vector<std::shared_ptr<AbstractApduResponseParser>>::const_iterator> abstractApduResponseParserIterator = std::make_shared<std::vector<std::shared_ptr<AbstractApduResponseParser>>::const_iterator>(poResponseParserList.begin());
                        std::vector<std::shared_ptr<PoModificationCommand>> poAtomicCommandBuilderList;
                        std::shared_ptr<SeResponse> seResponseClosing;
                        for (auto poCommandBuilderElement : poCommandBuilderList) {
                            if (!(reinterpret_cast<PoModificationCommand*>(poCommandBuilderElement.get()) != nullptr)) {
                                /*
                                 * This command does not affect the PO modifications buffer. We will call
                                 * processPoCommands first
                                 */
                                poAtomicCommandBuilderList.push_back(std::shared_ptr<PoModificationCommand>(reinterpret_cast<PoModificationCommand*>(poCommandBuilderElement.get())));
                                atLeastOneReadCommand = true;
                            }
                            else {
                                /* This command affects the PO modifications buffer */
                                if (willOverflowBuffer(std::shared_ptr<PoModificationCommand>(reinterpret_cast<PoModificationCommand*>(poCommandBuilderElement.get())))) {
                                    if (currentModificationMode == ModificationMode::ATOMIC) {
                                        throw IllegalStateException(StringHelper::formatSimple("ATOMIC mode error! This command would overflow the PO modifications buffer: %s\n", poCommandBuilderElement));
                                    }
                                    /*
                                     * Reopen a session with the same access level if it was previously closed in
                                     * this current processClosing
                                     */
                                    if (sessionPreviouslyClosed) {
                                        std::vector<std::shared_ptr<PoSendableInSession>> emptyVector;
                                        processAtomicOpening(currentAccessLevel, static_cast<char>(0x00), static_cast<char>(0x00), emptyVector);
                                    }

                                    /*
                                     * If at least one non-modifying was prepared, we use processAtomicPoCommands
                                     * instead of processAtomicClosing to send the list
                                     */
                                    if (atLeastOneReadCommand) {
                                        std::vector<std::shared_ptr<PoSendableInSession>> poSendableInSessionList;
                                        for (auto command : poAtomicCommandBuilderList) {
                                            poSendableInSessionList.push_back(std::shared_ptr<PoSendableInSession>(reinterpret_cast<PoSendableInSession*>(command.get())));
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

                                    std::shared_ptr<std::vector<std::shared_ptr<AbstractApduResponseParser>>::const_iterator> apduResponseParserIterator = std::make_shared<std::vector<std::shared_ptr<AbstractApduResponseParser>>::const_iterator>(poResponseParserList.begin());
                                    if (!updateParsersWithResponses(seResponseClosing, apduResponseParserIterator, poResponseParserList)) {
                                        poProcessSuccess = false;
                                    }
                                    /*
                                     * Clear the list and add the command that did not fit in the PO modifications
                                     * buffer. We also update the usage counter without checking the result.
                                     */
                                    poAtomicCommandBuilderList.clear();
                                    poAtomicCommandBuilderList.push_back(std::shared_ptr<PoModificationCommand>(reinterpret_cast<PoModificationCommand*>(poCommandBuilderElement.get())));
                                    /*
                                     * just update modifications buffer usage counter, ignore result (always false)
                                     */
                                    willOverflowBuffer(std::shared_ptr<PoModificationCommand>(reinterpret_cast<PoModificationCommand*>(poCommandBuilderElement.get())));
                                }
                                else {
                                    /*
                                     * The command fits in the PO modifications buffer, just add it to the list
                                     */
                                    poAtomicCommandBuilderList.push_back(std::shared_ptr<PoModificationCommand>(reinterpret_cast<PoModificationCommand*>(poCommandBuilderElement.get())));
                                }
                            }
                        }
                        if (sessionPreviouslyClosed) {
                            /*
                             * Reopen if needed, to close the session with the requested conditions
                             * (CommunicationMode and channelState)
                             */
                            std::vector<std::shared_ptr<PoSendableInSession>> emptyVector;
                            processAtomicOpening(currentAccessLevel, static_cast<char>(0x00), static_cast<char>(0x00), emptyVector);
                        }

                        /* Finally, close the session as requested */
                        seResponseClosing = processAtomicClosing(poAtomicCommandBuilderList, transmissionMode, channelState);

                        /* Update parsers */
                        if (!updateParsersWithResponses(seResponseClosing, abstractApduResponseParserIterator, poResponseParserList)) {
                            poProcessSuccess = false;
                        }

                        /* clean up global lists */
                        poCommandBuilderList.clear();
                        poResponseParserList.clear();
                        return poProcessSuccess;
                    }

                    bool PoTransaction::processCancel(ChannelState channelState)
                    {
                        /* PO ApduRequest List to hold Close Secure Session command */
                        std::vector<std::shared_ptr<ApduRequest>> poApduRequestList;

                        /* Build the PO Close Session command (in "abort" mode since no signature is provided). */
                        std::shared_ptr<CloseSessionCmdBuild> closeCommand = std::make_shared<CloseSessionCmdBuild>(calypsoPo->getPoClass());

                        poApduRequestList.push_back(closeCommand->getApduRequest());

                        /*
                         * Transfer PO commands
                         */
                        std::shared_ptr<SeRequest> poSeRequest = std::make_shared<SeRequest>(poApduRequestList, channelState);

                        logger->debug("processCancel => POSEREQUEST = %s", poSeRequest);

                        std::shared_ptr<SeResponse> poSeResponse;
                        try {
                            poSeResponse = poReader->transmit(poSeRequest);
                        }
                        catch (KeypleReaderException &ex) {
                            poSeResponse = ex.getSeResponse();
                        }

                        logger->debug("processCancel => POSERESPONSE = %s", poSeResponse);

                        /* clean up global lists */
                        poCommandBuilderList.clear();
                        poResponseParserList.clear();

                        /*
                         * session is now considered closed regardless the previous state or the result of the abort
                         * session command sent to the PO.
                         */
                        currentState = SessionState::SESSION_CLOSED;

                        /* return the successful status of the abort session command */
                        return poSeResponse->getApduResponses()[0]->isSuccessful();
                    }

                    bool PoTransaction::updateParsersWithResponses(std::shared_ptr<SeResponse> seResponse,
                                                                   std::shared_ptr<std::vector<std::shared_ptr<AbstractApduResponseParser>>::const_iterator> parserIterator,
                                                                   std::vector<std::shared_ptr<AbstractApduResponseParser>> responseList)
                    {
                        bool allSuccessfulCommands = true;
                        /* double loop to set apdu responses to corresponding parsers */
                        for (auto apduResponse : seResponse->getApduResponses()) {
                            if (*parserIterator != responseList.end()) {
                                throw IllegalStateException("Parsers list and responses list mismatch! ");
                            }
                            std::shared_ptr<AbstractApduResponseParser> parser = **parserIterator;
                            parser->setApduResponse(apduResponse);
                            if (!apduResponse->isSuccessful()) {
                                allSuccessfulCommands = false;
                            }
                        }
                        return allSuccessfulCommands;
                    }

                    bool PoTransaction::willOverflowBuffer(std::shared_ptr<PoModificationCommand> modificationCommand) {
                        bool willOverflow = false;
                        if (modificationsCounterIsInBytes) {
                            int bufferRequirement = (reinterpret_cast<AbstractApduCommandBuilder*>(modificationCommand.get()))->getApduRequest()->getBytes()[OFFSET_Lc] + 6;

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

                    std::shared_ptr<ReadRecordsRespPars> PoTransaction::prepareReadRecordsCmdInternal(char sfi,
                                                                                                      ReadDataStructure readDataStructureEnum,
                                                                                                      char firstRecordNumber,
                                                                                                      int expectedLength,
                                                                                                      const std::string &extraInfo)
                    {
                        /*
                         * the readJustOneRecord flag is set to false only in case of multiple read records, in all
                         * other cases it is set to true
                         */
                        bool readJustOneRecord = !(readDataStructureEnum == ReadDataStructure::MULTIPLE_RECORD_DATA);

                        poCommandBuilderList.push_back(std::make_shared<ReadRecordsCmdBuild>(calypsoPo->getPoClass(),
                                                                                             sfi,
                                                                                             firstRecordNumber,
                                                                                             readJustOneRecord,
                                                                                             static_cast<char>(expectedLength),
                                                                                             extraInfo));

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

                    std::shared_ptr<AppendRecordRespPars> PoTransaction::prepareAppendRecordCmd(char sfi, std::vector<char> &newRecordData, const std::string &extraInfo)
                    {
                        poCommandBuilderList.push_back(std::make_shared<AppendRecordCmdBuild>(calypsoPo->getPoClass(), sfi, newRecordData, extraInfo));

                        std::shared_ptr<AppendRecordRespPars> poResponseParser = std::make_shared<AppendRecordRespPars>();

                        poResponseParserList.push_back(poResponseParser);

                        return poResponseParser;
                    }

                    std::shared_ptr<UpdateRecordRespPars> PoTransaction::prepareUpdateRecordCmd(char sfi, char recordNumber, std::vector<char> &newRecordData, const std::string &extraInfo)
                    {
                        poCommandBuilderList.push_back(std::make_shared<UpdateRecordCmdBuild>(calypsoPo->getPoClass(), sfi, recordNumber, newRecordData, extraInfo));

                        std::shared_ptr<UpdateRecordRespPars> poResponseParser = std::make_shared<UpdateRecordRespPars>();

                        poResponseParserList.push_back(poResponseParser);

                        return poResponseParser;
                    }

                    std::shared_ptr<IncreaseRespPars> PoTransaction::prepareIncreaseCmd(char sfi, char counterNumber, int incValue, const std::string &extraInfo)
                    {
                        poCommandBuilderList.push_back(std::make_shared<IncreaseCmdBuild>(calypsoPo->getPoClass(), sfi, counterNumber, incValue, extraInfo));

                        std::shared_ptr<IncreaseRespPars> poResponseParser = std::make_shared<IncreaseRespPars>();

                        poResponseParserList.push_back(poResponseParser);

                        return poResponseParser;
                    }

                    std::shared_ptr<DecreaseRespPars> PoTransaction::prepareDecreaseCmd(char sfi, char counterNumber, int decValue, const std::string &extraInfo)
                    {
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
