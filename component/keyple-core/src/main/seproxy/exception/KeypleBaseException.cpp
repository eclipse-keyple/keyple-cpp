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

#include "KeypleBaseException.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

KeypleBaseException::KeypleBaseException(const std::string &message) : Exception(message)
{
}

KeypleBaseException::KeypleBaseException(const std::string &message, std::runtime_error cause) : Exception(message, cause)
{
}

}
}
}
}
