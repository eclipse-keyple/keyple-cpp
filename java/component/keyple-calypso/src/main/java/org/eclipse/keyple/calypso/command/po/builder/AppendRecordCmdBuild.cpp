#include "AppendRecordCmdBuild.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace builder {
                            using PoClass = org::eclipse::keyple::calypso::command::PoClass;
                            using namespace org::eclipse::keyple::calypso::command::po;

                            AppendRecordCmdBuild::AppendRecordCmdBuild(PoClass poClass, char sfi, std::vector<char> &newRecordData, const std::string &extraInfo) : PoCommandBuilder(command, nullptr) {
                                char cla = poClass.getValue();
                                char p1 = static_cast<char>(0x00);
                                char p2 = (sfi == 0) ? static_cast<char>(0x00) : static_cast<char>(sfi * 8);

                                this->request = setApduRequest(cla, command, p1, p2, newRecordData, nullptr);
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
