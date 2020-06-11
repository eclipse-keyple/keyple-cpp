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

#include "AbstractSeProxyComponent.h"

namespace keyple {
namespace core {
namespace seproxy {

AbstractSeProxyComponent::AbstractSeProxyComponent(const std::string& name)
: mName(name)
{
}

const std::string& AbstractSeProxyComponent::getName() const
{
    return mName;
}

void AbstractSeProxyComponent::setParameters(
    const std::map<std::string, std::string>& parameters)
{
    for (const auto& en : parameters)
        setParameter(en.first, en.second);

}

}
}
}
