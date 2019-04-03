#include "ReadRecordsCmdBuild.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace builder {
                            using PoClass = org::eclipse::keyple::calypso::command::PoClass;
                            using CalypsoPoCommands = org::eclipse::keyple::calypso::command::po::CalypsoPoCommands;
                            using PoCommandBuilder = org::eclipse::keyple::calypso::command::po::PoCommandBuilder;
                            using PoSendableInSession = org::eclipse::keyple::calypso::command::po::PoSendableInSession;

                            ReadRecordsCmdBuild::ReadRecordsCmdBuild(PoClass poClass, char sfi, char firstRecordNumber, bool readJustOneRecord, char expectedLength, const std::string &extraInfo) throw(std::invalid_argument)
                            : org::eclipse::keyple::calypso::command::po::PoCommandBuilder(std::make_shared<CalypsoPoCommands>(command), nullptr) {

                                if (firstRecordNumber < 1) {
                                    throw std::invalid_argument("Bad record number (< 1)");
                                }

                                char p2 = (sfi == static_cast<char>(0x00)) ? static_cast<char>(0x05) : static_cast<char>(static_cast<char>(sfi * 8) + 5);
                                if (readJustOneRecord) {
                                    p2 = static_cast<char>(p2 - static_cast<char>(0x01));
                                }
                                std::vector<char> emptyVector;
                                this->request = setApduRequest(poClass.getValue(), std::make_shared<CalypsoPoCommands>(command), firstRecordNumber, p2, emptyVector, expectedLength);
                                if (extraInfo != "") {
                                    this->addSubName(extraInfo);
                                }
                            }

                            ReadRecordsCmdBuild::ReadRecordsCmdBuild(PoClass poClass, char sfi, char firstRecordNumber, bool readJustOneRecord, const std::string &extraInfo) throw(std::invalid_argument)
                            : ReadRecordsCmdBuild(poClass, sfi, firstRecordNumber, readJustOneRecord, 0x00, extraInfo) {
                            }
                        }
                    }
                }
            }
        }
    }
}
