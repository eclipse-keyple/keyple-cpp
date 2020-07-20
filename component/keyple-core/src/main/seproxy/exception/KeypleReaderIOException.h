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

#include <string>
#include <stdexcept>
#include <memory>

/* Common */
#include "KeypleCoreExport.h"

/* Core */
#include "KeypleReaderException.h"

/* Forward declaration */
namespace keyple { namespace core { namespace seproxy { namespace message {
    class SeResponse; } } } }

namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

/**
 * The exception {@code KeypleReaderIOException} indicates that some IO
 * operations with the reader or SE have failed, generally when the reader was
 * disconnected or card removed.
 */
class KEYPLECORE_API KeypleReaderIOException : public KeypleReaderException {
public:
    /**
     * New exception to be thrown
     *
     * @param message : message to identify the exception and the context
     */
    KeypleReaderIOException(const std::string& msg) : KeypleReaderException(msg)
    {
    }

    /**
     * Encapsulate a lower level reader exception
     *
     * @param message : message to add some context to the exception
     * @param cause : lower level exception
     */
    KeypleReaderIOException(const std::string& msg, const std::exception& cause)
    : KeypleReaderException(msg, cause)
    {
    }

    /**
     *
     */
    const std::vector<std::shared_ptr<SeResponse>>& getSeResponses() const
    {
        return mSeResponses;
    }

    /**
     *
     */
    void setSeResponses(
        const std::vector<std::shared_ptr<SeResponse>>& seResponses)
    {
        mSeResponses = seResponses;
    }

    /**
     *
     */
    std::shared_ptr<SeResponse> getSeResponse() const
    {
        return mSeResponse;
    }

    /**
     *
     */
    void setSeResponse(const std::shared_ptr<SeResponse> seResponse)
    {
        mSeResponse = seResponse;
    }

private:
    /*
     * SeResponse and list of SeResponse objects to carry partial responses in
     * case of a breakdown in communication with the SE.
     */
    std::shared_ptr<SeResponse> mSeResponse;

    /**
     *
     */
    std::vector<std::shared_ptr<SeResponse>> mSeResponses;
};

}
}
}
}
