#include "UpdateRecordCmdBuild.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace builder {
                            using PoClass = org::eclipse::keyple::calypso::command::PoClass;
                            using namespace org::eclipse::keyple::calypso::command::po;

                            UpdateRecordCmdBuild::UpdateRecordCmdBuild(PoClass poClass, char sfi, char recordNumber, std::vector<char> &newRecordData, const std::string &extraInfo)
                            : PoCommandBuilder(std::make_shared<CalypsoPoCommands>(command), nullptr) {
                                if (recordNumber < 1) {
                                    throw std::invalid_argument("Bad record number (< 1)");
                                }
                                char p2 = (sfi == 0) ? static_cast<char>(0x04) : static_cast<char>(static_cast<char>(sfi * 8) + 4);

                                this->request = setApduRequest(poClass.getValue(), std::make_shared<CalypsoPoCommands>(command), recordNumber, p2, newRecordData, -1);
                                if (extraInfo != "") {
                                    this->addSubName(extraInfo);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
