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

#include "ApduRequest.h"
#include "ByteArrayUtil.h"
#include "stringhelper.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using namespace keyple::core::util;

ApduRequest::ApduRequest(const std::vector<uint8_t>& buffer, bool case4,
                         std::shared_ptr<std::set<int>> successfulStatusCodes)
: case4(case4), successfulStatusCodes(successfulStatusCodes)
{
    this->bytes = buffer;
}

ApduRequest::ApduRequest(const std::string& name,
                         const std::vector<uint8_t>& buffer, bool case4,
                         std::shared_ptr<std::set<int>> successfulStatusCodes)
: ApduRequest(buffer, case4, successfulStatusCodes)
{
    this->name = name;
}

ApduRequest::ApduRequest(const std::vector<uint8_t>& buffer, bool case4)
: ApduRequest(buffer, case4, nullptr)
{
}

ApduRequest::ApduRequest(const std::string& name,
                         const std::vector<uint8_t>& buffer, bool case4)
: ApduRequest(buffer, case4, nullptr)
{
    this->name = name;
}

bool ApduRequest::isCase4() const
{
    return case4;
}

void ApduRequest::setName(const std::string& name)
{
    this->name = name;
}

std::shared_ptr<std::set<int>> ApduRequest::getSuccessfulStatusCodes()
{
    return successfulStatusCodes;
}

std::string ApduRequest::getName() const
{
    return name;
}

const std::vector<uint8_t>& ApduRequest::getBytes()
{
    return this->bytes;
}

std::string ApduRequest::toString() const
{
    std::string string = StringHelper::formatSimple(
        "ApduRequest: NAME = %s, RAWDATA = %s", this->getName(),
        ByteArrayUtil::toHex(bytes));

    if (isCase4()) {
        string.append(", case4");
    }

    if (successfulStatusCodes != nullptr) {
        string.append(", additional successful status codes = ");
        std::set<int>::const_iterator iterator = successfulStatusCodes->begin();
        while (iterator != successfulStatusCodes->end()) {
            string.append(StringHelper::formatSimple("%04X", *iterator));
            if (iterator != successfulStatusCodes->end()) {
                string.append(", ");
            }
            iterator++;
        }
    }

    return string;
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

std::ostream& operator<<(std::ostream& os, const ApduRequest& se)
{
	os << "APDUREQUEST: {"
	   << "NAME = " << se.getName() << ", "
	   << "RAWDATA = " << se.bytes << ", "
	   << "CASE4 = " << se.case4;

    if (se.successfulStatusCodes != nullptr) {
        os << "ADD.SUCCESFULSTATUSCODES: {";
        std::set<int>::const_iterator iterator =
			se.successfulStatusCodes->begin();
        while (iterator != se.successfulStatusCodes->end()) {
            os << std::hex << std::setfill('0') << std::setw(4)
			   << static_cast<int>(*iterator);
            if (iterator != se.successfulStatusCodes->end()) {
                os << ", ";
            }
            iterator++;
        }
		os << "}";
    }

	os << "}";

    return os;
}

std::ostream& operator<<(std::ostream& os,
	                     const std::shared_ptr<ApduRequest>& se)
{
    if (se)
		os << *(se.get());
    else
		os << "APDUREQUEST: null";

    return os;
}

std::ostream& operator<<(std::ostream& os,
                         const std::vector<std::shared_ptr<ApduRequest>>& se)
{
	os << "ApduRequests: {";
	for (const auto& r : se) {
		os << *r.get();
		if (r != se.back())
			os << ", ";
	}
	os << "}";

	return os;
}

}
}
}
}
