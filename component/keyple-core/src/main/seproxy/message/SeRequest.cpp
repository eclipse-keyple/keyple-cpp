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

#include "SeRequest.h"
#include "SeSelector.h"
#include "ApduRequest.h"
#include "SeProtocol_Import.h"
#include "Pattern.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::protocol;

SeRequest::SeRequest(std::shared_ptr<SeSelector> seSelector,
                     std::vector<std::shared_ptr<ApduRequest>>& apduRequests)
: seSelector(seSelector)
{
    this->apduRequests = apduRequests;
}

SeRequest::SeRequest(std::vector<std::shared_ptr<ApduRequest>>& apduRequests)
: seSelector(nullptr)
{
    this->apduRequests = apduRequests;
}

std::shared_ptr<SeSelector> SeRequest::getSeSelector() const
{
    return seSelector;
}

std::vector<std::shared_ptr<ApduRequest>> SeRequest::getApduRequests() const
{
    return apduRequests;
}

std::ostream& operator<<(std::ostream& os, const SeRequest& se)
{
    os << "SEREQUEST: {"
       << "REQUESTS = " << se.apduRequests << ", "
       << "SELECTOR = " << se.seSelector
	   << "}";

	return os;
}

std::ostream& operator<<(std::ostream& os, const std::shared_ptr<SeRequest>& se)
{
	if (se)
		os << *(se.get());
    else
		os << "SEREQUEST: null";

    return os;
}

std::ostream& operator<<(std::ostream& os,
                         const std::set<std::shared_ptr<SeRequest>>& s)
{
	os << "SEREQUESTS: {";
	for (const auto& sr : s) {
		if (sr != *s.begin())
            os << ", ";
        os << sr;
    }
	os << "}";

	return os;
}

}
}
}
}
