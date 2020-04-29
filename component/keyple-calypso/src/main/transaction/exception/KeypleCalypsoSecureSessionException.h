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

#pragma once

/* Core */
#include "ApduRequest.h"
#include "ApduResponse.h"
#include "KeypleReaderException.h"

#include <string>
#include <vector>
#include <memory>

namespace keyple {
namespace calypso {
namespace transaction {
namespace exception {

using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;

class KeypleCalypsoSecureSessionException : public KeypleReaderException {
public:
    /**
     *
     */
    enum class Type { NO_TYPE, PO, SAM };

    /**
     *
     */
    KeypleCalypsoSecureSessionException(
        const std::string& message, Type type,
        std::vector<std::shared_ptr<ApduRequest>>& requests,
        std::vector<std::shared_ptr<ApduResponse>>& responses);

    /**
     *
     */
    KeypleCalypsoSecureSessionException(const std::string& message,
                                        std::shared_ptr<ApduRequest> req,
                                        std::shared_ptr<ApduResponse> resp);

    /**
     *
     */
    KeypleCalypsoSecureSessionException(const std::string& message,
                                        std::shared_ptr<ApduResponse> resp);

    /**
     *
     */
    KeypleCalypsoSecureSessionException(KeypleCalypsoSecureSessionException& o);

    /**
     *
     */
    virtual ~KeypleCalypsoSecureSessionException()
    {
    }

    /**
     *
     */
    virtual Type getType();

    /**
     *
     */
    virtual std::vector<std::shared_ptr<ApduRequest>>& getRequests();

    /**
     *
     */
    virtual std::vector<std::shared_ptr<ApduResponse>>& getResponses();

private:
    /**
     *
     */
    Type type;

    /**
     *
     */
    std::vector<std::shared_ptr<ApduRequest>> requests;

    /**
     *
     */
    std::vector<std::shared_ptr<ApduResponse>> responses;
};

}
}
}
}
