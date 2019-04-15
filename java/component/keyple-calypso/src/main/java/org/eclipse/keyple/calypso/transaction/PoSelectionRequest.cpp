#include "PoSelectionRequest.h"
#include "AbstractApduResponseParser.h"
#include "SeSelector.h"
#include "SeProtocol.h"
#include "CalypsoPo.h"
#include "ReadRecordsRespPars.h"
#include "ReadRecordsCmdBuild.h"
#include "ContactsProtocols.h"
#include "SelectFileRespPars.h"
#include "SelectFileCmdBuild.h"
#include "ByteArrayUtils.h"
#include "ApduRequest.h"
#include "PoCustomReadCommandBuilder.h"
#include "PoCustomModificationCommandBuilder.h"
#include "SeResponse.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace transaction {
                    using PoClass                            = org::eclipse::keyple::calypso::command::PoClass;
                    using PoCustomModificationCommandBuilder = org::eclipse::keyple::calypso::command::po::PoCustomModificationCommandBuilder;
                    using PoCustomReadCommandBuilder         = org::eclipse::keyple::calypso::command::po::PoCustomReadCommandBuilder;
                    using ReadRecordsCmdBuild                = org::eclipse::keyple::calypso::command::po::builder::ReadRecordsCmdBuild;
                    using SelectFileCmdBuild                 = org::eclipse::keyple::calypso::command::po::builder::SelectFileCmdBuild;
                    using ReadDataStructure                  = org::eclipse::keyple::calypso::command::po::parser::ReadDataStructure;
                    using ReadRecordsRespPars                = org::eclipse::keyple::calypso::command::po::parser::ReadRecordsRespPars;
                    using SelectFileRespPars                 = org::eclipse::keyple::calypso::command::po::parser::SelectFileRespPars;
                    using AbstractApduResponseParser         = org::eclipse::keyple::command::AbstractApduResponseParser;
                    using ChannelState                       = org::eclipse::keyple::seproxy::ChannelState;
                    using SeSelector                         = org::eclipse::keyple::seproxy::SeSelector;
                    using ApduRequest                        = org::eclipse::keyple::seproxy::message::ApduRequest;
                    using ApduResponse                       = org::eclipse::keyple::seproxy::message::ApduResponse;
                    using SeResponse                         = org::eclipse::keyple::seproxy::message::SeResponse;
                    using ContactsProtocols                  = org::eclipse::keyple::seproxy::protocol::ContactsProtocols;
                    using SeProtocol                         = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                    using SeSelectionRequest                 = org::eclipse::keyple::transaction::SeSelectionRequest;
                    using ByteArrayUtils                     = org::eclipse::keyple::util::ByteArrayUtils;

                    PoSelectionRequest::PoSelectionRequest(std::shared_ptr<SeSelector> seSelector, ChannelState channelState, std::shared_ptr<SeProtocol> protocolFlag) : SeSelectionRequest(seSelector, channelState, protocolFlag), poClass(PoClass::LEGACY)
                    {

                        setMatchingClass(typeid(CalypsoPo));
                        setSelectionClass(typeid(PoSelectionRequest));

                        /* No AID selector for a legacy Calypso PO */
                        if (seSelector->getAidSelector() == nullptr) {
                            poClass = PoClass::LEGACY;
                        }
                        else {
                            poClass = PoClass::ISO;
                        }

                        if (logger->isTraceEnabled()) {
                            logger->trace("Calypso %s selector", poClass.toString());
                        }
                    }

                    std::shared_ptr<ReadRecordsRespPars> PoSelectionRequest::prepareReadRecordsCmdInternal(char sfi, ReadDataStructure readDataStructureEnum, char firstRecordNumber, int expectedLength, const std::string &extraInfo) {

                        /*
                         * the readJustOneRecord flag is set to false only in case of multiple read records, in all
                         * other cases it is set to true
                         */
                        bool readJustOneRecord = !(readDataStructureEnum == ReadDataStructure::MULTIPLE_RECORD_DATA);

                        addApduRequest((std::make_shared<ReadRecordsCmdBuild>(poClass, sfi, firstRecordNumber, readJustOneRecord, static_cast<char>(expectedLength), extraInfo))->getApduRequest());

                        if (logger->isTraceEnabled()) {
                            logger->trace("ReadRecords: SFI = %02x, RECNUMBER = %d, JUSTONE = %d, EXPECTEDLENGTH = %d", sfi, firstRecordNumber, readJustOneRecord, expectedLength);
                        }

                        /* create a parser to be returned to the caller */
                        std::shared_ptr<ReadRecordsRespPars> poResponseParser = std::make_shared<ReadRecordsRespPars>(firstRecordNumber, readDataStructureEnum);

                        /*
                         * keep the parser in the CommandParser list
                         */
                        poResponseParserList.push_back(poResponseParser);

                        return poResponseParser;
                    }

                    std::shared_ptr<ReadRecordsRespPars> PoSelectionRequest::prepareReadRecordsCmd(char sfi, ReadDataStructure readDataStructureEnum, char firstRecordNumber, int expectedLength, const std::string &extraInfo) {
                        if (expectedLength < 1 || expectedLength > 250) {
                            throw std::invalid_argument("Bad length.");
                        }
                        return prepareReadRecordsCmdInternal(sfi, readDataStructureEnum, firstRecordNumber, expectedLength, extraInfo);
                    }

                    std::shared_ptr<ReadRecordsRespPars> PoSelectionRequest::prepareReadRecordsCmd(char sfi, ReadDataStructure readDataStructureEnum, char firstRecordNumber, const std::string &extraInfo) {
                        if (*(std::dynamic_pointer_cast<ContactsProtocols>(protocolFlag)).get() == ContactsProtocols::PROTOCOL_ISO7816_3) {
                            throw std::invalid_argument("In contacts mode, the expected length must be specified.");
                        }
                        return prepareReadRecordsCmdInternal(sfi, readDataStructureEnum, firstRecordNumber, 0, extraInfo);
                    }

                    std::shared_ptr<SelectFileRespPars> PoSelectionRequest::prepareSelectFileDfCmd(std::vector<char> &path, const std::string &extraInfo)
                    {
                        (void)extraInfo;

                        addApduRequest((std::make_shared<SelectFileCmdBuild>(poClass, SelectFileCmdBuild::SelectControl::PATH_FROM_MF, SelectFileCmdBuild::SelectOptions::FCI, path))->getApduRequest());
                        if (logger->isTraceEnabled()) {
                            logger->trace("Select File: PATH = %s", ByteArrayUtils::toHex(path));
                        }

                        /* create a parser to be returned to the caller */
                        std::shared_ptr<SelectFileRespPars> poResponseParser = std::make_shared<SelectFileRespPars>();

                        /*
                         * keep the parser in a CommandParser list with the number of apduRequest associated with it
                         */
                        poResponseParserList.push_back(poResponseParser);

                        return poResponseParser;
                    }

                    void PoSelectionRequest::preparePoCustomReadCmd(const std::string &name, std::shared_ptr<ApduRequest> apduRequest) {
                        addApduRequest((std::make_shared<PoCustomReadCommandBuilder>(name, apduRequest))->getApduRequest());
                        if (logger->isTraceEnabled()) {
                            logger->trace("CustomReadCommand: APDUREQUEST = %s", apduRequest);
                        }
                    }

                    void PoSelectionRequest::preparePoCustomModificationCmd(const std::string &name, std::shared_ptr<ApduRequest> apduRequest) {
                        addApduRequest((std::make_shared<PoCustomModificationCommandBuilder>(name, apduRequest))->getApduRequest());
                        if (logger->isTraceEnabled()) {
                            logger->trace("CustomModificationCommand: APDUREQUEST = %s", apduRequest);
                        }
                    }

                    void PoSelectionRequest::updateParsersWithResponses(std::shared_ptr<SeResponse> seResponse) {
                        /* attempt to update the parsers only if the list is not empty! */
                        if (poResponseParserList.size() != 0) {
                            std::vector<std::shared_ptr<AbstractApduResponseParser>>::const_iterator parserIterator = poResponseParserList.begin();
                            /* double loop to set apdu responses to corresponding parsers */
                            for (auto apduResponse : seResponse->getApduResponses()) {
                                if (parserIterator == poResponseParserList.end()) {
                                    throw std::make_shared<IllegalStateException>("Parsers list and responses list mismatch! ");
                                }
                                (*(++parserIterator)).get()->setApduResponse(apduResponse);
                                if (!apduResponse->isSuccessful()) {
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
