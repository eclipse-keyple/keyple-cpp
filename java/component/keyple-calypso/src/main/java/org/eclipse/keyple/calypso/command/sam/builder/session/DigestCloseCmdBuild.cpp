#include "DigestCloseCmdBuild.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace sam {
                        namespace builder {
                            namespace session {
                                using CalypsoSamCommands = org::eclipse::keyple::calypso::command::sam::CalypsoSamCommands;
                                using SamCommandBuilder = org::eclipse::keyple::calypso::command::sam::SamCommandBuilder;
                                using SamRevision = org::eclipse::keyple::calypso::command::sam::SamRevision;

                                DigestCloseCmdBuild::DigestCloseCmdBuild(SamRevision revision, char expectedResponseLength) throw(std::invalid_argument) : org::eclipse::keyple::calypso::command::sam::SamCommandBuilder(command, nullptr) {
                                    if (revision != nullptr) {
                                        this->defaultRevision = revision;
                                    }
                                    if (expectedResponseLength != 0x04 && expectedResponseLength != 0x08) {
                                        throw std::invalid_argument(StringHelper::formatSimple("Bad digest length! Expected 4 or 8, got %s", expectedResponseLength));
                                    }

                                    char cla = this->defaultRevision.getClassByte();
                                    char p1 = 0x00;
                                    char p2 = static_cast<char>(0x00);

                                    // CalypsoRequest calypsoRequest = new CalypsoRequest(cla, command, p1, p2, null,
                                    // expectedResponseLength);
                                    request = setApduRequest(cla, command, p1, p2, nullptr, expectedResponseLength);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
