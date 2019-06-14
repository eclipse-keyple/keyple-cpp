#include "OpenSession31CmdBuild.h"
#include "../../../PoClass.h"
#include "../../CalypsoPoCommands.h"
#include "../../PoRevision.h"

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
                                using PoRevision = org::eclipse::keyple::calypso::command::po::PoRevision;

                                OpenSession31CmdBuild::OpenSession31CmdBuild(char keyIndex, std::vector<char> &samChallenge, char sfiToSelect, char recordNumberToRead, const std::string &extraInfo)
                                : AbstractOpenSessionCmdBuild(PoRevision::REV3_1)
                                {

                                    char p1 = static_cast<char>((recordNumberToRead * 8) + keyIndex);
                                    char p2 = static_cast<char>((sfiToSelect * 8) + 1);
                                    /*
                                     * case 4: this command contains incoming and outgoing data. We define le = 0, the actual
                                     * length will be processed by the lower layers.
                                     */
                                    char le = 0;

                                    this->request = setApduRequest(PoClass::ISO.getValue(), std::make_shared<CalypsoPoCommands>(CalypsoPoCommands::getOpenSessionForRev(PoRevision::REV3_1)), p1, p2, samChallenge, le);
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
}
