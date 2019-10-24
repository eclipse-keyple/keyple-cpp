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

#include "ApduRequest.h"
#include "ByteArrayUtil.h"
#include "stringhelper.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using namespace keyple::core::util;

ApduRequest::ApduRequest(std::vector<char> &buffer, bool case4, std::shared_ptr<std::set<int>> successfulStatusCodes)
: case4(case4), successfulStatusCodes(successfulStatusCodes)
{
    this->bytes = buffer;
}

ApduRequest::ApduRequest(const std::string &name, std::vector<char> &buffer, bool case4,
                         std::shared_ptr<std::set<int>> successfulStatusCodes)
: ApduRequest(buffer, case4, successfulStatusCodes)
{
    this->name = name;
}

ApduRequest::ApduRequest(std::vector<char> &buffer, bool case4) : ApduRequest(buffer, case4, nullptr)
{
}

ApduRequest::ApduRequest(const std::string &name, std::vector<char> &buffer, bool case4) : ApduRequest(buffer, case4, nullptr)
{
    this->name = name;
}

bool ApduRequest::isCase4()
{
    return case4;
}

void ApduRequest::setName(const std::string &name)
{
    this->name = name;
}

std::shared_ptr<std::set<int>> ApduRequest::getSuccessfulStatusCodes()
{
    return successfulStatusCodes;
}

std::string ApduRequest::getName()
{
    return name;
}

std::vector<char> ApduRequest::getBytes()
{
    return this->bytes;
}

std::string ApduRequest::toString()
{
    std::shared_ptr<StringBuilder> string;
    string = std::make_shared<StringBuilder>("ApduRequest: NAME = \"" + this->getName() + "\", RAWDATA = " + ByteArrayUtil::toHex(bytes));
    if (isCase4()) {
        string->append(", case4");
    }
    if (successfulStatusCodes != nullptr) {
        string->append(", additional successful status codes = ");
        std::set<int>::const_iterator iterator = successfulStatusCodes->begin();
        while (iterator != successfulStatusCodes->end()) {
            string->append(StringHelper::formatSimple("%04X", *iterator));
            if (iterator != successfulStatusCodes->end()) {
                string->append(", ");
            }
            iterator++;
        }
    }
    return string->toString();
}

bool equals(std::shared_ptr<void> o)
{
                        
    (void)o;

    /* To be implemented */
    return false;
}

int hashCode()
{
    /* To be implemented */
    return 0;
}

}
}
}
}
