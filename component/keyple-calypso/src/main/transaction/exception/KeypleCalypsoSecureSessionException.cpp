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

/* Core */
#include "ApduRequest.h"
#include "ApduResponse.h"
#include "KeypleReaderException.h"

/* Calypso */
#include "KeypleCalypsoSecureSessionException.h"

namespace keyple {
namespace calypso {
namespace transaction {
namespace exception {

using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;

KeypleCalypsoSecureSessionException::KeypleCalypsoSecureSessionException(
    const std::string& message, Type type,
    std::vector<std::shared_ptr<ApduRequest>>& requests,
    std::vector<std::shared_ptr<ApduResponse>>& responses)
: KeypleReaderException(message), type(type), requests(requests),
  responses(responses)
{
}

KeypleCalypsoSecureSessionException::KeypleCalypsoSecureSessionException(
    const std::string& message, std::shared_ptr<ApduRequest> req,
    std::shared_ptr<ApduResponse> resp)
: KeypleReaderException(message)
{
    requests.push_back(req);
    responses.push_back(resp);
    type = Type::NO_TYPE;
}

KeypleCalypsoSecureSessionException::KeypleCalypsoSecureSessionException(
    const std::string& message, std::shared_ptr<ApduResponse> resp)
: KeypleCalypsoSecureSessionException(message, nullptr, resp)
{
}

KeypleCalypsoSecureSessionException::KeypleCalypsoSecureSessionException(
    KeypleCalypsoSecureSessionException& o)
: KeypleCalypsoSecureSessionException(o.getMessage(), o.type, o.getRequests(),
                                      o.getResponses())
{
}

KeypleCalypsoSecureSessionException::Type
KeypleCalypsoSecureSessionException::getType()
{
    return type;
}

std::vector<std::shared_ptr<ApduRequest>>&
KeypleCalypsoSecureSessionException::getRequests()
{
    return requests;
}

std::vector<std::shared_ptr<ApduResponse>>&
KeypleCalypsoSecureSessionException::getResponses()
{
    return responses;
}

}
}
}
}
