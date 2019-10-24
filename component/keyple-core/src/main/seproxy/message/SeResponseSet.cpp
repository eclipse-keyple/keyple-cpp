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

#include "SeResponseSet.h"
#include "SeResponse.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

std::vector<std::shared_ptr<SeResponse>> SeResponseSet::getResponses()
{
    return seResponses;
}

SeResponseSet::SeResponseSet(std::vector<std::shared_ptr<SeResponse>> &seResponses) : seResponses(seResponses)
{
}

SeResponseSet::SeResponseSet(std::shared_ptr<SeResponse> response)
{
    std::vector<std::shared_ptr<SeResponse>> seResponses;
    seResponses.push_back(response);
}

std::shared_ptr<SeResponse> SeResponseSet::getSingleResponse()
{
    if (seResponses.size() != 1) {
        throw std::make_shared<IllegalStateException>("This method only support ONE element");
    }
    return seResponses[0];
}

std::string SeResponseSet::toString()
{
    return std::to_string(seResponses.size()) + " SeReponse(s)";
}

}
}
}
}
