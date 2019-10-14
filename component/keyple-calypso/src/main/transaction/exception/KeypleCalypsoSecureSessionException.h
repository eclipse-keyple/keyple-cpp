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

#pragma once

#include "KeypleReaderException.h"

#include <string>
#include <vector>
#include <memory>


namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace transaction {
namespace exception {

using KeypleReaderException = org::eclipse::keyple::core::seproxy::exception::KeypleReaderException;
using ApduRequest           = org::eclipse::keyple::core::seproxy::message::ApduRequest;
using ApduResponse          = org::eclipse::keyple::core::seproxy::message::ApduResponse;

class KeypleCalypsoSecureSessionException : public KeypleReaderException {

public:
    enum class Type {
        NO_TYPE,
        PO,
        SAM
    };

private:
    Type type;
    std::vector<std::shared_ptr<ApduRequest>> requests;
    std::vector<std::shared_ptr<ApduResponse>> responses;

public:
    KeypleCalypsoSecureSessionException(const std::string &message, Type type, std::vector<std::shared_ptr<ApduRequest>> &requests, std::vector<std::shared_ptr<ApduResponse>> &responses);

    KeypleCalypsoSecureSessionException(const std::string &message, std::shared_ptr<ApduRequest> req, std::shared_ptr<ApduResponse> resp);

    KeypleCalypsoSecureSessionException(const std::string &message, std::shared_ptr<ApduResponse> resp);

    virtual Type getType();

    virtual std::vector<std::shared_ptr<ApduRequest>> getRequests();

    virtual std::vector<std::shared_ptr<ApduResponse>> getResponses();

protected:
    std::shared_ptr<KeypleCalypsoSecureSessionException> shared_from_this() {
        return std::static_pointer_cast<KeypleCalypsoSecureSessionException>(KeypleReaderException::shared_from_this());
    }
};

}
}
}
}
}
}
