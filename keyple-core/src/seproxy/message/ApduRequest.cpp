/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * This program and the accompanying materials are made available under the terms of the Eclipse
 * Public License version 2.0 which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 */

#include "ApduRequest.hpp"

ApduRequest::ApduRequest(std::vector<uint8_t> buffer, bool case4,
                         std::set<int> successfulStatusCodes)
: bytes(buffer),
  case4(case4),
  successfulStatusCodes(successfulStatusCodes)
{

}

ApduRequest::ApduRequest(std::string &name, std::vector<uint8_t> buffer, bool case4,
                         std::set<int> successfulStatusCodes)
: bytes(buffer),
  case4(case4),
  successfulStatusCodes(successfulStatusCodes),
  name(name)
{

}

ApduRequst::ApduRequest(std::vector<uint8_t> buffer, bool case4)
: bytes(buffer),
  case4(case4),
  successfulStatusCodes(nullptr)
{

}

ApduRequest::ApduRequest(std::string &name, std::vector<uint8_t> buffer, bool case4)
: bytes(buffer),
  case4(case4),
  successfulStatusCodes(nullptr),
  name(name)
{

}

bool ApduRequest::isCase4()
{
    return case4;
}

void ApduRequest::setName(const std::string& name)
{
    this->name = name;
}

std::set<int> ApduRequest::getSuccessfulStatusCodes()
{
    return successfulStatusCodes;
}

std::srting& ApduRequest::getName()
{
    return name;
}

std::vector<uint8_t>& ApduRequest::getBytes()
{
    return bytes;
}

std::string& ApduRequest::toString()
{
    /*
    StringBuilder string;
    string = new StringBuilder("ApduRequest: NAME = \"" + this.getName() + "\", RAWDATA = "
                               + ByteArrayUtils.toHex(bytes));
    if (isCase4()) {
    string.append(", case4");
    }
    if (successfulStatusCodes != null) {
    string.append(", additional successful status codes = ");
    Iterator <Integer> iterator = successfulStatusCodes.iterator();
    while (iterator.hasNext()) {
    string.append(String.format("%04X", iterator.next()));
    if (iterator.hasNext()) {
    string.append(", ");
    }
    }
    }
    return string.toString();
     */
    return "";
}