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

#include <string>
#include <stdexcept>
#include <memory>

/* Common */
#include "Export.h"

/* Core */
#include "KeypleBaseException.h"
#include "SeResponse.h"
#include "SeResponseSet.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

using namespace keyple::core::seproxy::message;

/**
 * Base Exceptions thrown in a {@link ProxyReader} context
 */
class EXPORT KeypleReaderException : public KeypleBaseException {
public:
    /**
     * New exception to be thrown
     *
     * @param message : message to identify the exception and the context
     */
    KeypleReaderException(const std::string &message);

    /**
     * Encapsulate a lower level reader exception
     *
     * @param message : message to add some context to the exception
     * @param cause : lower level exception
     */
    KeypleReaderException(const std::string &message, std::runtime_error cause);

    /**
     *
     */
    KeypleReaderException(const KeypleReaderException& o);

    /**
     *
     */
    virtual ~KeypleReaderException() {}

    /**
     * Setters and Getters for SeResponseSet and SeResponse
     */
    virtual std::shared_ptr<SeResponseSet> getSeResponseSet();

    virtual void setSeResponseSet(std::shared_ptr<SeResponseSet> seResponseSet);

    virtual std::shared_ptr<SeResponse> getSeResponse();

    virtual void setSeResponse(std::shared_ptr<SeResponse> seResponse);

protected:
    /**
     *
     */
    std::shared_ptr<KeypleReaderException> shared_from_this()
    {
        return std::static_pointer_cast<KeypleReaderException>(KeypleBaseException::shared_from_this());
    }

private:
    /**
     * SeResponseSet and SeResponse objects to carry partial responses in the event of a breakdown
     * in communication with the SE.
     */
    std::shared_ptr<SeResponseSet> seResponseSet;

    /**
     *
     */
    std::shared_ptr<SeResponse> seResponse;


};

}
}
}
}
