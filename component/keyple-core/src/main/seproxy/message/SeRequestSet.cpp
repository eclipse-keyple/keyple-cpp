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

#include "SeRequestSet.h"
#include "SeRequest.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

SeRequestSet::SeRequestSet(std::shared_ptr<std::set<std::shared_ptr<SeRequest>>> seRequests)
: sortedRequests(seRequests)
{
}

SeRequestSet::SeRequestSet(std::shared_ptr<SeRequest> seRequest)
{
    sortedRequests->insert(seRequest);
}

void SeRequestSet::add(std::shared_ptr<SeRequest> seRequest)
{
    this->sortedRequests->insert(seRequest);
}

std::shared_ptr<std::set<std::shared_ptr<SeRequest>>> SeRequestSet::getRequests()
{
    return sortedRequests;
}

std::shared_ptr<SeRequest> SeRequestSet::getSingleRequest()
{
    if (sortedRequests->size() != 1) {
        throw std::make_shared<IllegalStateException>("This method only support ONE element");
    }
    return *(sortedRequests->begin());
}

std::string SeRequestSet::toString()
{
    return std::to_string(sortedRequests->size()) + " SeRequest(s)";
}

}
}
}
}
