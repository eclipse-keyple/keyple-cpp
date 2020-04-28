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

#include "ApduResponse.h"
#include "DigestAuthenticateRespPars.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace parser {
namespace security {

using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

DigestAuthenticateRespPars::DigestAuthenticateRespPars(
    std::shared_ptr<ApduResponse> response)
: AbstractSamResponseParser(response)
{
}

}
}
}
}
}
}
