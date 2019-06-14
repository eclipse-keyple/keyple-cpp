#include "OpenSession10CmdBuild.h"
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

                                OpenSession10CmdBuild::OpenSession10CmdBuild(char keyIndex, std::vector<char> &samChallenge, char sfiToSelect, char recordNumberToRead, const std::string &extraInfo)
                                : AbstractOpenSessionCmdBuild(PoRevision::REV1_0)
                                {

                                    if (keyIndex == 0x00) {
                                        throw std::invalid_argument("Key index can't be null for rev 1.0!");
                                    }

                                    char p1 = static_cast<char>((recordNumberToRead * 8) + keyIndex);
                                    char p2 = static_cast<char>(sfiToSelect * 8);
                                    /*
                                     * case 4: this command contains incoming and outgoing data. We define le = 0, the actual
                                     * length will be processed by the lower layers.
                                     */
                                    char le = 0;

                                    this->request = setApduRequest(PoClass::LEGACY.getValue(), std::make_shared<CalypsoPoCommands>(CalypsoPoCommands::getOpenSessionForRev(PoRevision::REV1_0)), p1, p2, samChallenge, le);
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
