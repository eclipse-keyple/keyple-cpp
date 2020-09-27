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

#pragma once

#include <exception>
#include <stdexcept>
#include <iostream>

namespace keyple {
namespace common {

class Exception : public std::exception {
public:
    /**
     *
     */
    Exception() {}

    /**
     *
     */
    Exception(const std::string& message) : mMessage(message) {}

    /**
     *
     */
    Exception(const std::string& message, const std::exception cause)
    : mMessage(message), mCause(cause)
    {
    }

    /**
	 * Returns the detail message string of this exception.
	 */
    const std::string& getMessage() const
    {
        return mMessage;
    }

    /**
	 * Returns the cause of the exception.
	 */
    const std::exception& getCause() const
    {
        return mCause;
    }

    /**
     *
     */
    friend std::ostream& operator<<(std::ostream& os, const Exception& e)
    {
		os << "EXCEPTION: {"
           << "MESSAGE = " << e.mMessage << ", "
           << "CAUSE = " << e.mCause.what()
		   << "}";

        return os;
    }

private:
    /**
	 *
	 */
    const std::string mMessage;

    /**
	 *
	 */
    const std::exception mCause;
};

}
}
