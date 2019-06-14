#include "SelectDiversifierCmdBuild.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace sam {
                        namespace builder {
                            namespace security {
                                using CalypsoSamCommands = org::eclipse::keyple::calypso::command::sam::CalypsoSamCommands;
                                using SamCommandBuilder = org::eclipse::keyple::calypso::command::sam::SamCommandBuilder;
                                using SamRevision = org::eclipse::keyple::calypso::command::sam::SamRevision;

                                SelectDiversifierCmdBuild::SelectDiversifierCmdBuild(SamRevision revision, std::vector<char> &diversifier) : SamCommandBuilder(std::make_shared<CalypsoSamCommands>(command), nullptr)
                                {
                                    this->defaultRevision = revision;

                                    if (diversifier.empty() || (diversifier.size() != 4 && diversifier.size() != 8)) {
                                        throw std::invalid_argument("Bad diversifier value!");
                                    }
                                    char cla = this->defaultRevision.getClassByte();
                                    char p1 = 0x00;
                                    char p2 = 0x00;

                                    request = setApduRequest(cla, std::make_shared<CalypsoSamCommands>(command), p1, p2, diversifier, -1);

                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
