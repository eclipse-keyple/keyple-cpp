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

#pragma once

#include <memory>

#include "AbstractSamCommandBuilder.h"
#include "SendableInSession.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace sam {

using namespace org::eclipse::keyple::calypso::command;

using AbstractSamCommandBuilder = org::eclipse::keyple::calypso::command::sam::AbstractSamCommandBuilder;

class SamSendableInSession : public SendableInSession<AbstractSamCommandBuilder>
{
};

}
}
}
}
}
}
