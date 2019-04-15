#include <vector>

#include "GetDataFciCmdBuild.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace builder {
                            using PoClass           = org::eclipse::keyple::calypso::command::PoClass;
                            using CalypsoPoCommands = org::eclipse::keyple::calypso::command::po::CalypsoPoCommands;
                            using PoCommandBuilder  = org::eclipse::keyple::calypso::command::po::PoCommandBuilder;

                            const CalypsoPoCommands GetDataFciCmdBuild::command = CalypsoPoCommands::GET_DATA_FCI;

                            GetDataFciCmdBuild::GetDataFciCmdBuild(PoClass poClass) : org::eclipse::keyple::calypso::command::po::PoCommandBuilder(std::make_shared<CalypsoPoCommands>(command), nullptr) {
                                std::vector<char> emtpyVector;
                                request = setApduRequest(poClass.getValue(), std::make_shared<CalypsoPoCommands>(command), static_cast<char>(0x00), static_cast<char>(0x6F), emtpyVector, static_cast<char>(0x00));
                            }
                        }
                    }
                }
            }
        }
    }
}
