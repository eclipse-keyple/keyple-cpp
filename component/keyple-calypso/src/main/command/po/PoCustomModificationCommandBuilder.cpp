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

#include "AbstractIso7816CommandBuilder.h"
#include "ApduRequest.h"
#include "PoCustomModificationCommandBuilder.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {

using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

PoCustomModificationCommandBuilder::PoCustomModificationCommandBuilder(const std::string &name,
                                                              std::shared_ptr<ApduRequest> request)
: AbstractIso7816CommandBuilder("PO Custom Modification Command: " + name, request)
{
}

}
}
}
}
