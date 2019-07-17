/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#include "AbstractOpenSessionCmdBuild.h"
#include "AbstractOpenSessionRespPars.h"
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
namespace security{

using namespace org::eclipse::keyple::calypso::command::po;
using namespace org::eclipse::keyple::calypso::command::po::builder::security;
using namespace org::eclipse::keyple::calypso::command::po::parser::security;
using namespace org::eclipse::keyple::core::command;

/* Forward declarations - those classes all need AbstractOpenSessionCommandBuild.h */
class OpenSession10CmdBuild;
class OpenSession24CmdBuild;
class OpenSession31CmdBuild;
class OpenSession32CmdBuild;

template<typename T>
std::shared_ptr<AbstractOpenSessionCmdBuild<T>> AbstractOpenSessionCmdBuild<T>::create(PoRevision revision, char debitKeyIndex,
                                                                                      std::vector<char> &sessionTerminalChallenge, char sfi,
                                                                                      char recordNb, const std::string &extraInfo)
{
    switch (revision) {
    case PoRevision::REV1_0:
        return std::dynamic_pointer_cast<AbstractOpenSessionCmdBuild<T>>(
                   std::make_shared<OpenSession10CmdBuild>(debitKeyIndex, sessionTerminalChallenge, sfi, recordNb, extraInfo));
    case PoRevision::REV2_4:
        return std::dynamic_pointer_cast<AbstractOpenSessionCmdBuild<T>>(
                   std::make_shared<OpenSession24CmdBuild>(debitKeyIndex, sessionTerminalChallenge, sfi, recordNb, extraInfo));
    case PoRevision::REV3_1:
    case PoRevision::REV3_1_CLAP:
        return std::dynamic_pointer_cast<AbstractOpenSessionCmdBuild<T>>(
                   std::make_shared<OpenSession31CmdBuild>(debitKeyIndex, sessionTerminalChallenge, sfi, recordNb, extraInfo));
    case PoRevision::REV3_2:
        return std::dynamic_pointer_cast<AbstractOpenSessionCmdBuild<T>>(
                    std::make_shared<OpenSession32CmdBuild>(debitKeyIndex, sessionTerminalChallenge, sfi, recordNb, extraInfo));
    default:
        throw std::invalid_argument(StringHelper::formatSimple("Revision %d isn't supported", static_cast<int>(revision)));
    }
}

template class AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>;


}
}
}
}
}
}
}
}

