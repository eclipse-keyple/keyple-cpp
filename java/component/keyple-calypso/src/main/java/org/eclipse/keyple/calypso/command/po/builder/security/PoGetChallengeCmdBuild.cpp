#include "PoGetChallengeCmdBuild.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace builder {
                            namespace security {
                                using PoClass = org::eclipse::keyple::calypso::command::PoClass;
                                using CalypsoPoCommands = org::eclipse::keyple::calypso::command::po::CalypsoPoCommands;
                                using PoCommandBuilder = org::eclipse::keyple::calypso::command::po::PoCommandBuilder;

                                PoGetChallengeCmdBuild::PoGetChallengeCmdBuild(PoClass poClass) : org::eclipse::keyple::calypso::command::po::PoCommandBuilder(std::make_shared<CalypsoPoCommands>(command), nullptr) {

                                    char p1 = static_cast<char>(0x01);
                                    char p2 = static_cast<char>(0x10);
                                    char le = static_cast<char>(0x08);
                                    std::vector<char> emptyVector;
                                    this->request = setApduRequest(poClass.getValue(), std::make_shared<CalypsoPoCommands>(command), p1, p2, emptyVector, le);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
