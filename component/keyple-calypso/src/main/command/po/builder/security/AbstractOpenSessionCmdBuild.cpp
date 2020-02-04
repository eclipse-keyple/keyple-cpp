/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

#include "AbstractOpenSessionCmdBuild.h"
#include "AbstractOpenSessionRespPars.h"
#include "OpenSession10CmdBuild.h"
#include "OpenSession24CmdBuild.h"
#include "OpenSession31CmdBuild.h"
#include "OpenSession32CmdBuild.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security{

using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::builder::security;
using namespace keyple::calypso::command::po::parser::security;
using namespace keyple::core::command;

/*
 * Forward declarations - those classes all need
 * AbstractOpenSessionCommandBuild.h
 */
class OpenSession10CmdBuild;
class OpenSession24CmdBuild;
class OpenSession31CmdBuild;
class OpenSession32CmdBuild;

template<typename T>
std::shared_ptr<AbstractOpenSessionCmdBuild<T>>
    AbstractOpenSessionCmdBuild<T>::create(
        PoRevision revision, uint8_t debitKeyIndex,
        const std::vector<uint8_t>& sessionTerminalChallenge, uint8_t sfi,
        uint8_t recordNb, const std::string& extraInfo)
{
    switch (revision) {
    case PoRevision::REV1_0:
        {
        std::shared_ptr<OpenSession10CmdBuild> cmd =
            std::make_shared<OpenSession10CmdBuild>(
                debitKeyIndex, sessionTerminalChallenge, sfi, recordNb,
                extraInfo);
        return std::shared_ptr<AbstractOpenSessionCmdBuild<T>>(
                   cmd, reinterpret_cast<AbstractOpenSessionCmdBuild<T> *>(
                            cmd.get()));
        }
    case PoRevision::REV2_4:
        {
        std::shared_ptr<OpenSession24CmdBuild> cmd =
            std::make_shared<OpenSession24CmdBuild>(debitKeyIndex,
                sessionTerminalChallenge, sfi, recordNb, extraInfo);
        return std::shared_ptr<AbstractOpenSessionCmdBuild<T>>(
                   cmd, reinterpret_cast<AbstractOpenSessionCmdBuild<T> *>(
                            cmd.get()));
        }
    case PoRevision::REV3_1:
    case PoRevision::REV3_1_CLAP:
        {
        std::shared_ptr<OpenSession31CmdBuild> cmd =
            std::make_shared<OpenSession31CmdBuild>(debitKeyIndex,
                sessionTerminalChallenge, sfi, recordNb, extraInfo);
        return std::shared_ptr<AbstractOpenSessionCmdBuild<T>>(
                   cmd, reinterpret_cast<AbstractOpenSessionCmdBuild<T> *>(
                            cmd.get()));
        }
    case PoRevision::REV3_2:
        {
        std::shared_ptr<OpenSession32CmdBuild> cmd =
            std::make_shared<OpenSession32CmdBuild>(debitKeyIndex,
                sessionTerminalChallenge, sfi, recordNb, extraInfo);
        return std::shared_ptr<AbstractOpenSessionCmdBuild<T>>(
                   cmd, reinterpret_cast<AbstractOpenSessionCmdBuild<T> *>(
                            cmd.get()));
        }
    default:
        throw std::invalid_argument(
                  StringHelper::formatSimple(
                      "Revision %d isn't supported",
                      static_cast<int>(revision)));
    }
}

template class AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>;

}
}
}
}
}
}

