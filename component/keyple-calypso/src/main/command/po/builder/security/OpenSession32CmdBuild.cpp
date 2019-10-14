/* Common */
#include "System.h"

/* Calypso */
#include "OpenSession32CmdBuild.h"
#include "OpenSession32RespPars.h"
#include "PoClass.h"
#include "CalypsoPoCommands.h"
#include "PoRevision.h"
#include "ApduResponse.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace builder {
                            namespace security {

                                using PoClass               = org::eclipse::keyple::calypso::command::PoClass;
                                using CalypsoPoCommands     = org::eclipse::keyple::calypso::command::po::CalypsoPoCommands;
                                using PoRevision            = org::eclipse::keyple::calypso::command::po::PoRevision;
                                using OpenSession32RespPars = org::eclipse::keyple::calypso::command::po::parser::security::OpenSession32RespPars;
                                using ApduResponse          = org::eclipse::keyple::core::seproxy::message::ApduResponse;

                                OpenSession32CmdBuild::OpenSession32CmdBuild(char keyIndex, std::vector<char> &samChallenge, char sfiToSelect, char recordNumberToRead, const std::string &extraInfo)
                                : AbstractOpenSessionCmdBuild<OpenSession32RespPars>(PoRevision::REV3_2)
                                {

                                    char p1 = static_cast<char>((recordNumberToRead * 8) + keyIndex);
                                    char p2 = static_cast<char>((sfiToSelect * 8) + 2);
                                    /*
                                     * case 4: this command contains incoming and outgoing data. We define le = 0, the actual
                                     * length will be processed by the lower layers.
                                     */
                                    char le = 0;

                                    std::vector<char> dataIn(samChallenge.size() + 1);
                                    dataIn[0] = static_cast<char>(0x00);
                                    System::arraycopy(samChallenge, 0, dataIn, 1, samChallenge.size());

                                    this->request = setApduRequest(PoClass::ISO.getValue(),
                                                                   CalypsoPoCommands::getOpenSessionForRev(PoRevision::REV3_2), p1, p2,
                                                                   dataIn, le);
                                    if (extraInfo != "") {
                                        this->addSubName(extraInfo);
                                    }
                                }

                                std::shared_ptr<OpenSession32RespPars> OpenSession32CmdBuild::createResponseParser(std::shared_ptr<ApduResponse> apduResponse) {
                                    return std::make_shared<OpenSession32RespPars>(apduResponse);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
