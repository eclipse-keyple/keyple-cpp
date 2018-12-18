#include "SelectFileCmdBuild.h"

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

                            SelectFileCmdBuild::SelectFileCmdBuild(PoClass poClass, SelectControl selectControl, SelectOptions selectOptions, std::vector<char> &selectData) : org::eclipse::keyple::calypso::command::po::PoCommandBuilder(command, nullptr) {
                                char p1 = 0;
                                switch (selectControl) {
                                    case org::eclipse::keyple::calypso::command::po::builder::SelectFileCmdBuild::SelectControl::MF:
                                        p1 = static_cast<char>(0x00);
                                        break;
                                    case org::eclipse::keyple::calypso::command::po::builder::SelectFileCmdBuild::SelectControl::PATH_FROM_MF:
                                        p1 = static_cast<char>(0x08);
                                        break;
                                    case org::eclipse::keyple::calypso::command::po::builder::SelectFileCmdBuild::SelectControl::PATH_FROM_CURRENT_DF:
                                        p1 = static_cast<char>(0x02);
                                        break;
                                }

                                char p2 = 0;
                                switch (selectOptions) {
                                    case org::eclipse::keyple::calypso::command::po::builder::SelectFileCmdBuild::SelectOptions::FCI:
                                        break;
                                    case org::eclipse::keyple::calypso::command::po::builder::SelectFileCmdBuild::SelectOptions::FCP:
                                        p2 |= static_cast<char>(0x04);
                                        break;
                                }

                                request = setApduRequest(poClass.getValue(), command, p1, p2, selectData, static_cast<char>(0x00));
                            }
                        }
                    }
                }
            }
        }
    }
}
