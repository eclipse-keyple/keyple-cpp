#include "AbstractOpenSessionCmdBuild.h"
#include "OpenSession10CmdBuild.h"
#include "OpenSession24CmdBuild.h"
#include "OpenSession31CmdBuild.h"
#include "OpenSession32CmdBuild.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace builder {
                            namespace session {
                                using CalypsoPoCommands = org::eclipse::keyple::calypso::command::po::CalypsoPoCommands;
                                using PoCommandBuilder = org::eclipse::keyple::calypso::command::po::PoCommandBuilder;
                                using PoRevision = org::eclipse::keyple::calypso::command::po::PoRevision;

                                AbstractOpenSessionCmdBuild::AbstractOpenSessionCmdBuild(PoRevision revision)
                                : PoCommandBuilder(std::make_shared<CalypsoPoCommands>(CalypsoPoCommands::getOpenSessionForRev(revision)), nullptr) {
                                }

                                std::shared_ptr<AbstractOpenSessionCmdBuild> AbstractOpenSessionCmdBuild::create(PoRevision revision, char debitKeyIndex, std::vector<char> &sessionTerminalChallenge, char sfi, char recordNb, const std::string &extraInfo) {
                                    switch (revision) {
                                        case PoRevision::REV1_0:
                                            return std::make_shared<OpenSession10CmdBuild>(debitKeyIndex, sessionTerminalChallenge, sfi, recordNb, extraInfo);
                                        case PoRevision::REV2_4:
                                            return std::make_shared<OpenSession24CmdBuild>(debitKeyIndex, sessionTerminalChallenge, sfi, recordNb, extraInfo);
                                        case PoRevision::REV3_1:
                                        case PoRevision::REV3_1_CLAP:
                                            return std::make_shared<OpenSession31CmdBuild>(debitKeyIndex, sessionTerminalChallenge, sfi, recordNb, extraInfo);
                                        case PoRevision::REV3_2:
                                            return std::make_shared<OpenSession32CmdBuild>(debitKeyIndex, sessionTerminalChallenge, sfi, recordNb, extraInfo);
                                        default:
                                            throw std::invalid_argument("Revision <to print: revision> isn't supported");
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
