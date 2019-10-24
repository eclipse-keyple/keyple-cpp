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

KeypleReaderException::KeypleReaderException(const std::string &message)
: KeypleBaseException(message)
{
}

KeypleReaderException::KeypleReaderException(const std::string &message,
                                             std::runtime_error cause)
: KeypleBaseException(message, cause)
{
}

KeypleReaderException::KeypleReaderException(
  const KeypleReaderException& o)
: KeypleBaseException(o.getMessage(), o.getCause())
{
}

std::shared_ptr<SeResponseSet> KeypleReaderException::getSeResponseSet()
{
    return seResponseSet;
}

void KeypleReaderException::setSeResponseSet(
  std::shared_ptr<SeResponseSet> seResponseSet)
{
    this->seResponseSet = seResponseSet;
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
