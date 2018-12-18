#include "GetDataFciCmdBuild.h"

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

                            GetDataFciCmdBuild::GetDataFciCmdBuild(PoClass poClass) : org::eclipse::keyple::calypso::command::po::PoCommandBuilder(command, nullptr) {

                                request = setApduRequest(poClass.getValue(), command, static_cast<char>(0x00), static_cast<char>(0x6F), nullptr, static_cast<char>(0x00));
                            }
                        }
                    }
                }
            }
        }
    }
}
