#include "PoGetChallengeCmdBuild.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace builder {
                            namespace session {
                                using PoClass = org::eclipse::keyple::calypso::command::PoClass;
                                using CalypsoPoCommands = org::eclipse::keyple::calypso::command::po::CalypsoPoCommands;
                                using PoCommandBuilder = org::eclipse::keyple::calypso::command::po::PoCommandBuilder;

                                PoGetChallengeCmdBuild::PoGetChallengeCmdBuild(PoClass poClass) : org::eclipse::keyple::calypso::command::po::PoCommandBuilder(command, nullptr) {

                                    char p1 = static_cast<char>(0x01);
                                    char p2 = static_cast<char>(0x10);
                                    char le = static_cast<char>(0x08);

                                    this->request = setApduRequest(poClass.getValue(), command, p1, p2, nullptr, le);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
