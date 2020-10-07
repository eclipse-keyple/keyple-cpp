/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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

#include "ApduResponse.h"
#include "ByteArrayUtil.h"

/* Common */
#include "Arrays.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using namespace keyple::core::util;

ApduResponse::ApduResponse(
  const std::vector<uint8_t>& buffer,
  const std::shared_ptr<std::set<int>> successfulStatusCodes)
: mBytes(buffer)
{
    if (buffer.empty()) {
        mSuccessful = false;
    } else {
        if (buffer.size() < 2)
            throw std::invalid_argument("Bad buffer (length < 2): " +
                                        std::to_string(buffer.size()));

        int statusCode = ((buffer[buffer.size() - 2] & 0x000000FF) << 8) +
                         (buffer[buffer.size() - 1] & 0x000000FF);

        if (successfulStatusCodes != nullptr)
            mSuccessful = statusCode == 0x9000 ||
                          (successfulStatusCodes->find(statusCode) !=
                           successfulStatusCodes->end());
        else
            mSuccessful = statusCode == 0x9000;
    }
}

bool ApduResponse::isSuccessful() const
{
    return mSuccessful;
}

int ApduResponse::getStatusCode() const
{
    if (mBytes.size() < 2) {
        mLogger->debug("bad response length (%)\n", mBytes.size());
        return 0;
    }

    int code = ((mBytes[mBytes.size() - 2] & 0x000000FF) << 8) +
                (mBytes[mBytes.size() - 1] & 0x000000FF);

    return code;
}

const std::vector<uint8_t>& ApduResponse::getBytes() const
{
    return mBytes;
}

std::vector<uint8_t> ApduResponse::getDataOut() const
{
    if (mBytes.size() < 2)
        return std::vector<uint8_t>();

    return Arrays::copyOfRange(mBytes, 0, mBytes.size() - 2);
}

bool ApduResponse::operator==(const ApduResponse& o) const
{
    return mBytes == o.mBytes &&
           mSuccessful == o.mSuccessful;
}

bool ApduResponse::operator!=(const ApduResponse& o) const
{
    return !(*this == o);
}

std::ostream& operator<<(std::ostream& os, const ApduResponse& r)
{
    const std::string status = r.mSuccessful ? "SUCCESS" : "FAILURE";

	os << "R-APDU: {"
	   << "STATUS = " << status << ", "
	   << "BYTES (" << r.mBytes.size() << ") = " << r.mBytes
	   << "}";

	return os;
}

std::ostream& operator<<(std::ostream& os,
	                     const std::shared_ptr<ApduResponse>& r)
{
    if (r)
		os << *(r.get());
    else
		os << "R-APDU: null";

    return os;
}

std::ostream& operator<<(
	std::ostream& os, const std::vector<std::shared_ptr<ApduResponse>>& v)
{
    os << "APDURESPONSES: {";
	for (int i = 0; i < (int)v.size(); i++) {
		os << *(v[i].get());
		if (i != (int)v.size() - 1)
			os << ", ";
    }
	os << "}";

	return os;
}

}
}
}
}
