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

#include "KeypleReaderException.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

using namespace keyple::core::seproxy::message;

KeypleReaderException::KeypleReaderException(const std::string& msg)
: KeypleBaseException(msg)
{
}

KeypleReaderException::KeypleReaderException(
  const std::string& msg, const std::exception& cause)
: KeypleBaseException(msg, cause)
{
}

KeypleReaderException::KeypleReaderException(const KeypleReaderException& o)
: KeypleBaseException(o.getMessage(), o.getCause())
{
}

std::list<std::shared_ptr<SeResponse>>&
    KeypleReaderException::getSeResponseSet()
{
    return seResponseList;
}

void KeypleReaderException::setSeResponseSet(
    std::list<std::shared_ptr<SeResponse>> seResponseList)
{
    this->seResponseList = seResponseList;
}

std::shared_ptr<SeResponse> KeypleReaderException::getSeResponse()
{
    return seResponse;
}

void KeypleReaderException::setSeResponse(
  std::shared_ptr<SeResponse> seResponse)
{
    this->seResponse = seResponse;
}

}
}
}
}
