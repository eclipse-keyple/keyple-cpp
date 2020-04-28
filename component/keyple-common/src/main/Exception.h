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

#include <exception>
#include <stdexcept>
#include <iostream>

namespace keyple {
namespace common {

class Exception : public std::exception {
public:
    Exception(const std::string& message) : message(message)
    {
    }

    Exception(const std::string& message, const std::exception cause)
    : message(message), cause(cause)
    {
    }

    /**
	 * Returns the detail message string of this exception.
	 */
    std::string getMessage() const
    {
        return message;
    }

    /**
	 * Returns the cause of the exception.
	 */
    const std::exception getCause() const
    {
        return cause;
    }

    /**
     *
     */
    friend std::ostream& operator<<(std::ostream& os, const Exception& e)
    {
		os << "EXCEPTION: {"
           << "MESSAGE = " << e.message << ", "
           << "CAUSE = " << e.cause.what()
		   << "}";

        return os;
    }

private:
    /**
	 *
	 */
    const std::string message;

    /**
	 *
	 */
    const std::exception cause;
};

}
}
