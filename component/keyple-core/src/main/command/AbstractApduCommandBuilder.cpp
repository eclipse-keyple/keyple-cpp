/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#include <iostream>

#include "AbstractApduCommandBuilder.h"

#include "ApduRequest.h"

namespace keyple {
namespace core {
namespace command {

using namespace keyple::core::seproxy::message;

AbstractApduCommandBuilder::AbstractApduCommandBuilder(
  const std::shared_ptr<SeCommand> commandRef,
  const std::shared_ptr<ApduRequest> request)
: mCommandRef(commandRef), mRequest(request), mName(commandRef->getName())
{
    /* Set APDU name for non null request */
    if (request != nullptr)
        mRequest->setName(commandRef->getName());
}

AbstractApduCommandBuilder::AbstractApduCommandBuilder(
  const std::string& name, std::shared_ptr<ApduRequest> request)
: mCommandRef(nullptr), mRequest(request), mName(name)
{
    /* Set APDU name for non null request */
    if (request != nullptr)
        mRequest->setName(name);
}

void AbstractApduCommandBuilder::addSubName(const std::string& subName)
{
    if (subName.length() != 0) {
        mName += " - " + subName;
        if (mRequest != nullptr)
            mRequest->setName(mName);
    }
}

const std::shared_ptr<SeCommand> AbstractApduCommandBuilder::getCommandRef() const
{
    return mCommandRef;
}

const std::string& AbstractApduCommandBuilder::getName() const
{
    return mName;
}

const std::shared_ptr<ApduRequest> AbstractApduCommandBuilder::getApduRequest() const
{
    return mRequest;
}

}
}
}
